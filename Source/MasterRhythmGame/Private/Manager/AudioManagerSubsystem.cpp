// Fill out your copyright notice in the Description page of Project Settings.


#include "Manager/AudioManagerSubsystem.h"
#include "Quartz/AudioMixerClockHandle.h"
#include "Quartz/QuartzSubsystem.h"

UAudioManagerSubsystem::UAudioManagerSubsystem()
{
}

void UAudioManagerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	QuartzSubsystem = GetWorld()->GetSubsystem<UQuartzSubsystem>();

	if (QuartzSubsystem == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Subsystem not correctly initialized!"));
	}
}

void UAudioManagerSubsystem::Deinitialize()
{
	Super::Deinitialize();
	StopClock();
}

void UAudioManagerSubsystem::StartClock(double InBPM)
{
	if (QuartzSubsystem == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Quartz Subsystem is missing!"));
		return;
	}
	Bpm = InBPM;

	FQuartzClockSettings Settings;
	ClockHandle = QuartzSubsystem->CreateNewClock(GetWorld(), TEXT("GameplayClock"), Settings, true);
	
	// Prepare valid parameters for SetBeatsPerMinute
	FQuartzQuantizationBoundary QuantBoundary(EQuartzCommandQuantization::Beat, 1.0f, EQuarztQuantizationReference::BarRelative, true);
	FOnQuartzCommandEventBP EmptyDelegate; // no-op delegate

	// Set BPM (passes raw pointer by reference)
	ClockHandle->SetBeatsPerMinute(this, QuantBoundary, EmptyDelegate, ClockHandle, Bpm);
	ClockHandle->StartClock(this, ClockHandle);
}

void UAudioManagerSubsystem::StopClock()
{
	if (QuartzSubsystem && ClockHandle)
	{
		StopClock();
	}
}
