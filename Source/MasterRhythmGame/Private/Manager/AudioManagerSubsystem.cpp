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

void UAudioManagerSubsystem::PlayQuantized()
{
	if ((AudioComponent != nullptr) && (ClockHandle != nullptr))
	{
		FOnQuartzCommandEventBP InDelegate;
		FQuartzQuantizationBoundary QuantBoundary(EQuartzCommandQuantization::Bar, 1.0f, EQuarztQuantizationReference::BarRelative, true);
		AudioComponent->PlayQuantized(this, ClockHandle, QuantBoundary, InDelegate, 0.0f, 0.0f, 1.0f, EAudioFaderCurve::Linear);
	}
	 
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

		FOnMetasoundOutputValueChanged PartFinishedDelegate;
		PartFinishedDelegate.BindUFunction(this, FName("WatchOutputOnPartFinished"));
		MetaSoundOutputSubsystem->WatchOutput(AudioComponent, FName(TEXT("OnPartFinished")), PartFinishedDelegate);

		FOnMetasoundOutputValueChanged PartNameDelegate;
		PartNameDelegate.BindUFunction(this, FName("WatchOutputPartFinishedName"));
		MetaSoundOutputSubsystem->WatchOutput(AudioComponent, FName(TEXT("PartFinishedName")), PartNameDelegate);

		FOnMetasoundOutputValueChanged PartPercentDelegate;
		PartPercentDelegate.BindUFunction(this, FName("WatchOutputPartFinishedPercent"));
		MetaSoundOutputSubsystem->WatchOutput(AudioComponent, FName(TEXT("PartFinishPercent")), PartPercentDelegate);
	}
}

void UAudioManagerSubsystem::WatchOutputOnPartFinished(FName OutputName, const FMetaSoundOutput& Output)
{
	UKismetSystemLibrary::PrintString(this, FString(TEXT("Penis")), true, true, FLinearColor::Blue, 10.0f);
}

void UAudioManagerSubsystem::WatchOutputPartFinishedName(FName OutputName, const FMetaSoundOutput& Output)
{
	UKismetSystemLibrary::PrintString(this, FString(TEXT("Penis2")), true, true, FLinearColor::Blue, 10.0f);
}

void UAudioManagerSubsystem::WatchOutputPartFinishedPercent(FName OutputName, const FMetaSoundOutput& Output)
{
	UKismetSystemLibrary::PrintString(this, FString(TEXT("Penis3")), true, true, FLinearColor::Blue, 10.0f);
}

void UAudioManagerSubsystem::StartPartOneIntro(FName ClockName, EQuartzCommandQuantization QuantizationType,
	int32 NumBars, int32 Beat, float BeatFraction)
{
}

void UAudioManagerSubsystem::StartPartOne(FName ClockName, EQuartzCommandQuantization QuantizationType, int32 NumBars,
	int32 Beat, float BeatFraction)
{
}

void UAudioManagerSubsystem::StartPartTwoIntro(FName ClockName, EQuartzCommandQuantization QuantizationType,
	int32 NumBars, int32 Beat, float BeatFraction)
{
}

void UAudioManagerSubsystem::StartPartThreeIntro(FName ClockName, EQuartzCommandQuantization QuantizationType,
	int32 NumBars, int32 Beat, float BeatFraction)
{
}

void UAudioManagerSubsystem::StartPartThree(FName ClockName, EQuartzCommandQuantization QuantizationType, int32 NumBars,
	int32 Beat, float BeatFraction)
{
}

void UAudioManagerSubsystem::StartMusic()
{
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
	UKismetSystemLibrary::PrintString(this, FString::FormatAsNumber(Beat), true, true, FLinearColor::Blue, 10.0f);
}

void UAudioManagerSubsystem::WatchOutputMidiNoteChange(FName OutputName, const FMetaSoundOutput& Output)
{
}