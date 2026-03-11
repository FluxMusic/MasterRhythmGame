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
{}

void UAudioManagerSubsystem::InitSubsystem(ULevelData* LevelData)
{
	if (QuartzSubsystem == nullptr)
	{
		QuartzSubsystem = UQuartzSubsystem::Get(GetWorld());
	}
	if (LevelData)
	{
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
		MidiDelegate.BindDynamic(this, &UAudioManagerSubsystem::WatchOutputMidiNoteChange);
		MetaSoundOutputSubsystem->WatchOutput(AudioComponent, FName(TEXT("MIDINoteOut")), MidiDelegate);
		
		FOnMetasoundOutputValueChanged MidiTriggerDelegate;
		MidiTriggerDelegate.BindDynamic(this, &UAudioManagerSubsystem::WatchOutputMidiNoteTrigger);
		MetaSoundOutputSubsystem->WatchOutput(AudioComponent, FName(TEXT("OnMIDINoteOn")), MidiTriggerDelegate);

		FOnMetasoundOutputValueChanged PartNameDelegate;
		PartNameDelegate.BindDynamic(this, &UAudioManagerSubsystem::WatchOutputPartFinishedName);
		MetaSoundOutputSubsystem->WatchOutput(AudioComponent, FName(TEXT("PartFinishedName")), PartNameDelegate);

		FOnMetasoundOutputValueChanged PartFinishedDelegate;
		PartFinishedDelegate.BindDynamic(this, &UAudioManagerSubsystem::WatchOutputPartFinishedPart);
		MetaSoundOutputSubsystem->WatchOutput(AudioComponent, FName(TEXT("OnPartFinished")), PartFinishedDelegate);

		FOnMetasoundOutputValueChanged PartPercentDelegate;
		PartPercentDelegate.BindDynamic(this, &UAudioManagerSubsystem::WatchOutputPartFinishedPercent);
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
	if (Output.Get<int32>(MIDINote))
	{
		const FString Msg = FString::Printf(TEXT("%s = %d"), *OutputName.ToString(), MIDINote);
		UKismetSystemLibrary::PrintString(this, Msg, true, true, FLinearColor::Red, 5.0f);
	}
}

void UAudioManagerSubsystem::WatchOutputMidiNoteTrigger(FName OutputName, const FMetaSoundOutput& Output)
{
	MIDINoteDelegate.Broadcast(MIDINote);
}

void UAudioManagerSubsystem::WatchOutputPartFinishedPart(FName OutputName, const FMetaSoundOutput& Output)
{
	if (PartNameFix != PartPlayed)
	{
		PartPlayed = PartNameFix;
		bPartAlreadyPlayed = false;
	}
	else
	{
		bPartAlreadyPlayed = true;
	}

	//Broadcast Signal first so we can have the Controller etc check some stuff for the looping
	SegmentFinishedDelegate.Broadcast(PartNameFix);

	HandleMusicSequencing();
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
	float Value = 0.0f;
	if (Output.Get<float>(Value))
	{
		PartFinishedPercentDelegate.Broadcast(Value);
	}
}

void UAudioManagerSubsystem::StartMusic()
{
	StartIntro();
}

void UAudioManagerSubsystem::StartIntro()
{
	if (ActiveAudioComponent)
	{
		ActiveAudioComponent->SetTriggerParameter("PlayIntro");
	}
}

void UAudioManagerSubsystem::HandlePartIntro()
{
	if (bRepeatCurrentSegment || bFirstTimePlayingSegment)	
	{
		FString PlaybackString = "PlayPart"; 
		PlaybackString += FString::FromInt(CurrentSegment); 
		PlaybackString += "Intro";

		ActiveAudioComponent->SetTriggerParameter(FName(PlaybackString));

		bFirstTimePlayingSegment = false;
	}
	else
	{
		HandlePart();

		bFirstTimePlayingSegment = true;
	}
}

void UAudioManagerSubsystem::HandlePart()
{
	//if enemy health bar is 0, play next segment intro
	if (bEnemySegmentHealthDepleted)
	{
		bEnemySegmentHealthDepleted = false;

		CurrentSegment++;
		
		//Play next part if there is one, otherwise play the outro
		if (CurrentSegment <= static_cast<int32>(PartFinish))
		{
			HandlePartIntro();
		}
		else
		{
			StartOutroLoop();
		}

		return;
	}

	FString PlaybackString = "PlayPart"; 
	PlaybackString += FString::FromInt(CurrentSegment);

	ActiveAudioComponent->SetTriggerParameter(FName(PlaybackString));
	
	if (!bPlayerTurn)	
	{
		//Enemy Turn - needs Melody	
		UnmuteLeads();
	}
	else
	{
		//Player Turn - Player makes his own Melody
		MuteLeads();

		//Remove this. Just to cycle through the song for now
		bEnemySegmentHealthDepleted = true;
	}
	//Also just to cycle through
	bPlayerTurn = !bPlayerTurn;
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

void UAudioManagerSubsystem::HandleMusicSequencing()
{
	// Immediately select the next part based on the reported name.
	if (PartNameFix.Equals(TEXT("IntroEnd")))
	{
		HandlePartIntro();
	}
	else if (PartNameFix.Equals(TEXT("OutroLoopEnd")))
	{
		if (bRepeatCurrentSegment)
		{
			StartOutroLoop();
		}
		else
		{
			StartOutro();
		}
	}
	else if (PartNameFix.Equals(TEXT("OutroEnd")))
	{
		OnMusicFinished();
	}
	else
	{
		//This collects all of the other instances
		if (PartNameFix.Contains("Intro"))
		{
			HandlePartIntro();
		}
		else
		{
			HandlePart();
		}
	}
}

void UAudioManagerSubsystem::OnMusicFinished()
{
	UKismetSystemLibrary::PrintString(this, FString(TEXT("All parts finished")), true, true, FLinearColor::Blue, 5.0f);
	StopClock();
}
