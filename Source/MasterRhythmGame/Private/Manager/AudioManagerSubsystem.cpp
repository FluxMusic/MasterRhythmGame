// Fill out your copyright notice in the Description page of Project Settings.


#include "Manager/AudioManagerSubsystem.h"
#include "Quartz/AudioMixerClockHandle.h"
#include "Quartz/QuartzSubsystem.h"
#include "Quartz/QuartzSystem.h"
#include "Quartz/QuartzTypes.h"
#include "Actor/NodeActor.h"

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
	FQuartzQuantizationBoundary QuantBoundaryBar(EQuartzCommandQuantization::Bar , 1.0f, EQuartzQuantizationReference::BarRelative, true);
	FQuartzQuantizationBoundary QuantBoundaryBeat(EQuartzCommandQuantization::Beat , 1.0f, EQuarztQuantizationReference::BarRelative, true);
	FOnQuartzCommandEventBP EmptyDelegate; // no-op delegate

	// Set BPM (passes raw pointer by reference)
	ClockHandle->SetBeatsPerMinute(this, QuantBoundaryBar, EmptyDelegate, ClockHandle, Bpm);

	// Subscribe to bar and beat events
	ClockHandle->SubscribeToQuantizationEvent(this, QuantBoundaryBar, FOnQuartzMetronomeEventBP::CreateUObject(this, &UAudioManagerSubsystem::HandleBarEvent));
	ClockHandle->SubscribeToQuantizationEvent(this, QuantBoundaryBeat, FOnQuartzMetronomeEventBP::CreateUObject(this, &UAudioManagerSubsystem::HandleBeatEvent));

	ClockHandle->StartClock(this, ClockHandle);
	// TODO: Audio Component -> Play Quantized Sound
	// TODO: Watch Outputs
}

void UAudioManagerSubsystem::HandleBarEvent(const FQuartzQuantizationEventArgs& InArgs)
{
	// Broadcast Blueprint event for bar
	OnQuartzClockBar.Broadcast();
}

void UAudioManagerSubsystem::HandleBeatEvent(const FQuartzQuantizationEventArgs& InArgs)
{
	// Broadcast Blueprint event for beat
	OnQuartzClockBeat.Broadcast();
}

void UAudioManagerSubsystem::StopClock()
{
	if (QuartzSubsystem && ClockHandle)
	{
		ClockHandle->StopClock(this, ClockHandle);
		ClockHandle = nullptr;
	}
}
