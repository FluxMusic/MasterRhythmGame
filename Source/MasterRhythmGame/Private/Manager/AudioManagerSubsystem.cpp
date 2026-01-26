// Fill out your copyright notice in the Description page of Project Settings.

#include "Manager/AudioManagerSubsystem.h"
#include "Quartz/AudioMixerClockHandle.h"
#include "Quartz/QuartzSubsystem.h"
#include "MetasoundOutput.h"
#include "Components/AudioComponent.h"
#include "Sound/QuartzQuantizationUtilities.h"
#include "AudioParameterControllerInterface.h"
#include "Kismet/KismetSystemLibrary.h"
#include "MetaSoundOutputSubsystem.h"
#include "Actor/SplineActor.h"
#include "Components/SplineComponent.h"
#include "Enemy/TestEnemyActor.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"
#include "Components/TextBlock.h"
#include "Controller/GameController.h"
#include "Data/LevelData.h"
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

	// If Part1IntroEnd has not happened yet, always show "0%"
	if (!bPart1IntroEnded)
	{
		if (GameHUD != nullptr)
		{
			if (UTextBlock* CompletionPercent = GameHUD->GetMainGameInstance()->GetCompletionPercent())
			{
				CompletionPercent->SetText(FText::FromString(TEXT("0%")));
			}
		}
		return;
	}

	// Try to extract a float value from the MetaSound output
	float Value = 0.0f;
	if (Output.Get<float>(Value))
	{
		// If the MetaSound outputs a normalized value [0..1], convert to percent
		const bool bNormalized = (Value >= 0.0f && Value <= 1.0f);
		const float DisplayValue = bNormalized ? Value * 100.0f : Value;

		// Round to nearest integer so no decimal places are shown
		const int32 IntPercent = FMath::RoundToInt(DisplayValue);

		// Only show the percentage number and percent sign (e.g. "42%")
		const FString Msg = FString::Printf(TEXT("%d%%"), IntPercent);

		if (GameHUD != nullptr)
		{
			if (UTextBlock* CompletionPercent = GameHUD->GetMainGameInstance()->GetCompletionPercent())
			{
				CompletionPercent->SetText(FText::FromString(Msg));
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
	// Immediately select the next part based on the reported name.
	if (PartNameFix.Equals(TEXT("IntroEnd")))
	{
		StartPart1Intro();
	}
	else if (PartNameFix.Equals(TEXT("Part1IntroEnd")))
	{
		StartPart1();
		bEnemyCanAttack = true;
		bPlayAgain = true;
		
		// Mark that Part1Intro has finished so percentage updates are allowed
		bPart1IntroEnded = true;

		UnmuteLeads();
	}
	else if (PartNameFix.Equals(TEXT("Part1End")))
	{
		// Check if loop needed -> Check if Enemy no life
		if ((Enemy != nullptr) && (Enemy->GetHealth1() > 0) && (bEnemyCanAttack))
		{
			bEnemyCanAttack = false;
			bPlayerCanAttack = false;
			StartPart1();
			bPlayAgain = true;

			MuteLeads();
		}
		else if ((Enemy != nullptr) && (Enemy->GetHealth1() > 0) && (!bEnemyCanAttack) && bPlayAgain)
		{
			StartPart1();
			bPlayAgain = false;
			bPlayerCanAttack = true;

			MuteLeads();
		}
		else if ((Enemy != nullptr) && (Enemy->GetHealth1() > 0) && (!bEnemyCanAttack) && !bPlayAgain)
		{
			StartPart1();
			bEnemyCanAttack = true;
			bPlayAgain = false;
			bPlayerCanAttack = true;

			UnmuteLeads();
		}
		else
		{
			StartPart2Intro();
		}
	}
	else if (PartNameFix.Equals(TEXT("Part2IntroEnd")))
	{
		StartPart2();
		bEnemyCanAttack = true;
		bPlayAgain = true;

		UnmuteLeads();
	}
	else if (PartNameFix.Equals(TEXT("Part2End")))
	{
		// Check if loop needed -> Check if Enemy no life
		if ((Enemy != nullptr) && (Enemy->GetHealth2() > 0) && (bEnemyCanAttack))
		{
			bEnemyCanAttack = false;
			bPlayerCanAttack = false;
			StartPart2();
			bPlayAgain = true;

			MuteLeads();
		}
		else if ((Enemy != nullptr) && (Enemy->GetHealth2() > 0) && (!bEnemyCanAttack) && bPlayAgain)
		{
			StartPart2();
			bPlayAgain = false;
			bPlayerCanAttack = true;

			MuteLeads();
		}
		else if ((Enemy != nullptr) && (Enemy->GetHealth2() > 0) && (!bEnemyCanAttack) && !bPlayAgain)
		{
			StartPart2();
			bEnemyCanAttack = true;
			bPlayAgain = false;
			bPlayerCanAttack = true;

			UnmuteLeads();
		}
		else
		{
			StartPart3Intro();
		}
	}
	else if (PartNameFix.Equals(TEXT("Part3IntroEnd")))
	{
		StartPart3();
		bEnemyCanAttack = true;
		bPlayAgain = true;

		UnmuteLeads();
	}
	else if (PartNameFix.Equals(TEXT("Part3End")))
	{
		// Check if loop needed -> Check if Enemy no life
		if ((Enemy != nullptr) && (Enemy->GetHealth3() > 0) && (bEnemyCanAttack))
		{
			bEnemyCanAttack = false;
			bPlayerCanAttack = false;
			StartPart3();
			bPlayAgain = true;

			MuteLeads();
		}
		else if ((Enemy != nullptr) && (Enemy->GetHealth3() > 0) && (!bEnemyCanAttack) && bPlayAgain)
		{
			StartPart3();
			bPlayAgain = false;
			bPlayerCanAttack = true;

			MuteLeads();
		}
		else if ((Enemy != nullptr) && (Enemy->GetHealth3() > 0) && (!bEnemyCanAttack) && !bPlayAgain)
		{
			StartPart3();
			bEnemyCanAttack = true;
			bPlayAgain = false;
			bPlayerCanAttack = true;

			UnmuteLeads();
		}
		else if ((Enemy != nullptr) && (Enemy->GetHealth3() <= 0) && PartFinish == EPartFinish::Three)
		{
			StartOutro();
		}
		else
		{
			StartPart4Intro();
		}
	}
	else if (PartNameFix.Equals("Part4IntroEnd"))
	{
		StartPart4();
		bEnemyCanAttack = true;
		bPlayAgain = true;

		UnmuteLeads();
	}
	else if (PartNameFix.Equals("Part4End"))
	{
		// Check if loop needed -> Check if Enemy no life
		if ((Enemy != nullptr) && (Enemy->GetHealth4() > 0) && (bEnemyCanAttack))
		{
			bEnemyCanAttack = false;
			bPlayerCanAttack = false;
			StartPart4();
			bPlayAgain = true;

			MuteLeads();
		}
		else if ((Enemy != nullptr) && (Enemy->GetHealth4() > 0) && (!bEnemyCanAttack) && bPlayAgain)
		{
			StartPart4();
			bPlayAgain = false;
			bPlayerCanAttack = true;

			MuteLeads();
		}
		else if ((Enemy != nullptr) && (Enemy->GetHealth4() > 0) && (!bEnemyCanAttack) && !bPlayAgain)
		{
			StartPart4();
			bEnemyCanAttack = true;
			bPlayAgain = false;
			bPlayerCanAttack = true;

			UnmuteLeads();
		}
		else if ((Enemy != nullptr) && (Enemy->GetHealth4()) <= 0 && PartFinish == EPartFinish::Four)
		{
			StartOutro();
		}
		else
		{
			StartPart5Intro();
		}
	}
	else if (PartNameFix.Equals("Part5IntroEnd"))
	{
		StartPart5();
		bEnemyCanAttack = true;
		bPlayAgain = true;

		UnmuteLeads();
	}
	else if (PartNameFix.Equals("Part5End"))
	{
		// Check if loop needed -> Check if Enemy no life
		if ((Enemy != nullptr) && (Enemy->GetHealth5() > 0) && (bEnemyCanAttack))
		{
			bEnemyCanAttack = false;
			bPlayerCanAttack = false;
			StartPart5();
			bPlayAgain = true;

			MuteLeads();
		}
		else if ((Enemy != nullptr) && (Enemy->GetHealth5() > 0) && (!bEnemyCanAttack) && bPlayAgain)
		{
			StartPart5();
			bPlayAgain = false;
			bPlayerCanAttack = true;

			MuteLeads();
		}
		else if ((Enemy != nullptr) && (Enemy->GetHealth5() > 0) && (!bEnemyCanAttack) && !bPlayAgain)
		{
			StartPart5();
			bEnemyCanAttack = true;
			bPlayAgain = false;
			bPlayerCanAttack = true;

			UnmuteLeads();
		}
		else if (Enemy != nullptr && Enemy->GetHealth5() <= 0 && PartFinish == EPartFinish::Five)
		{
			StartOutro();
		}
	}
	else if (PartNameFix.Equals(TEXT("OutroEnd")))
	{
		UKismetSystemLibrary::PrintString(this, FString(TEXT("All parts finished")), true, true, FLinearColor::Blue, 5.0f);
		StopClock();
		// TODO: Do sth else
		GameHUD->GetDeathWidgetInstance()->SetVisibility(ESlateVisibility::Visible);
		AGameController* PlayerController = Cast<AGameController>(GEngine->GetFirstLocalPlayerController(GetWorld()));
		PlayerController->SetControllerState(EControllerStateGame::DeathMenu);
		// Toggle pause state
		bool bCurrentlyPaused = UGameplayStatics::IsGamePaused(GetWorld());
		UGameplayStatics::SetGamePaused(GetWorld(), !bCurrentlyPaused);

		if (bCurrentlyPaused == false)
		{
			if (GameHUD != nullptr)
			{
				GameHUD->GetPauseMenuInstance()->SetVisibility(ESlateVisibility::Visible);
			}
			// Show mouse cursor and switch to GameAndUI input so widgets receive focus
			PlayerController->bShowMouseCursor = true;
			FInputModeGameAndUI InputMode;
			InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
			PlayerController->SetInputMode(InputMode);
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
		bPlayAgain = true;

		// Mark that Part1Intro has finished so percentage updates are allowed
		bPart1IntroEnded = true;

		UnmuteLeads();
	}
	else if (PartNameFix.Equals(TEXT("Part1End")))
	{
		// Check if loop needed -> Check if Enemy no life
		if ((Enemy != nullptr) && (Enemy->GetHealth1() > 0) && (bEnemyCanAttack))
		{
			bEnemyCanAttack = false;
			bPlayerCanAttack = false;
			StartPart1();
			bPlayAgain = true;

			MuteLeads();
		}
		else if ((Enemy != nullptr) && (Enemy->GetHealth1() > 0) && (!bEnemyCanAttack) && bPlayAgain)
		{
			StartPart1();
			bPlayAgain = false;
			bPlayerCanAttack = true;

			MuteLeads();
		}
		else if ((Enemy != nullptr) && (Enemy->GetHealth1() > 0) && (!bEnemyCanAttack) && !bPlayAgain)
		{
			StartPart1();
			bEnemyCanAttack = true;
			bPlayAgain = false;
			bPlayerCanAttack = true;

			UnmuteLeads();
		}
		else
		{
			const FLatentActionInfo Info;
			UGameplayStatics::LoadStreamLevel(GetWorld(), FName(TEXT("Lvl_Swamp_Endless")), true, true, Info);
			UGameplayStatics::UnloadStreamLevel(GetWorld(), FName(TEXT("Lvl_Swamp_Begin")), Info, true);
			StartPart2Intro();
		}
	}
	else if (PartNameFix.Equals(TEXT("Part2IntroEnd")))
	{
		StartPart2();
		bEnemyCanAttack = true;
		bPlayAgain = true;

		UnmuteLeads();
	}
	else if (PartNameFix.Equals(TEXT("Part2End")))
	{
		// Check if loop needed -> Check if Enemy no life
		if ((Enemy != nullptr) && (Enemy->GetHealth2() > 0) && (bEnemyCanAttack))
		{
			bEnemyCanAttack = false;
			bPlayerCanAttack = false;
			StartPart2();
			bPlayAgain = true;

			MuteLeads();
		}
		else if ((Enemy != nullptr) && (Enemy->GetHealth2() > 0) && (!bEnemyCanAttack) && bPlayAgain)
		{
			StartPart2();
			bPlayAgain = false;
			bPlayerCanAttack = true;

			MuteLeads();
		}
		else if ((Enemy != nullptr) && (Enemy->GetHealth2() > 0) && (!bEnemyCanAttack) && !bPlayAgain)
		{
			StartPart2();
			bEnemyCanAttack = true;
			bPlayAgain = false;
			bPlayerCanAttack = true;

			UnmuteLeads();
		}
		else
		{
			StartPart3Intro();
		}
	}
	else if (PartNameFix.Equals(TEXT("Part3IntroEnd")))
	{
		StartPart3();
		bEnemyCanAttack = true;
		bPlayAgain = true;

		UnmuteLeads();
	}
	else if (PartNameFix.Equals(TEXT("Part3End")))
	{
		// Check if loop needed -> Check if Enemy no life
		if ((Enemy != nullptr) && (Enemy->GetHealth3() > 0) && (bEnemyCanAttack))
		{
			bEnemyCanAttack = false;
			bPlayerCanAttack = false;
			StartPart3();
			bPlayAgain = true;

			MuteLeads();
		}
		else if ((Enemy != nullptr) && (Enemy->GetHealth3() > 0) && (!bEnemyCanAttack) && bPlayAgain)
		{
			StartPart3();
			bPlayAgain = false;
			bPlayerCanAttack = true;

			MuteLeads();
		}
		else if ((Enemy != nullptr) && (Enemy->GetHealth3() > 0) && (!bEnemyCanAttack) && !bPlayAgain)
		{
			StartPart3();
			bEnemyCanAttack = true;
			bPlayAgain = false;
			bPlayerCanAttack = true;

			UnmuteLeads();
		}
		else if ((Enemy != nullptr) && (Enemy->GetHealth3() <= 0))
		{
			StartOutro();
		}
		else if (PartNameFix.Equals(TEXT("OutroEnd")))
		{
			UKismetSystemLibrary::PrintString(this, FString(TEXT("All parts finished")), true, true, FLinearColor::Blue, 5.0f);
			StopClock();
			// TODO: Do sth else
			GameHUD->GetDeathWidgetInstance()->SetVisibility(ESlateVisibility::Visible);
			AGameController* PlayerController = Cast<AGameController>(GEngine->GetFirstLocalPlayerController(GetWorld()));
			PlayerController->SetControllerState(EControllerStateGame::DeathMenu);
			// Toggle pause state
			bool bCurrentlyPaused = UGameplayStatics::IsGamePaused(GetWorld());
			UGameplayStatics::SetGamePaused(GetWorld(), !bCurrentlyPaused);

			if (bCurrentlyPaused == false)
			{
				if (GameHUD != nullptr)
				{
					GameHUD->GetPauseMenuInstance()->SetVisibility(ESlateVisibility::Visible);
				}
				// Show mouse cursor and switch to GameAndUI input so widgets receive focus
				PlayerController->bShowMouseCursor = true;
				FInputModeGameAndUI InputMode;
				InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
				PlayerController->SetInputMode(InputMode);
			}
		}
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
		bPlayAgain = true;

		// Mark that Part1Intro has finished so percentage updates are allowed
		bPart1IntroEnded = true;

		UnmuteLeads();
	}
	else if (PartNameFix.Equals(TEXT("Part1End")))
	{
		// Check if loop needed -> Check if Enemy no life
		if ((Enemy != nullptr) && (Enemy->GetHealth1() > 0) && (bEnemyCanAttack))
		{
			bEnemyCanAttack = false;
			bPlayerCanAttack = false;
			StartPart1();
			bPlayAgain = true;

			MuteLeads();
		}
		else if ((Enemy != nullptr) && (Enemy->GetHealth1() > 0) && (!bEnemyCanAttack) && bPlayAgain)
		{
			StartPart1();
			bPlayAgain = false;
			bPlayerCanAttack = true;

			MuteLeads();
		}
		else if ((Enemy != nullptr) && (Enemy->GetHealth1() > 0) && (!bEnemyCanAttack) && !bPlayAgain)
		{
			StartPart1();
			bEnemyCanAttack = true;
			bPlayAgain = false;
			bPlayerCanAttack = true;

			UnmuteLeads();
		}
		else
		{
			StartPart2Intro();
		}
	}
	else if (PartNameFix.Equals(TEXT("Part2IntroEnd")))
	{
		StartPart2();
		bEnemyCanAttack = true;
		bPlayAgain = true;

		UnmuteLeads();
	}
	else if (PartNameFix.Equals(TEXT("Part2End")))
	{
		// Check if loop needed -> Check if Enemy no life
		if ((Enemy != nullptr) && (Enemy->GetHealth2() > 0) && (bEnemyCanAttack))
		{
			bEnemyCanAttack = false;
			bPlayerCanAttack = false;
			StartPart2();
			bPlayAgain = true;

			MuteLeads();
		}
		else if ((Enemy != nullptr) && (Enemy->GetHealth2() > 0) && (!bEnemyCanAttack) && bPlayAgain)
		{
			StartPart2();
			bPlayAgain = false;
			bPlayerCanAttack = true;

			MuteLeads();
		}
		else if ((Enemy != nullptr) && (Enemy->GetHealth2() > 0) && (!bEnemyCanAttack) && !bPlayAgain)
		{
			StartPart2();
			bEnemyCanAttack = true;
			bPlayAgain = false;
			bPlayerCanAttack = true;

			UnmuteLeads();
		}
		else
		{
			StartPart3Intro();
		}
	}
	else if (PartNameFix.Equals(TEXT("Part3IntroEnd")))
	{
		StartPart3();
		bEnemyCanAttack = true;
		bPlayAgain = true;

		UnmuteLeads();
	}
	else if (PartNameFix.Equals(TEXT("Part3End")))
	{
		// Check if loop needed -> Check if Enemy no life
		if ((Enemy != nullptr) && (Enemy->GetHealth3() > 0) && (bEnemyCanAttack))
		{
			bEnemyCanAttack = false;
			bPlayerCanAttack = false;
			StartPart3();
			bPlayAgain = true;

			MuteLeads();
		}
		else if ((Enemy != nullptr) && (Enemy->GetHealth3() > 0) && (!bEnemyCanAttack) && bPlayAgain)
		{
			StartPart3();
			bPlayAgain = false;
			bPlayerCanAttack = true;

			MuteLeads();
		}
		else if ((Enemy != nullptr) && (Enemy->GetHealth3() > 0) && (!bEnemyCanAttack) && !bPlayAgain)
		{
			StartPart3();
			bEnemyCanAttack = true;
			bPlayAgain = false;
			bPlayerCanAttack = true;

			UnmuteLeads();
		}
		else if ((Enemy != nullptr) && (Enemy->GetHealth3() <= 0) && PartFinish == EPartFinish::Three)
		{
			StartOutro();
		}
		else
		{
			StartPart4Intro();
		}
	}
	else if (PartNameFix.Equals("Part4IntroEnd"))
	{
		StartPart4();
		bEnemyCanAttack = true;
		bPlayAgain = true;

		UnmuteLeads();
	}
	else if (PartNameFix.Equals("Part4End"))
	{
		// Check if loop needed -> Check if Enemy no life
		if ((Enemy != nullptr) && (Enemy->GetHealth4() > 0) && (bEnemyCanAttack))
		{
			bEnemyCanAttack = false;
			bPlayerCanAttack = false;
			StartPart4();
			bPlayAgain = true;

			MuteLeads();
		}
		else if ((Enemy != nullptr) && (Enemy->GetHealth4() > 0) && (!bEnemyCanAttack) && bPlayAgain)
		{
			StartPart4();
			bPlayAgain = false;
			bPlayerCanAttack = true;

			MuteLeads();
		}
		else if ((Enemy != nullptr) && (Enemy->GetHealth4() > 0) && (!bEnemyCanAttack) && !bPlayAgain)
		{
			StartPart4();
			bEnemyCanAttack = true;
			bPlayAgain = false;
			bPlayerCanAttack = true;

			UnmuteLeads();
		}
		else if ((Enemy != nullptr) && (Enemy->GetHealth4()) <= 0 && PartFinish == EPartFinish::Four)
		{
			StartOutro();
		}
		else
		{
			StartPart5Intro();
		}
	}
	else if (PartNameFix.Equals("Part5IntroEnd"))
	{
		StartPart5();
		bEnemyCanAttack = true;
		bPlayAgain = true;

		UnmuteLeads();
	}
	else if (PartNameFix.Equals("Part5End"))
	{
		// Check if loop needed -> Check if Enemy no life
		if ((Enemy != nullptr) && (Enemy->GetHealth5() > 0) && (bEnemyCanAttack))
		{
			bEnemyCanAttack = false;
			bPlayerCanAttack = false;
			StartPart5();
			bPlayAgain = true;

			MuteLeads();
		}
		else if ((Enemy != nullptr) && (Enemy->GetHealth5() > 0) && (!bEnemyCanAttack) && bPlayAgain)
		{
			StartPart5();
			bPlayAgain = false;
			bPlayerCanAttack = true;

			MuteLeads();
		}
		else if ((Enemy != nullptr) && (Enemy->GetHealth5() > 0) && (!bEnemyCanAttack) && !bPlayAgain)
		{
			StartPart5();
			bEnemyCanAttack = true;
			bPlayAgain = false;
			bPlayerCanAttack = true;

			UnmuteLeads();
		}
		else if (Enemy != nullptr && Enemy->GetHealth5() <= 0 && PartFinish == EPartFinish::Five)
		{
			StartOutro();
		}
	}
	else if (PartNameFix.Equals(TEXT("OutroEnd")))
	{
		UKismetSystemLibrary::PrintString(this, FString(TEXT("All parts finished")), true, true, FLinearColor::Blue, 5.0f);
		StopClock();
		// TODO: Do sth else
		GameHUD->GetDeathWidgetInstance()->SetVisibility(ESlateVisibility::Visible);
		AGameController* PlayerController = Cast<AGameController>(GEngine->GetFirstLocalPlayerController(GetWorld()));
		PlayerController->SetControllerState(EControllerStateGame::DeathMenu);
		// Toggle pause state
		bool bCurrentlyPaused = UGameplayStatics::IsGamePaused(GetWorld());
		UGameplayStatics::SetGamePaused(GetWorld(), !bCurrentlyPaused);

		if (bCurrentlyPaused == false)
		{
			if (GameHUD != nullptr)
			{
				GameHUD->GetPauseMenuInstance()->SetVisibility(ESlateVisibility::Visible);
			}
			// Show mouse cursor and switch to GameAndUI input so widgets receive focus
			PlayerController->bShowMouseCursor = true;
			FInputModeGameAndUI InputMode;
			InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
			PlayerController->SetInputMode(InputMode);
		}
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
		bPlayAgain = true;

		// Mark that Part1Intro has finished so percentage updates are allowed
		bPart1IntroEnded = true;

		UnmuteLeads();
	}
	else if (PartNameFix.Equals(TEXT("Part1End")))
	{
		// Check if loop needed -> Check if Enemy no life
		if ((Enemy != nullptr) && (Enemy->GetHealth1() > 0) && (bEnemyCanAttack))
		{
			bEnemyCanAttack = false;
			bPlayerCanAttack = false;
			StartPart1();
			bPlayAgain = true;

			MuteLeads();
		}
		else if ((Enemy != nullptr) && (Enemy->GetHealth1() > 0) && (!bEnemyCanAttack) && bPlayAgain)
		{
			StartPart1();
			bPlayAgain = false;
			bPlayerCanAttack = true;

			MuteLeads();
		}
		else if ((Enemy != nullptr) && (Enemy->GetHealth1() > 0) && (!bEnemyCanAttack) && !bPlayAgain)
		{
			StartPart1();
			bEnemyCanAttack = true;
			bPlayAgain = false;
			bPlayerCanAttack = true;

			UnmuteLeads();
		}
		else
		{
			StartPart2Intro();
		}
	}
	else if (PartNameFix.Equals(TEXT("Part2IntroEnd")))
	{
		StartPart2();
		bEnemyCanAttack = true;
		bPlayAgain = true;

		UnmuteLeads();
	}
	else if (PartNameFix.Equals(TEXT("Part2End")))
	{
		// Check if loop needed -> Check if Enemy no life
		if ((Enemy != nullptr) && (Enemy->GetHealth2() > 0) && (bEnemyCanAttack))
		{
			bEnemyCanAttack = false;
			bPlayerCanAttack = false;
			StartPart2();
			bPlayAgain = true;

			MuteLeads();
		}
		else if ((Enemy != nullptr) && (Enemy->GetHealth2() > 0) && (!bEnemyCanAttack) && bPlayAgain)
		{
			StartPart2();
			bPlayAgain = false;
			bPlayerCanAttack = true;

			MuteLeads();
		}
		else if ((Enemy != nullptr) && (Enemy->GetHealth2() > 0) && (!bEnemyCanAttack) && !bPlayAgain)
		{
			StartPart2();
			bEnemyCanAttack = true;
			bPlayAgain = false;
			bPlayerCanAttack = true;

			UnmuteLeads();
		}
		else
		{
			StartPart3Intro();
		}
	}
	else if (PartNameFix.Equals(TEXT("Part3IntroEnd")))
	{
		StartPart3();
		bEnemyCanAttack = true;
		bPlayAgain = true;

		UnmuteLeads();
	}
	else if (PartNameFix.Equals(TEXT("Part3End")))
	{
		// Check if loop needed -> Check if Enemy no life
		if ((Enemy != nullptr) && (Enemy->GetHealth3() > 0) && (bEnemyCanAttack))
		{
			bEnemyCanAttack = false;
			bPlayerCanAttack = false;
			StartPart3();
			bPlayAgain = true;

			MuteLeads();
		}
		else if ((Enemy != nullptr) && (Enemy->GetHealth3() > 0) && (!bEnemyCanAttack) && bPlayAgain)
		{
			StartPart3();
			bPlayAgain = false;
			bPlayerCanAttack = true;

			MuteLeads();
		}
		else if ((Enemy != nullptr) && (Enemy->GetHealth3() > 0) && (!bEnemyCanAttack) && !bPlayAgain)
		{
			StartPart3();
			bEnemyCanAttack = true;
			bPlayAgain = false;
			bPlayerCanAttack = true;

			UnmuteLeads();
		}
		else if ((Enemy != nullptr) && (Enemy->GetHealth3() <= 0) && PartFinish == EPartFinish::Three)
		{
			StartOutro();
		}
		else
		{
			StartPart4Intro();
		}
	}
	else if (PartNameFix.Equals("Part4IntroEnd"))
	{
		StartPart4();
		bEnemyCanAttack = true;
		bPlayAgain = true;

		UnmuteLeads();
	}
	else if (PartNameFix.Equals("Part4End"))
	{
		// Check if loop needed -> Check if Enemy no life
		if ((Enemy != nullptr) && (Enemy->GetHealth4() > 0) && (bEnemyCanAttack))
		{
			bEnemyCanAttack = false;
			bPlayerCanAttack = false;
			StartPart4();
			bPlayAgain = true;

			MuteLeads();
		}
		else if ((Enemy != nullptr) && (Enemy->GetHealth4() > 0) && (!bEnemyCanAttack) && bPlayAgain)
		{
			StartPart4();
			bPlayAgain = false;
			bPlayerCanAttack = true;

			MuteLeads();
		}
		else if ((Enemy != nullptr) && (Enemy->GetHealth4() > 0) && (!bEnemyCanAttack) && !bPlayAgain)
		{
			StartPart4();
			bEnemyCanAttack = true;
			bPlayAgain = false;
			bPlayerCanAttack = true;

			UnmuteLeads();
		}
		else if ((Enemy != nullptr) && (Enemy->GetHealth4()) <= 0 && PartFinish == EPartFinish::Four)
		{
			StartOutro();
		}
	}
	else if (PartNameFix.Equals(TEXT("OutroEnd")))
	{
		UKismetSystemLibrary::PrintString(this, FString(TEXT("All parts finished")), true, true, FLinearColor::Blue, 5.0f);
		StopClock();
		// TODO: Do sth else
		GameHUD->GetDeathWidgetInstance()->SetVisibility(ESlateVisibility::Visible);
		AGameController* PlayerController = Cast<AGameController>(GEngine->GetFirstLocalPlayerController(GetWorld()));
		PlayerController->SetControllerState(EControllerStateGame::DeathMenu);
		// Toggle pause state
		bool bCurrentlyPaused = UGameplayStatics::IsGamePaused(GetWorld());
		UGameplayStatics::SetGamePaused(GetWorld(), !bCurrentlyPaused);

		if (bCurrentlyPaused == false)
		{
			if (GameHUD != nullptr)
			{
				GameHUD->GetPauseMenuInstance()->SetVisibility(ESlateVisibility::Visible);
			}
			// Show mouse cursor and switch to GameAndUI input so widgets receive focus
			PlayerController->bShowMouseCursor = true;
			FInputModeGameAndUI InputMode;
			InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
			PlayerController->SetInputMode(InputMode);
		}
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
		bPlayAgain = true;

		// Mark that Part1Intro has finished so percentage updates are allowed
		bPart1IntroEnded = true;

		UnmuteLeads();
	}
	else if (PartNameFix.Equals(TEXT("Part1End")))
	{
		// Check if loop needed -> Check if Enemy no life
		if ((Enemy != nullptr) && (Enemy->GetHealth1() > 0) && (bEnemyCanAttack))
		{
			bEnemyCanAttack = false;
			bPlayerCanAttack = false;
			StartPart1();
			bPlayAgain = true;

			MuteLeads();
		}
		else if ((Enemy != nullptr) && (Enemy->GetHealth1() > 0) && (!bEnemyCanAttack) && bPlayAgain)
		{
			StartPart1();
			bPlayAgain = false;
			bPlayerCanAttack = true;

			MuteLeads();
		}
		else if ((Enemy != nullptr) && (Enemy->GetHealth1() > 0) && (!bEnemyCanAttack) && !bPlayAgain)
		{
			StartPart1();
			bEnemyCanAttack = true;
			bPlayAgain = false;
			bPlayerCanAttack = true;

			UnmuteLeads();
		}
		else
		{
			StartPart2Intro();
		}
	}
	else if (PartNameFix.Equals(TEXT("Part2IntroEnd")))
	{
		StartPart2();
		bEnemyCanAttack = true;
		bPlayAgain = true;

		UnmuteLeads();
	}
	else if (PartNameFix.Equals(TEXT("Part2End")))
	{
		// Check if loop needed -> Check if Enemy no life
		if ((Enemy != nullptr) && (Enemy->GetHealth2() > 0) && (bEnemyCanAttack))
		{
			bEnemyCanAttack = false;
			bPlayerCanAttack = false;
			StartPart2();
			bPlayAgain = true;

			MuteLeads();
		}
		else if ((Enemy != nullptr) && (Enemy->GetHealth2() > 0) && (!bEnemyCanAttack) && bPlayAgain)
		{
			StartPart2();
			bPlayAgain = false;
			bPlayerCanAttack = true;

			MuteLeads();
		}
		else if ((Enemy != nullptr) && (Enemy->GetHealth2() > 0) && (!bEnemyCanAttack) && !bPlayAgain)
		{
			StartPart2();
			bEnemyCanAttack = true;
			bPlayAgain = false;
			bPlayerCanAttack = true;

			UnmuteLeads();
		}
		else
		{
			StartPart3Intro();
		}
	}
	else if (PartNameFix.Equals(TEXT("Part3IntroEnd")))
	{
		StartPart3();
		bEnemyCanAttack = true;
		bPlayAgain = true;

		UnmuteLeads();
	}
	else if (PartNameFix.Equals(TEXT("Part3End")))
	{
		// Check if loop needed -> Check if Enemy no life
		if ((Enemy != nullptr) && (Enemy->GetHealth3() > 0) && (bEnemyCanAttack))
		{
			bEnemyCanAttack = false;
			bPlayerCanAttack = false;
			StartPart3();
			bPlayAgain = true;

			MuteLeads();
		}
		else if ((Enemy != nullptr) && (Enemy->GetHealth3() > 0) && (!bEnemyCanAttack) && bPlayAgain)
		{
			StartPart3();
			bPlayAgain = false;
			bPlayerCanAttack = true;

			MuteLeads();
		}
		else if ((Enemy != nullptr) && (Enemy->GetHealth3() > 0) && (!bEnemyCanAttack) && !bPlayAgain)
		{
			StartPart3();
			bEnemyCanAttack = true;
			bPlayAgain = false;
			bPlayerCanAttack = true;

			UnmuteLeads();
		}
		else if ((Enemy != nullptr) && (Enemy->GetHealth3() <= 0) && PartFinish == EPartFinish::Three)
		{
			StartOutro();
		}
		else
		{
			StartPart4Intro();
		}
	}
	else if (PartNameFix.Equals("Part4IntroEnd"))
	{
		StartPart4();
		bEnemyCanAttack = true;
		bPlayAgain = true;

		UnmuteLeads();
	}
	else if (PartNameFix.Equals("Part4End"))
	{
		// Check if loop needed -> Check if Enemy no life
		if ((Enemy != nullptr) && (Enemy->GetHealth4() > 0) && (bEnemyCanAttack))
		{
			bEnemyCanAttack = false;
			bPlayerCanAttack = false;
			StartPart4();
			bPlayAgain = true;

			MuteLeads();
		}
		else if ((Enemy != nullptr) && (Enemy->GetHealth4() > 0) && (!bEnemyCanAttack) && bPlayAgain)
		{
			StartPart4();
			bPlayAgain = false;
			bPlayerCanAttack = true;

			MuteLeads();
		}
		else if ((Enemy != nullptr) && (Enemy->GetHealth4() > 0) && (!bEnemyCanAttack) && !bPlayAgain)
		{
			StartPart4();
			bEnemyCanAttack = true;
			bPlayAgain = false;
			bPlayerCanAttack = true;

			UnmuteLeads();
		}
		else if ((Enemy != nullptr) && (Enemy->GetHealth4()) <= 0 && PartFinish == EPartFinish::Four)
		{
			StartOutro();
		}
	}
	else if (PartNameFix.Equals(TEXT("OutroEnd")))
	{
		UKismetSystemLibrary::PrintString(this, FString(TEXT("All parts finished")), true, true, FLinearColor::Blue, 5.0f);
		StopClock();
		// TODO: Do sth else
		GameHUD->GetDeathWidgetInstance()->SetVisibility(ESlateVisibility::Visible);
		AGameController* PlayerController = Cast<AGameController>(GEngine->GetFirstLocalPlayerController(GetWorld()));
		PlayerController->SetControllerState(EControllerStateGame::DeathMenu);
		// Toggle pause state
		bool bCurrentlyPaused = UGameplayStatics::IsGamePaused(GetWorld());
		UGameplayStatics::SetGamePaused(GetWorld(), !bCurrentlyPaused);

		if (bCurrentlyPaused == false)
		{
			if (GameHUD != nullptr)
			{
				GameHUD->GetPauseMenuInstance()->SetVisibility(ESlateVisibility::Visible);
			}
			// Show mouse cursor and switch to GameAndUI input so widgets receive focus
			PlayerController->bShowMouseCursor = true;
			FInputModeGameAndUI InputMode;
			InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
			PlayerController->SetInputMode(InputMode);
		}
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
		bPlayAgain = true;

		// Mark that Part1Intro has finished so percentage updates are allowed
		bPart1IntroEnded = true;

		UnmuteLeads();
	}
	else if (PartNameFix.Equals(TEXT("Part1End")))
	{
		// Check if loop needed -> Check if Enemy no life
		if ((Enemy != nullptr) && (Enemy->GetHealth1() > 0) && (bEnemyCanAttack))
		{
			bEnemyCanAttack = false;
			bPlayerCanAttack = false;
			StartPart1();
			bPlayAgain = true;

			MuteLeads();
		}
		else if ((Enemy != nullptr) && (Enemy->GetHealth1() > 0) && (!bEnemyCanAttack) && bPlayAgain)
		{
			StartPart1();
			bPlayAgain = false;
			bPlayerCanAttack = true;

			MuteLeads();
		}
		else if ((Enemy != nullptr) && (Enemy->GetHealth1() > 0) && (!bEnemyCanAttack) && !bPlayAgain)
		{
			StartPart1();
			bEnemyCanAttack = true;
			bPlayAgain = false;
			bPlayerCanAttack = true;

			UnmuteLeads();
		}
		else
		{
			StartPart2Intro();
		}
	}
	else if (PartNameFix.Equals(TEXT("Part2IntroEnd")))
	{
		StartPart2();
		bEnemyCanAttack = true;
		bPlayAgain = true;

		UnmuteLeads();
	}
	else if (PartNameFix.Equals(TEXT("Part2End")))
	{
		// Check if loop needed -> Check if Enemy no life
		if ((Enemy != nullptr) && (Enemy->GetHealth2() > 0) && (bEnemyCanAttack))
		{
			bEnemyCanAttack = false;
			bPlayerCanAttack = false;
			StartPart2();
			bPlayAgain = true;

			MuteLeads();
		}
		else if ((Enemy != nullptr) && (Enemy->GetHealth2() > 0) && (!bEnemyCanAttack) && bPlayAgain)
		{
			StartPart2();
			bPlayAgain = false;
			bPlayerCanAttack = true;

			MuteLeads();
		}
		else if ((Enemy != nullptr) && (Enemy->GetHealth2() > 0) && (!bEnemyCanAttack) && !bPlayAgain)
		{
			StartPart2();
			bEnemyCanAttack = true;
			bPlayAgain = false;
			bPlayerCanAttack = true;

			UnmuteLeads();
		}
		else
		{
			StartPart3Intro();
		}
	}
	else if (PartNameFix.Equals(TEXT("Part3IntroEnd")))
	{
		StartPart3();
		bEnemyCanAttack = true;
		bPlayAgain = true;

		UnmuteLeads();
	}
	else if (PartNameFix.Equals(TEXT("Part3End")))
	{
		// Check if loop needed -> Check if Enemy no life
		if ((Enemy != nullptr) && (Enemy->GetHealth3() > 0) && (bEnemyCanAttack))
		{
			bEnemyCanAttack = false;
			bPlayerCanAttack = false;
			StartPart3();
			bPlayAgain = true;

			MuteLeads();
		}
		else if ((Enemy != nullptr) && (Enemy->GetHealth3() > 0) && (!bEnemyCanAttack) && bPlayAgain)
		{
			StartPart3();
			bPlayAgain = false;
			bPlayerCanAttack = true;

			MuteLeads();
		}
		else if ((Enemy != nullptr) && (Enemy->GetHealth3() > 0) && (!bEnemyCanAttack) && !bPlayAgain)
		{
			StartPart3();
			bEnemyCanAttack = true;
			bPlayAgain = false;
			bPlayerCanAttack = true;

			UnmuteLeads();
		}
		else if ((Enemy != nullptr) && (Enemy->GetHealth3() <= 0) && PartFinish == EPartFinish::Three)
		{
			StartOutro();
		}
		else
		{
			StartPart4Intro();
		}
	}
	else if (PartNameFix.Equals("Part4IntroEnd"))
	{
		StartPart4();
		bEnemyCanAttack = true;
		bPlayAgain = true;

		UnmuteLeads();
	}
	else if (PartNameFix.Equals("Part4End"))
	{
		// Check if loop needed -> Check if Enemy no life
		if ((Enemy != nullptr) && (Enemy->GetHealth4() > 0) && (bEnemyCanAttack))
		{
			bEnemyCanAttack = false;
			bPlayerCanAttack = false;
			StartPart4();
			bPlayAgain = true;

			MuteLeads();
		}
		else if ((Enemy != nullptr) && (Enemy->GetHealth4() > 0) && (!bEnemyCanAttack) && bPlayAgain)
		{
			StartPart4();
			bPlayAgain = false;
			bPlayerCanAttack = true;

			MuteLeads();
		}
		else if ((Enemy != nullptr) && (Enemy->GetHealth4() > 0) && (!bEnemyCanAttack) && !bPlayAgain)
		{
			StartPart4();
			bEnemyCanAttack = true;
			bPlayAgain = false;
			bPlayerCanAttack = true;

			UnmuteLeads();
		}
		else if ((Enemy != nullptr) && (Enemy->GetHealth4()) <= 0 && PartFinish == EPartFinish::Four)
		{
			StartOutro();
		}
	}
	else if (PartNameFix.Equals(TEXT("OutroEnd")))
	{
		UKismetSystemLibrary::PrintString(this, FString(TEXT("All parts finished")), true, true, FLinearColor::Blue, 5.0f);
		StopClock();
		// TODO: Do sth else
		GameHUD->GetDeathWidgetInstance()->SetVisibility(ESlateVisibility::Visible);
		AGameController* PlayerController = Cast<AGameController>(GEngine->GetFirstLocalPlayerController(GetWorld()));
		PlayerController->SetControllerState(EControllerStateGame::DeathMenu);
		// Toggle pause state
		bool bCurrentlyPaused = UGameplayStatics::IsGamePaused(GetWorld());
		UGameplayStatics::SetGamePaused(GetWorld(), !bCurrentlyPaused);

		if (bCurrentlyPaused == false)
		{
			if (GameHUD != nullptr)
			{
				GameHUD->GetPauseMenuInstance()->SetVisibility(ESlateVisibility::Visible);
			}
			// Show mouse cursor and switch to GameAndUI input so widgets receive focus
			PlayerController->bShowMouseCursor = true;
			FInputModeGameAndUI InputMode;
			InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
			PlayerController->SetInputMode(InputMode);
		}
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
		bPlayAgain = true;

		// Mark that Part1Intro has finished so percentage updates are allowed
		bPart1IntroEnded = true;

		UnmuteLeads();
	}
	else if (PartNameFix.Equals(TEXT("Part1End")))
	{
		// Check if loop needed -> Check if Enemy no life
		if ((Enemy != nullptr) && (Enemy->GetHealth1() > 0) && (bEnemyCanAttack))
		{
			bEnemyCanAttack = false;
			bPlayerCanAttack = false;
			StartPart1();
			bPlayAgain = true;

			MuteLeads();
		}
		else if ((Enemy != nullptr) && (Enemy->GetHealth1() > 0) && (!bEnemyCanAttack) && bPlayAgain)
		{
			StartPart1();
			bPlayAgain = false;
			bPlayerCanAttack = true;

			MuteLeads();
		}
		else if ((Enemy != nullptr) && (Enemy->GetHealth1() > 0) && (!bEnemyCanAttack) && !bPlayAgain)
		{
			StartPart1();
			bEnemyCanAttack = true;
			bPlayAgain = false;
			bPlayerCanAttack = true;

			UnmuteLeads();
		}
		else
		{
			StartPart2Intro();
		}
	}
	else if (PartNameFix.Equals(TEXT("Part2IntroEnd")))
	{
		StartPart2();
		bEnemyCanAttack = true;
		bPlayAgain = true;

		UnmuteLeads();
	}
	else if (PartNameFix.Equals(TEXT("Part2End")))
	{
		// Check if loop needed -> Check if Enemy no life
		if ((Enemy != nullptr) && (Enemy->GetHealth2() > 0) && (bEnemyCanAttack))
		{
			bEnemyCanAttack = false;
			bPlayerCanAttack = false;
			StartPart2();
			bPlayAgain = true;

			MuteLeads();
		}
		else if ((Enemy != nullptr) && (Enemy->GetHealth2() > 0) && (!bEnemyCanAttack) && bPlayAgain)
		{
			StartPart2();
			bPlayAgain = false;
			bPlayerCanAttack = true;

			MuteLeads();
		}
		else if ((Enemy != nullptr) && (Enemy->GetHealth2() > 0) && (!bEnemyCanAttack) && !bPlayAgain)
		{
			StartPart2();
			bEnemyCanAttack = true;
			bPlayAgain = false;
			bPlayerCanAttack = true;

			UnmuteLeads();
		}
		else
		{
			StartPart3Intro();
		}
	}
	else if (PartNameFix.Equals(TEXT("Part3IntroEnd")))
	{
		StartPart3();
		bEnemyCanAttack = true;
		bPlayAgain = true;

		UnmuteLeads();
	}
	else if (PartNameFix.Equals(TEXT("Part3End")))
	{
		// Check if loop needed -> Check if Enemy no life
		if ((Enemy != nullptr) && (Enemy->GetHealth3() > 0) && (bEnemyCanAttack))
		{
			bEnemyCanAttack = false;
			bPlayerCanAttack = false;
			StartPart3();
			bPlayAgain = true;

			MuteLeads();
		}
		else if ((Enemy != nullptr) && (Enemy->GetHealth3() > 0) && (!bEnemyCanAttack) && bPlayAgain)
		{
			StartPart3();
			bPlayAgain = false;
			bPlayerCanAttack = true;

			MuteLeads();
		}
		else if ((Enemy != nullptr) && (Enemy->GetHealth3() > 0) && (!bEnemyCanAttack) && !bPlayAgain)
		{
			StartPart3();
			bEnemyCanAttack = true;
			bPlayAgain = false;
			bPlayerCanAttack = true;

			UnmuteLeads();
		}
		else if ((Enemy != nullptr) && (Enemy->GetHealth3() <= 0) && PartFinish == EPartFinish::Three)
		{
			StartOutro();
		}
		else
		{
			StartPart4Intro();
		}
	}
	else if (PartNameFix.Equals("Part4IntroEnd"))
	{
		StartPart4();
		bEnemyCanAttack = true;
		bPlayAgain = true;

		UnmuteLeads();
	}
	else if (PartNameFix.Equals("Part4End"))
	{
		// Check if loop needed -> Check if Enemy no life
		if ((Enemy != nullptr) && (Enemy->GetHealth4() > 0) && (bEnemyCanAttack))
		{
			bEnemyCanAttack = false;
			bPlayerCanAttack = false;
			StartPart4();
			bPlayAgain = true;

			MuteLeads();
		}
		else if ((Enemy != nullptr) && (Enemy->GetHealth4() > 0) && (!bEnemyCanAttack) && bPlayAgain)
		{
			StartPart4();
			bPlayAgain = false;
			bPlayerCanAttack = true;

			MuteLeads();
		}
		else if ((Enemy != nullptr) && (Enemy->GetHealth4() > 0) && (!bEnemyCanAttack) && !bPlayAgain)
		{
			StartPart4();
			bEnemyCanAttack = true;
			bPlayAgain = false;
			bPlayerCanAttack = true;

			UnmuteLeads();
		}
		else if ((Enemy != nullptr) && (Enemy->GetHealth4()) <= 0 && PartFinish == EPartFinish::Four)
		{
			StartOutro();
		}
	}
	else if (PartNameFix.Equals(TEXT("OutroEnd")))
	{
		UKismetSystemLibrary::PrintString(this, FString(TEXT("All parts finished")), true, true, FLinearColor::Blue, 5.0f);
		StopClock();
		// TODO: Do sth else
		GameHUD->GetDeathWidgetInstance()->SetVisibility(ESlateVisibility::Visible);
		AGameController* PlayerController = Cast<AGameController>(GEngine->GetFirstLocalPlayerController(GetWorld()));
		PlayerController->SetControllerState(EControllerStateGame::DeathMenu);
		// Toggle pause state
		bool bCurrentlyPaused = UGameplayStatics::IsGamePaused(GetWorld());
		UGameplayStatics::SetGamePaused(GetWorld(), !bCurrentlyPaused);

		if (bCurrentlyPaused == false)
		{
			if (GameHUD != nullptr)
			{
				GameHUD->GetPauseMenuInstance()->SetVisibility(ESlateVisibility::Visible);
			}
			// Show mouse cursor and switch to GameAndUI input so widgets receive focus
			PlayerController->bShowMouseCursor = true;
			FInputModeGameAndUI InputMode;
			InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
			PlayerController->SetInputMode(InputMode);
		}
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
		bPlayAgain = true;

		// Mark that Part1Intro has finished so percentage updates are allowed
		bPart1IntroEnded = true;

		UnmuteLeads();
	}
	else if (PartNameFix.Equals(TEXT("Part1End")))
	{
		// Check if loop needed -> Check if Enemy no life
		if ((Enemy != nullptr) && (Enemy->GetHealth1() > 0) && (bEnemyCanAttack))
		{
			bEnemyCanAttack = false;
			bPlayerCanAttack = false;
			StartPart1();
			bPlayAgain = true;

			MuteLeads();
		}
		else if ((Enemy != nullptr) && (Enemy->GetHealth1() > 0) && (!bEnemyCanAttack) && bPlayAgain)
		{
			StartPart1();
			bPlayAgain = false;
			bPlayerCanAttack = true;

			MuteLeads();
		}
		else if ((Enemy != nullptr) && (Enemy->GetHealth1() > 0) && (!bEnemyCanAttack) && !bPlayAgain)
		{
			StartPart1();
			bEnemyCanAttack = true;
			bPlayAgain = false;
			bPlayerCanAttack = true;

			UnmuteLeads();
		}
		else
		{
			StartPart2Intro();
		}
	}
	else if (PartNameFix.Equals(TEXT("Part2IntroEnd")))
	{
		StartPart2();
		bEnemyCanAttack = true;
		bPlayAgain = true;

		UnmuteLeads();
	}
	else if (PartNameFix.Equals(TEXT("Part2End")))
	{
		// Check if loop needed -> Check if Enemy no life
		if ((Enemy != nullptr) && (Enemy->GetHealth2() > 0) && (bEnemyCanAttack))
		{
			bEnemyCanAttack = false;
			bPlayerCanAttack = false;
			StartPart2();
			bPlayAgain = true;

			MuteLeads();
		}
		else if ((Enemy != nullptr) && (Enemy->GetHealth2() > 0) && (!bEnemyCanAttack) && bPlayAgain)
		{
			StartPart2();
			bPlayAgain = false;
			bPlayerCanAttack = true;

			MuteLeads();
		}
		else if ((Enemy != nullptr) && (Enemy->GetHealth2() > 0) && (!bEnemyCanAttack) && !bPlayAgain)
		{
			StartPart2();
			bEnemyCanAttack = true;
			bPlayAgain = false;
			bPlayerCanAttack = true;

			UnmuteLeads();
		}
		else
		{
			StartPart3Intro();
		}
	}
	else if (PartNameFix.Equals(TEXT("Part3IntroEnd")))
	{
		StartPart3();
		bEnemyCanAttack = true;
		bPlayAgain = true;

		UnmuteLeads();
	}
	else if (PartNameFix.Equals(TEXT("Part3End")))
	{
		// Check if loop needed -> Check if Enemy no life
		if ((Enemy != nullptr) && (Enemy->GetHealth3() > 0) && (bEnemyCanAttack))
		{
			bEnemyCanAttack = false;
			bPlayerCanAttack = false;
			StartPart3();
			bPlayAgain = true;

			MuteLeads();
		}
		else if ((Enemy != nullptr) && (Enemy->GetHealth3() > 0) && (!bEnemyCanAttack) && bPlayAgain)
		{
			StartPart3();
			bPlayAgain = false;
			bPlayerCanAttack = true;

			MuteLeads();
		}
		else if ((Enemy != nullptr) && (Enemy->GetHealth3() > 0) && (!bEnemyCanAttack) && !bPlayAgain)
		{
			StartPart3();
			bEnemyCanAttack = true;
			bPlayAgain = false;
			bPlayerCanAttack = true;

			UnmuteLeads();
		}
		else if ((Enemy != nullptr) && (Enemy->GetHealth3() <= 0) && PartFinish == EPartFinish::Three)
		{
			StartOutro();
		}
		else
		{
			StartPart4Intro();
		}
	}
	else if (PartNameFix.Equals("Part4IntroEnd"))
	{
		StartPart4();
		bEnemyCanAttack = true;
		bPlayAgain = true;

		UnmuteLeads();
	}
	else if (PartNameFix.Equals("Part4End"))
	{
		// Check if loop needed -> Check if Enemy no life
		if ((Enemy != nullptr) && (Enemy->GetHealth4() > 0) && (bEnemyCanAttack))
		{
			bEnemyCanAttack = false;
			bPlayerCanAttack = false;
			StartPart4();
			bPlayAgain = true;

			MuteLeads();
		}
		else if ((Enemy != nullptr) && (Enemy->GetHealth4() > 0) && (!bEnemyCanAttack) && bPlayAgain)
		{
			StartPart4();
			bPlayAgain = false;
			bPlayerCanAttack = true;

			MuteLeads();
		}
		else if ((Enemy != nullptr) && (Enemy->GetHealth4() > 0) && (!bEnemyCanAttack) && !bPlayAgain)
		{
			StartPart4();
			bEnemyCanAttack = true;
			bPlayAgain = false;
			bPlayerCanAttack = true;

			UnmuteLeads();
		}
		else if ((Enemy != nullptr) && (Enemy->GetHealth4()) <= 0 && PartFinish == EPartFinish::Four)
		{
			StartOutro();
		}
		else
		{
			StartPart5Intro();
		}
	}
	else if (PartNameFix.Equals("Part5IntroEnd"))
	{
		StartPart5();
		bEnemyCanAttack = true;
		bPlayAgain = true;

		UnmuteLeads();
	}
	else if (PartNameFix.Equals("Part5End"))
	{
		// Check if loop needed -> Check if Enemy no life
		if ((Enemy != nullptr) && (Enemy->GetHealth5() > 0) && (bEnemyCanAttack))
		{
			bEnemyCanAttack = false;
			bPlayerCanAttack = false;
			StartPart5();
			bPlayAgain = true;

			MuteLeads();
		}
		else if ((Enemy != nullptr) && (Enemy->GetHealth5() > 0) && (!bEnemyCanAttack) && bPlayAgain)
		{
			StartPart5();
			bPlayAgain = false;
			bPlayerCanAttack = true;

			MuteLeads();
		}
		else if ((Enemy != nullptr) && (Enemy->GetHealth5() > 0) && (!bEnemyCanAttack) && !bPlayAgain)
		{
			StartPart5();
			bEnemyCanAttack = true;
			bPlayAgain = false;
			bPlayerCanAttack = true;

			UnmuteLeads();
		}
		else if (Enemy != nullptr && Enemy->GetHealth5() <= 0 && PartFinish == EPartFinish::Five)
		{
			StartOutro();
		}
	}
	else if (PartNameFix.Equals(TEXT("OutroEnd")))
	{
		UKismetSystemLibrary::PrintString(this, FString(TEXT("All parts finished")), true, true, FLinearColor::Blue, 5.0f);
		StopClock();
		// TODO: Do sth else
		GameHUD->GetDeathWidgetInstance()->SetVisibility(ESlateVisibility::Visible);
		AGameController* PlayerController = Cast<AGameController>(GEngine->GetFirstLocalPlayerController(GetWorld()));
		PlayerController->SetControllerState(EControllerStateGame::DeathMenu);
		// Toggle pause state
		bool bCurrentlyPaused = UGameplayStatics::IsGamePaused(GetWorld());
		UGameplayStatics::SetGamePaused(GetWorld(), !bCurrentlyPaused);

		if (bCurrentlyPaused == false)
		{
			if (GameHUD != nullptr)
			{
				GameHUD->GetPauseMenuInstance()->SetVisibility(ESlateVisibility::Visible);
			}
			// Show mouse cursor and switch to GameAndUI input so widgets receive focus
			PlayerController->bShowMouseCursor = true;
			FInputModeGameAndUI InputMode;
			InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
			PlayerController->SetInputMode(InputMode);
		}
	}
}

EMapNames UAudioManagerSubsystem::GetMapTypeFromString(const FString& MapName)
{
	if (MapName.Contains(TEXT("Lvl_Swamp")))
	{
		return EMapNames::Swamp;
	}
	else if (MapName.Contains(TEXT("Lvl_Cyberpunk")))
	{
		return EMapNames::Cyberpunk;
	}
	else if (MapName.Contains(TEXT("Lvl_Ice")))
	{
		return EMapNames::Ice;
	}
	else if (MapName.Contains(TEXT("Lvl_Steampunk")))
	{
		return EMapNames::Steampunk;
	}
	else if (MapName.Contains(TEXT("Lvl_Space")))
	{
		return EMapNames::Space;
	}
	else if (MapName.Contains(TEXT("Lvl_Volcano")))
	{
		return EMapNames::Volcano;
	}
	else if (MapName.Contains(TEXT("TestMap")))
	{
		return  EMapNames::TestMap;
	}
	
	// Default fallback
	UE_LOG(LogTemp, Warning, TEXT("Unknown map name: %s"), *MapName);
	return EMapNames::TestMap;
}
