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

UAudioManagerSubsystem::UAudioManagerSubsystem()
{
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

		FOnMetasoundOutputValueChanged PartFinishedDelegate;
		PartFinishedDelegate.BindUFunction(this, FName("HandleMetaSoundOutputForPartFinished"));
		MetaSoundOutputSubsystem->WatchOutput(AudioComponent, FName(TEXT("OnPartFinished")), PartFinishedDelegate);
		OutputAudioComponentMap.Add(FName(TEXT("OnPartFinished")), AudioComponent);

		FOnMetasoundOutputValueChanged PartNameDelegate;
		PartNameDelegate.BindUFunction(this, FName("WatchOutputPartFinishedName"));
		MetaSoundOutputSubsystem->WatchOutput(AudioComponent, FName(TEXT("PartFinishedName")), PartNameDelegate);
		OutputAudioComponentMap.Add(FName(TEXT("PartFinishedName")), AudioComponent);

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
		// Immediately select the next part based on the reported name.
		if (PartName.Equals(TEXT("IntroEnd")))
		{
			StartPart1Intro();
		}
		else if (PartName.Equals(TEXT("Part1IntroEnd")))
		{
			StartPart1();
		}
		else if (PartName.Equals(TEXT("Part1End")))
		{
			StartPart2Intro();
		}
		else if (PartName.Equals(TEXT("Part2IntroEnd")))
		{
			StartPart2();
		}
		else if (PartName.Equals(TEXT("Part2End")))
		{
			StartPart3Intro();
		}
		else if (PartName.Equals(TEXT("Part3IntroEnd")))
		{
			StartPart3();
		}
		else if (PartName.Equals(TEXT("Part3End")))
		{
			UKismetSystemLibrary::PrintString(this, FString(TEXT("All parts finished")), true, true, FLinearColor::Blue, 5.0f);
			StopClock();
		}
	}
}

void UAudioManagerSubsystem::WatchOutputPartFinishedPercent(FName OutputName, const FMetaSoundOutput& Output)
{
	// Try to extract a float value from the MetaSound output
	float Value = 0.0f;
	if (Output.Get<float>(Value))
	{
		const bool bNormalized = (Value >= 0.0f && Value <= 1.0f);
		const float DisplayValue = bNormalized ? Value * 100.0f : Value;
		const FString Msg = FString::Printf(TEXT("%s = %.2f%%"), *OutputName.ToString(), DisplayValue);
		UKismetSystemLibrary::PrintString(this, Msg, true, true, FLinearColor::Yellow, 5.0f);
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

void UAudioManagerSubsystem::StartMusic()
{
	if (ActiveAudioComponent)
	{
		ActiveAudioComponent->SetTriggerParameter("PlayIntro");
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
	UKismetSystemLibrary::PrintString(this, FString::FormatAsNumber(Beat), true, true, FLinearColor::Green, 10.0f);
}

void UAudioManagerSubsystem::WatchOutputMidiNoteChange(FName OutputName, const FMetaSoundOutput& Output)
{
	// Extract MIDI note as int32 and print it
	int32 MidiNote = -1;
	if (Output.Get<int32>(MidiNote))
	{
		const FString Msg = FString::Printf(TEXT("%s = %d"), *OutputName.ToString(), MidiNote);
		UKismetSystemLibrary::PrintString(this, Msg, true, true, FLinearColor::Red, 5.0f);
	}
}