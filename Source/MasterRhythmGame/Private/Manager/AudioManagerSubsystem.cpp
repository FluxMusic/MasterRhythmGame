// Fill out your copyright notice in the Description page of Project Settings.

#include "Manager/AudioManagerSubsystem.h"
#include "Quartz/AudioMixerClockHandle.h"
#include "Quartz/QuartzSubsystem.h"
#include "Actor/NodeActor.h"
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

void UAudioManagerSubsystem::InitSubsystem()
{
	if (QuartzSubsystem == nullptr)
	{
		QuartzSubsystem = UQuartzSubsystem::Get(GetWorld());
	}
}

void UAudioManagerSubsystem::StartClock()
{
	if (ClockHandle != nullptr)
	{
		ClockHandle->StartClock(this, ClockHandle);
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
		OutputAudioComponentMap.Add(FName(TEXT("MIDINoteOut")), AudioComponent);

		FOnMetasoundOutputValueChanged PartNameDelegate;
		PartNameDelegate.BindUFunction(this, FName("WatchOutputPartFinishedName"));
		MetaSoundOutputSubsystem->WatchOutput(AudioComponent, FName(TEXT("PartFinishedName")), PartNameDelegate);
		OutputAudioComponentMap.Add(FName(TEXT("PartFinishedName")), AudioComponent);

		FOnMetasoundOutputValueChanged PartFinishedDelegate;
		PartFinishedDelegate.BindUFunction(this, FName("WatchOutputPartFinishedPart"));
		MetaSoundOutputSubsystem->WatchOutput(AudioComponent, FName(TEXT("OnPartFinished")), PartFinishedDelegate);
		OutputAudioComponentMap.Add(FName(TEXT("OnPartFinished")), AudioComponent);

		FOnMetasoundOutputValueChanged PartPercentDelegate;
		PartPercentDelegate.BindUFunction(this, FName("WatchOutputPartFinishedPercent"));
		MetaSoundOutputSubsystem->WatchOutput(AudioComponent, FName(TEXT("PartFinishPercent")), PartPercentDelegate);
		OutputAudioComponentMap.Add(FName(TEXT("PartFinishPercent")), AudioComponent);
	}
	StartMusic();
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
	//// Try to extract a float value from the MetaSound output
	//float Value = 0.0f;
	//if (Output.Get<float>(Value))
	//{
	//	const bool bNormalized = (Value >= 0.0f && Value <= 1.0f);
	//	const float DisplayValue = bNormalized ? Value * 100.0f : Value;
	//	const FString Msg = FString::Printf(TEXT("%s = %.2f%%"), *OutputName.ToString(), DisplayValue);
	//	UKismetSystemLibrary::PrintString(this, Msg, true, true, FLinearColor::Yellow, 5.0f);
	//}
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
	//UKismetSystemLibrary::PrintString(this, FString::FormatAsNumber(Beat), true, true, FLinearColor::Green, 10.0f);
}

void UAudioManagerSubsystem::WatchOutputMidiNoteChange(FName OutputName, const FMetaSoundOutput& Output)
{
	// Extract MIDI note as int32 and print it
	int32 MidiNote = -1;
	if (Output.Get<int32>(MidiNote))
	{
		if (MidiNote == 0)
		{
			return;
		}
		MidiNote = MidiNote % 12;

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

	if (Enemy != nullptr && Spline != nullptr && bEnemyCanAttack)
	{
		Enemy->SetSplineRef(Spline->GetSplines(MidiNote));

		const int32 Index = Enemy->GetSplineRef()->GetNumberOfSplinePoints() - 1;

		FVector SplineWorldLocation = Enemy->GetSplineRef()->GetLocationAtSplinePoint(Index, ESplineCoordinateSpace::World);

		FVector FinalLocation;
		FinalLocation.X = -2300.f;								
		FinalLocation.Y = -3000.f;								
		FinalLocation.Z = SplineWorldLocation.Z;                

		Enemy->SetActorLocation(FinalLocation);
		Enemy->Attack(Enemy->GetBPM());
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
	}
	else if (PartNameFix.Equals(TEXT("Part1End")))
	{
		// Check if loop needed -> Check if Enemy no life
		if ((Enemy != nullptr) && (Enemy->GetHealth1() > 0))
		{
			StartPart1();
			if (ActiveAudioComponent != nullptr)
			{
				ActiveAudioComponent->SetTriggerParameter("MuteLeads");
				UE_LOG(LogTemp, Error, TEXT("Penis"));
			}
		}
		else
		{
			StartPart2Intro();
		}
	}
	else if (PartNameFix.Equals(TEXT("Part2IntroEnd")))
	{
		StartPart2();
	}
	else if (PartNameFix.Equals(TEXT("Part2End")))
	{
		// Check if loop needed -> Check if Enemy no life
		if ((Enemy != nullptr) && (Enemy->GetHealth2() > 0))
		{
			StartPart2();
			if (ActiveAudioComponent != nullptr)
			{
				ActiveAudioComponent->SetTriggerParameter("MuteLeads");
			}
		}
		else
		{
			StartPart3Intro();
		}
	}
	else if (PartNameFix.Equals(TEXT("Part3IntroEnd")))
	{
		StartPart3();
	}
	else if (PartNameFix.Equals(TEXT("Part3End")))
	{
		// Check if loop needed -> Check if Enemy no life
		if ((Enemy != nullptr) && (Enemy->GetHealth3() > 0))
		{
			StartPart3();
			if (ActiveAudioComponent != nullptr)
			{
				ActiveAudioComponent->SetTriggerParameter("MuteLeads");
			}
		}
		else
		{
			StartOutro();
		}
	}
	else if (PartNameFix.Equals(TEXT("OutroEnd")))
	{
		UKismetSystemLibrary::PrintString(this, FString(TEXT("All parts finished")), true, true, FLinearColor::Blue, 5.0f);
		StopClock();
	}
}
