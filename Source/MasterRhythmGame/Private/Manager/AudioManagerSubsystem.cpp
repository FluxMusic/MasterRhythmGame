// Fill out your copyright notice in the Description page of Project Settings.

#include "Manager/AudioManagerSubsystem.h"
#include "Quartz/AudioMixerClockHandle.h"
#include "Quartz/QuartzSubsystem.h"
#include "MetasoundOutput.h"
#include "Components/AudioComponent.h"
#include "Sound/QuartzQuantizationUtilities.h"
#include "Kismet/KismetSystemLibrary.h"
#include "MetaSoundOutputSubsystem.h"
#include "Actor/SplineActor.h"
#include "Components/SplineComponent.h"
#include "Enemy/TestEnemyActor.h"
#include "Kismet/GameplayStatics.h"
#include "Controller/GameController.h"
#include "Data/LevelData.h"
#include "GameInstance/MyGameInstance.h"
#include "HUD/GameHUD.h"

UAudioManagerSubsystem::UAudioManagerSubsystem()
{
	// Ensure we have a valid enemy class to search for. If none has been set, default to the concrete actor class.
	if (EnemyClass == nullptr)
	{
		EnemyClass = ATestEnemyActor::StaticClass();
	}

	// Find an existing actor of the specified enemy class in the world and cache it.
	AActor* FoundEnemy = UGameplayStatics::GetActorOfClass(GetWorld(), EnemyClass);
	Enemy = Cast<ATestEnemyActor>(FoundEnemy);
}

void UAudioManagerSubsystem::InitSubsystem(ULevelData* LevelData)
{
	if (QuartzSubsystem == nullptr)
	{
		QuartzSubsystem = UQuartzSubsystem::Get(GetWorld());
	}
	if (LevelData)
	{
		RootNote = LevelData->RootNote;
		PartFinish = LevelData->NumSegments;
		
		// Initialize score system from LevelData
		BaseScorePerNote = LevelData->BaseScorePerNote;
		ComboMultiplierMin = LevelData->ComboMultiplierMin;
		ComboMultiplierMax = LevelData->ComboMultiplierMax;
		ComboMultiplierIncrement = LevelData->ComboMultiplierIncrement;
		
		// Reset score for new level
		CurrentScore = 0;
		CurrentComboMultiplier = ComboMultiplierMin;
		CurrentComboCount = 0;
		bScoringEnabled = true;
	}
	
}

void UAudioManagerSubsystem::StartClock()
{
	if (ClockHandle != nullptr)
	{
		ClockHandle->StartClock(this, ClockHandle);

		if (QuartzSubsystem)
		{
			LatencyBetweenThreads = QuartzSubsystem->GetAudioRenderThreadToGameThreadMaxLatency();
		}
	}
}

void UAudioManagerSubsystem::ClockHandleInit(FName CLockName)
{
	if (QuartzSubsystem != nullptr)
	{
		FQuartzClockSettings Settings;
		ClockHandle = QuartzSubsystem->CreateNewClock(GetWorld(), CLockName, Settings, true);
	}
}

void UAudioManagerSubsystem::SetBeatsPerMinute(float InBPM, FQuartzQuantizationBoundary InBoundary, FOnQuartzCommandEventBP InDelegate)
{
	if (ClockHandle != nullptr)
	{
		ClockHandle->SetBeatsPerMinute(this, InBoundary, InDelegate, ClockHandle, InBPM);

		FOnQuartzMetronomeEventBP BeatDelegate;
		BeatDelegate.BindUFunction(this, FName(TEXT("OnQuartzClockBeat")));
		ClockHandle->SubscribeToQuantizationEvent(GetWorld(), EQuartzCommandQuantization::Beat, BeatDelegate, ClockHandle);
	}
}

void UAudioManagerSubsystem::PlayQuantized(UAudioComponent* AudioComponent)
{
	if ((AudioComponent != nullptr) && (ClockHandle != nullptr))
	{
		FOnQuartzCommandEventBP InDelegate;
		FQuartzQuantizationBoundary QuantBoundary(EQuartzCommandQuantization::Bar, 1.0f, EQuarztQuantizationReference::BarRelative, true);
		AudioComponent->PlayQuantized(this, ClockHandle, QuantBoundary, InDelegate, 0.0f, 0.0f, 1.0f, EAudioFaderCurve::Linear);
	}

	// remember active audio component so StartPart* functions can trigger it
	ActiveAudioComponent = AudioComponent;

	UMetaSoundOutputSubsystem* MetaSoundOutputSubsystem = nullptr;
	if (UWorld* World = GetWorld())
	{
		MetaSoundOutputSubsystem = World->GetSubsystem<UMetaSoundOutputSubsystem>();
	}

	if (MetaSoundOutputSubsystem != nullptr)
	{
		FOnMetasoundOutputValueChanged MidiDelegate;
		MidiDelegate.BindUFunction(this, FName("WatchOutputMidiNoteChange"));
		MetaSoundOutputSubsystem->WatchOutput(AudioComponent, FName(TEXT("MIDINoteOut")), MidiDelegate);

		FOnMetasoundOutputValueChanged PartNameDelegate;
		PartNameDelegate.BindUFunction(this, FName("WatchOutputPartFinishedName"));
		MetaSoundOutputSubsystem->WatchOutput(AudioComponent, FName(TEXT("PartFinishedName")), PartNameDelegate);

		FOnMetasoundOutputValueChanged PartFinishedDelegate;
		PartFinishedDelegate.BindUFunction(this, FName("WatchOutputPartFinishedPart"));
		MetaSoundOutputSubsystem->WatchOutput(AudioComponent, FName(TEXT("OnPartFinished")), PartFinishedDelegate);

		FOnMetasoundOutputValueChanged PartPercentDelegate;
		PartPercentDelegate.BindUFunction(this, FName("WatchOutputPartFinishedPercent"));
		MetaSoundOutputSubsystem->WatchOutput(AudioComponent, FName(TEXT("PartFinishPercent")), PartPercentDelegate);
	}
	StartMusic();
}

float UAudioManagerSubsystem::GetAnimTime()
{
	if (ClockHandle)
	{
		FQuartzTransportTimeStamp ClockTimestamp = ClockHandle->GetCurrentTimestamp(this);

		const float SyncDuration = AnimLength - AnimStartupTime;

		float ContinuousBeat = ClockTimestamp.Beat + ClockTimestamp.BeatFraction;

		float IdleBeatPhase = FMath::Fmod(ContinuousBeat, LoopLength);

		if (IdleBeatPhase < 0.f)
		{
			IdleBeatPhase += LoopLength;
		}
		
		float MusicalPhase = IdleBeatPhase / 2.f;

		AnimTime = AnimStartupTime + (MusicalPhase * SyncDuration);
	}

	return AnimTime;
}

void UAudioManagerSubsystem::WatchOutputPartFinishedName(FName OutputName, const FMetaSoundOutput& Output)
{
	// Try to extract a string name from the MetaSound output that indicates which part finished.
	FString PartName;
	if (Output.Get<FString>(PartName))
	{
		PartNameFix = PartName;
	}
}

void UAudioManagerSubsystem::WatchOutputPartFinishedPercent(FName OutputName, const FMetaSoundOutput& Output)
{
	if (GameHUD == nullptr)
	{
		if (APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0))
		{
			GameHUD = Cast<AGameHUD>(PC->GetHUD());
		}
	}

	if (!bPart1IntroEnded)
	{
		if (GameHUD != nullptr)
		{
			if (UMainGameWidget* MainGameWidget = GameHUD->GetMainGameInstance())
			{
				MainGameWidget->SetCompletionPercent(0.0f);
			}
		}
		return;
	}

	float Value = 0.0f;
	if (Output.Get<float>(Value))
	{
		const bool bNormalized = (Value >= 0.0f && Value <= 1.0f);
		const float NormalizedValue = bNormalized ? Value : (Value / 100.0f);

		if (GameHUD != nullptr)
		{
			if (UMainGameWidget* MainGameWidget = GameHUD->GetMainGameInstance())
			{
				MainGameWidget->SetCompletionPercent(NormalizedValue);
			}
		}
	}
}

void UAudioManagerSubsystem::StartIntro()
{
	if (ActiveAudioComponent)
	{
		ActiveAudioComponent->SetTriggerParameter("PlayIntro");
	}
}

void UAudioManagerSubsystem::StartPart1Intro()
{
	if (ActiveAudioComponent)
	{
		ActiveAudioComponent->SetTriggerParameter("PlayPart1Intro");
	}
}

void UAudioManagerSubsystem::StartPart1()
{
	if (ActiveAudioComponent)
	{
		ActiveAudioComponent->SetTriggerParameter("PlayPart1");
	}
}

void UAudioManagerSubsystem::StartPart2Intro()
{
	if (ActiveAudioComponent)
	{
		ActiveAudioComponent->SetTriggerParameter("PlayPart2Intro");
	}
}

void UAudioManagerSubsystem::StartPart2()
{
	if (ActiveAudioComponent)
	{
		ActiveAudioComponent->SetTriggerParameter("PlayPart2");
	}
}

void UAudioManagerSubsystem::StartPart3Intro()
{
	if (ActiveAudioComponent)
	{
		ActiveAudioComponent->SetTriggerParameter("PlayPart3Intro");
	}
}

void UAudioManagerSubsystem::StartPart3()
{
	if (ActiveAudioComponent)
	{
		ActiveAudioComponent->SetTriggerParameter("PlayPart3");
	}
}

void UAudioManagerSubsystem::StartPart4Intro()
{
	if (ActiveAudioComponent)
	{
		ActiveAudioComponent->SetTriggerParameter("PlayPart4Intro");
	}
}

void UAudioManagerSubsystem::StartPart4()
{
	if (ActiveAudioComponent)
	{
		ActiveAudioComponent->SetTriggerParameter("PlayPart4");
	}
}

void UAudioManagerSubsystem::StartPart5Intro()
{
	if (ActiveAudioComponent)
	{
		ActiveAudioComponent->SetTriggerParameter("PlayPart5Intro");
	}
}

void UAudioManagerSubsystem::StartPart5()
{
	if (ActiveAudioComponent)
	{
		ActiveAudioComponent->SetTriggerParameter("PlayPart5");
	}
}

void UAudioManagerSubsystem::StartMusic()
{
	if (ActiveAudioComponent)
	{
		ActiveAudioComponent->SetTriggerParameter("PlayIntro");
	}
}

void UAudioManagerSubsystem::StartOutro()
{
	if (ActiveAudioComponent)
	{
		ActiveAudioComponent->SetTriggerParameter("PlayOutro");
	}
}

void UAudioManagerSubsystem::StartOutroLoop()
{
	if (ActiveAudioComponent)
	{
		ActiveAudioComponent->SetTriggerParameter("PlayOutroLoop");
	}
}

void UAudioManagerSubsystem::StopClock()
{
	if (QuartzSubsystem && ClockHandle)
	{
		// Stop the underlying clock and clear the handle
		ClockHandle->StopClock(this, true, ClockHandle);
		ClockHandle = nullptr;
	}
}

void UAudioManagerSubsystem::OnQuartzClockBeat(FName ClockName, EQuartzCommandQuantization QuantizationType,
	int32 NumBars, int32 Beat, float BeatFraction)
{
	if (QuartzSubsystem)
	{
		// LastBeatAudioTimeSeconds = QuartzSubsystem->GetAudioRenderThreadToGameThreadMaxLatency();
		LastBeatAudioTimeSeconds = FPlatformTime::Seconds();
	}

	UKismetSystemLibrary::PrintString(this, FString::FormatAsNumber(Beat), true, true, FLinearColor::Green, 10.0f);
}

void UAudioManagerSubsystem::WatchOutputMidiNoteChange(FName OutputName, const FMetaSoundOutput& Output)
{
	//Test for Latency
	MetaSoundOutputTimeSeconds = FPlatformTime::Seconds();

	// Extract MIDI note as int32 and print it
	int32 MidiNote = -1;
	if (Output.Get<int32>(MidiNote))
	{
		if (MidiNote == 0)
		{
			return;
		}
		MidiNote = (MidiNote - static_cast<int32>(RootNote)) % 12;

		const FString Msg = FString::Printf(TEXT("%s = %d"), *OutputName.ToString(), MidiNote);
		UKismetSystemLibrary::PrintString(this, Msg, true, true, FLinearColor::Red, 5.0f);
	}

	// Ensure we have a valid enemy class to search for. If none has been set, default to the concrete actor class.
	if (EnemyClass == nullptr)
	{
		EnemyClass = ATestEnemyActor::StaticClass();
	}

	// Find an existing actor of the specified enemy class in the world and cache it.
	AActor* FoundEnemy = UGameplayStatics::GetActorOfClass(GetWorld(), EnemyClass);
	Enemy = Cast<ATestEnemyActor>(FoundEnemy);

	// Ensure Spline is set at runtime (find the first ASplineActor in the world if not already assigned).
	if (Spline == nullptr)
	{
		AActor* FoundSpline = UGameplayStatics::GetActorOfClass(GetWorld(), ASplineActor::StaticClass());
		Spline = Cast<ASplineActor>(FoundSpline);
	}

	//Move the Enemy to the corresponding Line based on the Midi Note
	if (Enemy != nullptr && Spline != nullptr)
	{
		Enemy->SetSplineRef(Spline->GetSplines(MidiNote));

		const int32 Index = Enemy->GetSplineRef()->GetNumberOfSplinePoints() - 1;

		FVector SplineWorldLocation = Enemy->GetSplineRef()->GetLocationAtSplinePoint(Index, ESplineCoordinateSpace::World);               

		Enemy->SetActorLocation(SplineWorldLocation);

		if (bEnemyCanAttack)
		{
			Enemy->Attack(Enemy->GetBPM());
		}
	}
}

void UAudioManagerSubsystem::WatchOutputPartFinishedPart(FName OutputName, const FMetaSoundOutput& Output)
{
	FString WorldName = GetWorld()->GetMapName();

	if (PartNameFix != PartPlayed)
	{
		PartPlayed = PartNameFix;
		bPartAlreadyPlayed = false;
	}
	else
	{
		bPartAlreadyPlayed = true;
	}

	if (bPartAlreadyPlayed)
	{
		SetScoringEnabled(false);
	}
	else
	{
		SetScoringEnabled(true);
	}

	switch (GetMapTypeFromString(WorldName))
	{
		case EMapNames::Swamp:
		{
			HandleSwampLevel();
			break;
		}
		case EMapNames::TestMap:
		{
			HandleTestLevel();
			break;
		}
		case EMapNames::Cyberpunk:
		{
			HandleCyberpunkLevel();
			break;
		}
		case EMapNames::Ice:
		{	
			HandleIceLevel();
			break;
		}
		case EMapNames::Steampunk:
		{
			HandleSteampunkLevel();
			break;
		}
		case EMapNames::Space:
		{
			HandleSpaceLevel();
			break;
		}
		case EMapNames::Volcano:
		{
			HandleVolcanoLevel();
			break;
		}
		default:
		{
			break;
		}
	}
}

void UAudioManagerSubsystem::MuteLeads()
{
	if (ActiveAudioComponent)
	{
		ActiveAudioComponent->SetTriggerParameter("MuteLeads");
		UE_LOG(LogTemp, Warning, TEXT("MuteLeads triggered"));
	}
}

void UAudioManagerSubsystem::UnmuteLeads()
{
	if (ActiveAudioComponent)
	{
		ActiveAudioComponent->SetTriggerParameter("UnmuteLeads");
		UE_LOG(LogTemp, Warning, TEXT("UnmuteLeads triggered"));
	}
}

void UAudioManagerSubsystem::AddScore(int32 BasePoints)
{
	if (!bScoringEnabled)
	{
		return;
	}

	// Calculate score with combo multiplier
	int32 PointsToAdd = FMath::RoundToInt(BasePoints * CurrentComboMultiplier);
	CurrentScore += PointsToAdd;
	CurrentComboCount++;

	// Increase combo multiplier
	CurrentComboMultiplier = FMath::Min(CurrentComboMultiplier + ComboMultiplierIncrement, ComboMultiplierMax);

	// Update UI
	if (GameHUD != nullptr && GameHUD->GetMainGameInstance() != nullptr)
	{
		GameHUD->GetMainGameInstance()->UpdateScore(CurrentScore, CurrentComboMultiplier);
	}

	UE_LOG(LogTemp, Log, TEXT("Score Added: %d (Base: %d x Multiplier: %.2f) | Total Score: %d | Combo: %d"), 
		PointsToAdd, BasePoints, CurrentComboMultiplier, CurrentScore, CurrentComboCount);
}

void UAudioManagerSubsystem::ResetCombo()
{
	CurrentComboMultiplier = ComboMultiplierMin;
	CurrentComboCount = 0;

	// Update UI
	if (GameHUD != nullptr && GameHUD->GetMainGameInstance() != nullptr)
	{
		GameHUD->GetMainGameInstance()->UpdateScore(CurrentScore, CurrentComboMultiplier);
	}

	UE_LOG(LogTemp, Warning, TEXT("Combo Reset! Multiplier back to 1.0"));
}

void UAudioManagerSubsystem::SetScoringEnabled(bool bEnabled)
{
	bScoringEnabled = bEnabled;
	
	if (!bEnabled)
	{
		UE_LOG(LogTemp, Warning, TEXT("Scoring Disabled (Part Repeat)"));
	}
	else
	{
		UE_LOG(LogTemp, Log, TEXT("Scoring Enabled"));
	}
}

void UAudioManagerSubsystem::HandleSwampLevel()
{
	// Immediately select the next part based on the reported name.
	if (PartNameFix.Equals(TEXT("IntroEnd")))
	{
		StartPart1Intro();
	}
	else if (PartNameFix.Equals(TEXT("Part1IntroEnd")))
	{
		StartPart1();
		bEnemyCanAttack = true;

		// Mark that Part1Intro has finished so percentage updates are allowed
		bPart1IntroEnded = true;

		UnmuteLeads();
	}
	else if (PartNameFix.Equals(TEXT("Part1End")))
	{
		HandlePart1(true, TEXT("MAP_Swamp_Begin"), TEXT("MAP_Swamp_Endless"));
	}
	else if (PartNameFix.Equals(TEXT("Part2IntroEnd")))
	{
		StartPart2();
		bEnemyCanAttack = true;

		UnmuteLeads();
	}
	else if (PartNameFix.Equals(TEXT("Part2End")))
	{
		HandlePart2(true, TEXT("MAP_Swamp_Endless"), TEXT("MAP_Swamp_End"));
	}
	else if (PartNameFix.Equals(TEXT("Part3IntroEnd")))
	{
		StartPart3();
		bEnemyCanAttack = true;

		UnmuteLeads();
	}
	else if (PartNameFix.Equals(TEXT("Part3End")))
	{
		HandlePart3(false);
	}
	else if (PartNameFix.Equals(TEXT("OutroEnd")))
	{
		HandleOutro();
	}
}

void UAudioManagerSubsystem::HandleVolcanoLevel()
{
	// Immediately select the next part based on the reported name.
	if (PartNameFix.Equals(TEXT("IntroEnd")))
	{
		StartPart1Intro();
	}
	else if (PartNameFix.Equals(TEXT("Part1IntroEnd")))
	{
		StartPart1();
		bEnemyCanAttack = true;

		// Mark that Part1Intro has finished so percentage updates are allowed
		bPart1IntroEnded = true;

		UnmuteLeads();
	}
	else if (PartNameFix.Equals(TEXT("Part1End")))
	{
		HandlePart1(true, TEXT("MAP_Vulcano_Segment_1"), TEXT("MAP_Vulcano_Segment_2"));
	}
	else if (PartNameFix.Equals(TEXT("Part2IntroEnd")))
	{
		StartPart2();
		bEnemyCanAttack = true;

		UnmuteLeads();
	}
	else if (PartNameFix.Equals(TEXT("Part2End")))
	{
		HandlePart2(true, TEXT("MAP_Vulcano_Segment_2"), TEXT("MAP_Vulcano_Segment_3"));
	}
	else if (PartNameFix.Equals(TEXT("Part3IntroEnd")))
	{
		StartPart3();
		bEnemyCanAttack = true;

		UnmuteLeads();
	}
	else if (PartNameFix.Equals(TEXT("Part3End")))
	{
		HandlePart3(true, TEXT("MAP_Vulcano_Segment_3"), TEXT("MAP_Vulcano_Segment_4"));
	}
	else if (PartNameFix.Equals("Part4IntroEnd"))
	{
		StartPart4();
		bEnemyCanAttack = true;

		UnmuteLeads();
	}
	else if (PartNameFix.Equals("Part4End"))
	{
		HandlePart4(true, TEXT("MAP_Vulcano_Segment_4"), TEXT("MAP_Vulcano_Segment_5"));
	}
	else if (PartNameFix.Equals("Part5IntroEnd"))
	{
		StartPart5();
		bEnemyCanAttack = true;

		UnmuteLeads();
	}
	else if (PartNameFix.Equals("Part5End"))
	{
		HandlePart5();
	}
	else if (PartNameFix.Equals(TEXT("OutroEnd")))
	{
		HandleOutro();
	}
}

void UAudioManagerSubsystem::HandleSpaceLevel()
{
	// Immediately select the next part based on the reported name.
	if (PartNameFix.Equals(TEXT("IntroEnd")))
	{
		StartPart1Intro();
	}
	else if (PartNameFix.Equals(TEXT("Part1IntroEnd")))
	{
		StartPart1();
		bEnemyCanAttack = true;

		// Mark that Part1Intro has finished so percentage updates are allowed
		bPart1IntroEnded = true;

		UnmuteLeads();
	}
	else if (PartNameFix.Equals(TEXT("Part1End")))
	{
		HandlePart1(true, TEXT("MAP_Galaxy_Segment_1"), TEXT("MAP_Galaxy_Segment_2"));
	}
	else if (PartNameFix.Equals(TEXT("Part2IntroEnd")))
	{
		StartPart2();
		bEnemyCanAttack = true;

		UnmuteLeads();
	}
	else if (PartNameFix.Equals(TEXT("Part2End")))
	{
		HandlePart2(true, TEXT("MAP_Galaxy_Segment_2"), TEXT("MAP_Galaxy_Segment_3_Endless"));
	}
	else if (PartNameFix.Equals(TEXT("Part3IntroEnd")))
	{
		StartPart3();
		bEnemyCanAttack = true;

		UnmuteLeads();
	}
	else if (PartNameFix.Equals(TEXT("Part3End")))
	{
		HandlePart3(true, TEXT("MAP_Galaxy_Segment_3_Endless"), TEXT("MAP_Galaxy_Segment_4"));
	}
	else if (PartNameFix.Equals("Part4IntroEnd"))
	{
		StartPart4();
		bEnemyCanAttack = true;

		UnmuteLeads();
	}
	else if (PartNameFix.Equals("Part4End"))
	{
		HandlePart4(false);
	}
	else if (PartNameFix.Equals(TEXT("OutroEnd")))
	{
		HandleOutro();
	}
}

void UAudioManagerSubsystem::HandleIceLevel()
{
	// Immediately select the next part based on the reported name.
	if (PartNameFix.Equals(TEXT("IntroEnd")))
	{
		StartPart1Intro();
	}
	else if (PartNameFix.Equals(TEXT("Part1IntroEnd")))
	{
		StartPart1();
		bEnemyCanAttack = true;

		// Mark that Part1Intro has finished so percentage updates are allowed
		bPart1IntroEnded = true;

		UnmuteLeads();
	}
	else if (PartNameFix.Equals(TEXT("Part1End")))
	{
		HandlePart1(true, TEXT("MAP_Ice_1"), TEXT("MAP_Ice_2"));
	}
	else if (PartNameFix.Equals(TEXT("Part2IntroEnd")))
	{
		StartPart2();
		bEnemyCanAttack = true;

		UnmuteLeads();
	}
	else if (PartNameFix.Equals(TEXT("Part2End")))
	{
		HandlePart2(true, TEXT("MAP_Ice_2"), TEXT("MAP_Ice_3"));
	}
	else if (PartNameFix.Equals(TEXT("Part3IntroEnd")))
	{
		StartPart3();
		bEnemyCanAttack = true;

		UnmuteLeads();
	}
	else if (PartNameFix.Equals(TEXT("Part3End")))
	{
		HandlePart3(true, TEXT("MAP_Ice_3"), TEXT("MAP_Ice_4"));
	}
	else if (PartNameFix.Equals("Part4IntroEnd"))
	{
		StartPart4();
		bEnemyCanAttack = true;

		UnmuteLeads();
	}
	else if (PartNameFix.Equals("Part4End"))
	{
		HandlePart4(false);
	}
	else if (PartNameFix.Equals(TEXT("OutroEnd")))
	{
		HandleOutro();
	}
}

void UAudioManagerSubsystem::HandleCyberpunkLevel()
{
	// Immediately select the next part based on the reported name.
	if (PartNameFix.Equals(TEXT("IntroEnd")))
	{
		StartPart1Intro();
	}
	else if (PartNameFix.Equals(TEXT("Part1IntroEnd")))
	{
		StartPart1();
		bEnemyCanAttack = true;

		// Mark that Part1Intro has finished so percentage updates are allowed
		bPart1IntroEnded = true;

		UnmuteLeads();
	}
	else if (PartNameFix.Equals(TEXT("Part1End")))
	{
		HandlePart1(true, TEXT("MAP_Cyberpunk_First_Segment"), TEXT("MAP_Cyberpunk_Second_Segment"));
	}
	else if (PartNameFix.Equals(TEXT("Part2IntroEnd")))
	{
		StartPart2();
		bEnemyCanAttack = true;

		UnmuteLeads();
	}
	else if (PartNameFix.Equals(TEXT("Part2End")))
	{
		HandlePart2(true, TEXT("MAP_Cyberpunk_Second_Segment"), TEXT("MAP_Cyberpunk_Third_Segment"));
	}
	else if (PartNameFix.Equals(TEXT("Part3IntroEnd")))
	{
		StartPart3();
		bEnemyCanAttack = true;

		UnmuteLeads();
	}
	else if (PartNameFix.Equals(TEXT("Part3End")))
	{
		HandlePart3(true, TEXT("MAP_Cyberpunk_Third_Segment"), TEXT("MAP_Cyberpunk_Fourth_Segment"));
	}
	else if (PartNameFix.Equals("Part4IntroEnd"))
	{
		StartPart4();
		bEnemyCanAttack = true;

		UnmuteLeads();
	}
	else if (PartNameFix.Equals("Part4End"))
	{
		HandlePart4(false);
	}
	else if (PartNameFix.Equals(TEXT("OutroEnd")))
	{
		HandleOutro();
	}
}

void UAudioManagerSubsystem::HandleSteampunkLevel()
{
	// Immediately select the next part based on the reported name.
	if (PartNameFix.Equals(TEXT("IntroEnd")))
	{
		StartPart1Intro();
	}
	else if (PartNameFix.Equals(TEXT("Part1IntroEnd")))
	{
		StartPart1();
		bEnemyCanAttack = true;

		// Mark that Part1Intro has finished so percentage updates are allowed
		bPart1IntroEnded = true;

		UnmuteLeads();
	}
	else if (PartNameFix.Equals(TEXT("Part1End")))
	{
		HandlePart1(true, TEXT("MAP_Steampunk_1"), TEXT("MAP_Steampunk_2"));
	}
	else if (PartNameFix.Equals(TEXT("Part2IntroEnd")))
	{
		StartPart2();
		bEnemyCanAttack = true;

		UnmuteLeads();
	}
	else if (PartNameFix.Equals(TEXT("Part2End")))
	{
		HandlePart2(true, TEXT("MAP_Steampunk_2"), TEXT("MAP_Steampunk_3"));
	}
	else if (PartNameFix.Equals(TEXT("Part3IntroEnd")))
	{
		StartPart3();
		bEnemyCanAttack = true;

		UnmuteLeads();
	}
	else if (PartNameFix.Equals(TEXT("Part3End")))
	{
		HandlePart3(true, TEXT("MAP_Steampunk_3"), TEXT("MAP_Steampunk_4"));
	}
	else if (PartNameFix.Equals("Part4IntroEnd"))
	{
		StartPart4();
		bEnemyCanAttack = true;

		UnmuteLeads();
	}
	else if (PartNameFix.Equals("Part4End"))
	{
		HandlePart4(false);
	}
	else if (PartNameFix.Equals(TEXT("OutroEnd")))
	{
		HandleOutro();
	}
}

void UAudioManagerSubsystem::HandleTestLevel()
{
	// Immediately select the next part based on the reported name.
	if (PartNameFix.Equals(TEXT("IntroEnd")))
	{
		StartPart1Intro();
	}
	else if (PartNameFix.Equals(TEXT("Part1IntroEnd")))
	{
		StartPart1();
		bEnemyCanAttack = true;

		// Mark that Part1Intro has finished so percentage updates are allowed
		bPart1IntroEnded = true;

		UnmuteLeads();
	}
	else if (PartNameFix.Equals(TEXT("Part1End")))
	{
		HandlePart1(false);
	}
	else if (PartNameFix.Equals(TEXT("Part2IntroEnd")))
	{
		StartPart2();
		bEnemyCanAttack = true;

		UnmuteLeads();
	}
	else if (PartNameFix.Equals(TEXT("Part2End")))
	{
		HandlePart2(false);
	}
	else if (PartNameFix.Equals(TEXT("Part3IntroEnd")))
	{
		StartPart3();
		bEnemyCanAttack = true;

		UnmuteLeads();
	}
	else if (PartNameFix.Equals(TEXT("Part3End")))
	{
		HandlePart3(false);
	}
	else if (PartNameFix.Equals("Part4IntroEnd"))
	{
		StartPart4();
		bEnemyCanAttack = true;

		UnmuteLeads();
	}
	else if (PartNameFix.Equals("Part4End"))
	{
		HandlePart4(false);
	}
	else if (PartNameFix.Equals("Part5IntroEnd"))
	{
		StartPart5();
		bEnemyCanAttack = true;

		UnmuteLeads();
	}
	else if (PartNameFix.Equals("Part5End"))
	{
		HandlePart5();
	}
	else if (PartNameFix.Equals(TEXT("OutroEnd")))
	{
		HandleOutro();
	}
}

void UAudioManagerSubsystem::HandlePart1(bool LoadOtherLevel, FName UnloadLevelName, FName LoadLevelName)
{
	if ((Enemy != nullptr) && (Enemy->GetHealth1() > 0) && (bEnemyCanAttack))
	{
		bEnemyCanAttack = false;
		StartPart1();

		MuteLeads();
	}
	else if ((Enemy != nullptr) && (Enemy->GetHealth1() > 0) && (!bEnemyCanAttack))
	{
		StartPart1();
		bEnemyCanAttack = true;

		UnmuteLeads();
	}
	else
	{
		if (LoadOtherLevel)
		{
			if (UMyGameInstance* Gi = UMyGameInstance::Get())
			{
				Gi->UnloadLevel(UnloadLevelName);
				if (GameHUD != nullptr)
				{
					GameHUD->GetBlackscreenInstance()->SetVisibility(ESlateVisibility::Visible);
					GameHUD->GetBlackscreenInstance()->FadeOut();
				}
				Gi->LoadLevel(LoadLevelName);
			}
		}

		StartPart2Intro();
	}
}

void UAudioManagerSubsystem::HandlePart2(bool LoadOtherLevel, FName UnloadLevelName, FName LoadLevelName)
{
	if ((Enemy != nullptr) && (Enemy->GetHealth2() > 0) && (bEnemyCanAttack))
	{
		bEnemyCanAttack = false;
		StartPart2();

		MuteLeads();
	}
	else if ((Enemy != nullptr) && (Enemy->GetHealth2() > 0) && (!bEnemyCanAttack))
	{
		StartPart2();
		bEnemyCanAttack = true;
		UnmuteLeads();
	}
	else
	{
		if (LoadOtherLevel)
		{
			if (UMyGameInstance* Gi = UMyGameInstance::Get())
			{
				Gi->UnloadLevel(UnloadLevelName);
				if (GameHUD != nullptr)
				{
					GameHUD->GetBlackscreenInstance()->SetVisibility(ESlateVisibility::Visible);
					GameHUD->GetBlackscreenInstance()->FadeOut();
				}
				Gi->LoadLevel(LoadLevelName);
			}
		}

		StartPart3Intro();
	}
}

void UAudioManagerSubsystem::HandlePart3(bool LoadOtherLevel, FName UnloadLevelName, FName LoadLevelName)
{
	if ((Enemy != nullptr) && (Enemy->GetHealth3() > 0) && (bEnemyCanAttack))
	{
		bEnemyCanAttack = false;
		StartPart3();
		MuteLeads();
	}
	else if ((Enemy != nullptr) && (Enemy->GetHealth3() > 0) && (!bEnemyCanAttack))
	{
		StartPart3();
		bEnemyCanAttack = true;
		UnmuteLeads();
	}
	else if ((Enemy != nullptr) && (Enemy->GetHealth3() <= 0) && PartFinish == EPartFinish::Three)
	{
		StartOutro();
	}
	else
	{
		if (LoadOtherLevel)
		{
			if (UMyGameInstance* Gi = UMyGameInstance::Get())
			{
				Gi->UnloadLevel(UnloadLevelName);
				if (GameHUD != nullptr)
				{
					GameHUD->GetBlackscreenInstance()->SetVisibility(ESlateVisibility::Visible);
					GameHUD->GetBlackscreenInstance()->FadeOut();
				}
				Gi->LoadLevel(LoadLevelName);
			}
		}

		StartPart4Intro();
	}
}

void UAudioManagerSubsystem::HandlePart4(bool LoadOtherLevel, FName UnloadLevelName, FName LoadLevelName)
{
	if ((Enemy != nullptr) && (Enemy->GetHealth4() > 0) && (bEnemyCanAttack))
	{
		bEnemyCanAttack = false;
		StartPart4();
		MuteLeads();
	}
	else if ((Enemy != nullptr) && (Enemy->GetHealth4() > 0) && (!bEnemyCanAttack))
	{
		StartPart4();
		bEnemyCanAttack = true;
		UnmuteLeads();
	}
	else if ((Enemy != nullptr) && (Enemy->GetHealth4() <= 0) && PartFinish == EPartFinish::Four)
	{
		StartOutro();
	}
	else
	{
		if (LoadOtherLevel)
		{
			if (UMyGameInstance* Gi = UMyGameInstance::Get())
			{
				Gi->UnloadLevel(UnloadLevelName);
				if (GameHUD != nullptr)
				{
					GameHUD->GetBlackscreenInstance()->SetVisibility(ESlateVisibility::Visible);
					GameHUD->GetBlackscreenInstance()->FadeOut();
				}
				Gi->LoadLevel(LoadLevelName);
			}
		}

		StartPart5Intro();
	}
}

void UAudioManagerSubsystem::HandlePart5()
{
	if ((Enemy != nullptr) && (Enemy->GetHealth5() > 0) && (bEnemyCanAttack))
	{
		bEnemyCanAttack = false;
		StartPart5();
		MuteLeads();
	}
	else if ((Enemy != nullptr) && (Enemy->GetHealth5() > 0) && (!bEnemyCanAttack))
	{
		StartPart5();
		bEnemyCanAttack = true;
		UnmuteLeads();
	}
	else if (Enemy != nullptr && Enemy->GetHealth5() <= 0 && PartFinish == EPartFinish::Five)
	{
		StartOutro();
	}
}

void UAudioManagerSubsystem::HandleOutro()
{
	UKismetSystemLibrary::PrintString(this, FString(TEXT("All parts finished")), true, true, FLinearColor::Blue, 5.0f);
	StopClock();

	if (UMyGameInstance* Gi = UMyGameInstance::Get())
	{
		switch (GetMapTypeFromString(GetWorld()->GetMapName()))
		{
			case EMapNames::Swamp:
			{
				Gi->SetLevelOneUnlocked(true);
				break;
			}
			case EMapNames::Ice:
			{
				Gi->SetLevelFiveUnlocked(true);
				break;
			}
			case EMapNames::Cyberpunk:
			{
				Gi->SetLevelTwoUnlocked(true);
				break;
			}
			case EMapNames::Space:
			{
				Gi->SetLevelFourUnlocked(true);
				break;
			}
			case EMapNames::Steampunk:
			{
				Gi->SetLevelThreeUnlocked(true);
				break;
			}
			case EMapNames::Volcano:
			{
				Gi->SetLevelSixUnlocked(true);
				break;
			}
			case EMapNames::TestMap:
			{
				UKismetSystemLibrary::PrintString(this, FString(TEXT("This is where the Level would be set unlocked")), true, true, FLinearColor::Blue, 5.0f);
				break;
			}
		}
	}

	//Show Success Menu
	GameHUD->GetSuccessWidgetInstance()->SetVisibility(ESlateVisibility::Visible);
	AGameController* PlayerController = Cast<AGameController>(GEngine->GetFirstLocalPlayerController(GetWorld()));
	PlayerController->SetControllerState(EControllerStateGame::SuccessMenu);

	// Show mouse cursor and switch to GameAndUI input so widgets receive focus
	PlayerController->bShowMouseCursor = true;
	FInputModeGameAndUI InputMode;
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	PlayerController->SetInputMode(InputMode);
}


EMapNames UAudioManagerSubsystem::GetMapTypeFromString(const FString& MapName)
{
	if (MapName.Contains(TEXT("MAP_Swamp")))
	{
		return EMapNames::Swamp;
	}
	else if (MapName.Contains(TEXT("MAP_Cyberpunk")))
	{
		return EMapNames::Cyberpunk;
	}
	else if (MapName.Contains(TEXT("MAP_Ice")))
	{
		return EMapNames::Ice;
	}
	else if (MapName.Contains(TEXT("MAP_Steampunk")))
	{
		return EMapNames::Steampunk;
	}
	else if (MapName.Contains(TEXT("MAP_Galaxy")))
	{
		return EMapNames::Space;
	}
	else if (MapName.Contains(TEXT("MAP_Vulcano")))
	{
		return EMapNames::Volcano;
	}
	else if (MapName.Contains(TEXT("TestMap")))
	{
		return EMapNames::TestMap;
	}
	
	// Default fallback
	UE_LOG(LogTemp, Warning, TEXT("Unknown map name: %s"), *MapName);
	return EMapNames::TestMap;
}
