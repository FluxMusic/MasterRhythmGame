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

UAudioManagerSubsystem::UAudioManagerSubsystem()
{
}

void UAudioManagerSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	// We try to cache the Quartz subsystem here, but it may not always be available yet.
	QuartzSubsystem = UQuartzSubsystem::Get(GetWorld());

	if (QuartzSubsystem == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UAudioManagerSubsystem::Initialize - Quartz subsystem not found. Will attempt to acquire it later in StartClock()."));
	}
}

void UAudioManagerSubsystem::Deinitialize()
{
	Super::Deinitialize();
	StopClock();
}

void UAudioManagerSubsystem::StartClock(double InBPM)
{
	// Try to get the Quartz subsystem at call time (lazy acquire).
	if (QuartzSubsystem == nullptr)
	{
		QuartzSubsystem = UQuartzSubsystem::Get(GetWorld());
	}

	if (QuartzSubsystem == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("UAudioManagerSubsystem::StartClock - Quartz subsystem is missing!"));
		UE_LOG(LogTemp, Warning, TEXT("  - Ensure the Audio/Quartz modules are added to your module dependencies (Build.cs)."));
		UE_LOG(LogTemp, Warning, TEXT("  - Ensure the project is configured to use the Audio Mixer (Project Settings -> Engine -> Audio)."));
		return;
	}

	Bpm = InBPM;

	FQuartzClockSettings Settings;
	ClockHandle = QuartzSubsystem->CreateNewClock(GetWorld(), TEXT("GameplayClock"), Settings, true);

	if (!ClockHandle)
	{
		UE_LOG(LogTemp, Warning, TEXT("UAudioManagerSubsystem::StartClock - failed to create clock handle."));
		return;
	}

	// Prepare valid parameters for SetBeatsPerMinute
	FQuartzQuantizationBoundary QuantBoundary(EQuartzCommandQuantization::Beat, 1.0f, EQuarztQuantizationReference::BarRelative, true);
	FOnQuartzCommandEventBP EmptyDelegate; // no-op delegate

	// Set BPM (SetBeatsPerMinute expects a float)
	ClockHandle->SetBeatsPerMinute(this, QuantBoundary, EmptyDelegate, ClockHandle, static_cast<float>(Bpm));

	// Bind delegates as script (dynamic) delegates using BindUFunction
	FOnQuartzMetronomeEventBP BarDelegate;
	BarDelegate.BindUFunction(this, FName("OnQuartzClockBar"));

	FOnQuartzMetronomeEventBP BeatDelegate;
	BeatDelegate.BindUFunction(this, FName("OnQuartzClockBeat"));

	EQuartzCommandQuantization QuantizationType = EQuartzCommandQuantization::Bar;

	ClockHandle->SubscribeToQuantizationEvent(GetWorld(), QuantizationType, BarDelegate, ClockHandle);
	ClockHandle->SubscribeToQuantizationEvent(GetWorld(), EQuartzCommandQuantization::Beat, BeatDelegate, ClockHandle);

	ClockHandle->StartClock(this, ClockHandle);
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

void UAudioManagerSubsystem::OnQuartzClockBar(FName ClockName, EQuartzCommandQuantization QuantizationType,
	int32 NumBars, int32 Beat, float BeatFraction)
{
	NumBarsDelay++;

	// Guard AudioComponent usage
	if (!IsValid(AudioComponent))
	{
		UE_LOG(LogTemp, Warning, TEXT("UAudioManagerSubsystem::OnQuartzClockBar - AudioComponent is null or invalid."));
		return;
	}

	bool bIsPlaying = AudioComponent->IsPlaying();

	if ((NumBarsDelay == NumBars) && bIsPlaying)
	{
		AudioComponent->SetTriggerParameter(FName(TEXT("PlayIntro")));
	}
	//bIsPlaying = AudioComponent->IsPlaying();

	// TODO: Verschiedene Musikteile abspielen
	//NumBarsDelay += 2;
	//if ((NumBarsDelay >= NumBars) && bIsPlaying)
	//{
	//	AudioComponent->SetTriggerParameter(Fname(TEXT("")))
	//}
}

void UAudioManagerSubsystem::OnQuartzClockBeat(FName ClockName, EQuartzCommandQuantization QuantizationType,
	int32 NumBars, int32 Beat, float BeatFraction)
{
	UKismetSystemLibrary::PrintString(this, FString::FormatAsNumber(Beat), true, true, FLinearColor::Blue, 10.0f);
}

void UAudioManagerSubsystem::OnMidiNoteChange(FName OutputName, const FMetaSoundOutput& Output)
{
	
}
