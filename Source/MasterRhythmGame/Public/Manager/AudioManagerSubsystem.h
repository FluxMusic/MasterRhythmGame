// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../CoreTypes.h"
#include "Subsystems/WorldSubsystem.h"
#include "TimerManager.h"
#include "AudioManagerSubsystem.generated.h"

class AGameHUD;
class ASplineActor;
class ATestEnemyActor;
class ANodeActor;
class UQuartzSubsystem;
class UQuartzClockHandle;
class ULevelData;
struct FMetaSoundOutput;

/**
 * Delegates
 */

//Fires when a Segment finished playing
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSegmentFinished, FString, SegmentName);
//Fires when the MIDI Note does
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnMIDINote, int32, MIDINote);
//Fires when the playback percent changes
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPartFinishedPercentChanged, float, Percent);

/**
 * 
 */
UCLASS()
class MASTERRHYTHMGAME_API UAudioManagerSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
public:
    UAudioManagerSubsystem();

    UFUNCTION(BlueprintCallable)
    void InitSubsystem(ULevelData* Data);

    // --- Quartz control ---
    UFUNCTION(BlueprintCallable)
    void StartClock();

    UFUNCTION(BlueprintCallable)
    void StopClock();

    UFUNCTION(BlueprintCallable)
    void ClockHandleInit(FName ClockName);

    UFUNCTION(BlueprintCallable)
    void SetBeatsPerMinute(float InBPM, FQuartzQuantizationBoundary InBoundary, FOnQuartzCommandEventBP InDelegate);

    UFUNCTION(BlueprintCallable)
    void PlayQuantized(UAudioComponent* AudioComponent);

    UFUNCTION(BlueprintCallable)
    UQuartzClockHandle* GetClockHandle() const { return ClockHandle; }

	// --- Part Name Fix accessors ---
	FString GetPartNameFix() const { return PartNameFix; }

    //Get Beat Seconds used for Input Latency Test
    double GetBeatSeconds() const { return LastBeatAudioTimeSeconds; }
    
    //Gets the Time when the Output of the Metasound is read used for Latency Test
    double GetMetaSoundOutputTimeSeconds() const { return LastBeatAudioTimeSeconds; }

    double GetLatencyBetweenThreads() const { return LatencyBetweenThreads; }

    //Get the Subsystem, used for Input Latency Test
    UQuartzSubsystem* GetQuartzSubsystem() { return QuartzSubsystem; }

    UFUNCTION(BlueprintCallable)
    float GetAnimTime();

private:

    UFUNCTION()
    void OnQuartzClockBeat(FName ClockName, EQuartzCommandQuantization QuantizationType, int32 NumBars, int32 Beat, float BeatFraction);
    
    UFUNCTION()
	void WatchOutputMidiNoteChange(FName OutputName, const FMetaSoundOutput& Output);
    
    UFUNCTION()
	void WatchOutputMidiNoteTrigger(FName OutputName, const FMetaSoundOutput& Output);

    UFUNCTION()
    void WatchOutputPartFinishedPart(FName OutputName, const FMetaSoundOutput& Output);
    
    UFUNCTION()
    void WatchOutputPartFinishedName(FName OutputName, const FMetaSoundOutput& Output);
    
    UFUNCTION()
    void WatchOutputPartFinishedPercent(FName OutputName, const FMetaSoundOutput& Output);
    
    UFUNCTION()
    void StartMusic();

    UFUNCTION()
    void StartIntro();

    UFUNCTION()
    void HandlePartIntro();

    UFUNCTION()
    void HandlePart();
    
    UFUNCTION()
    void StartOutroLoop();
    
    UFUNCTION()
    void StartOutro();

    UFUNCTION()
    void OnMusicFinished();

    UFUNCTION()
	void MuteLeads();
    
    UFUNCTION()
	void UnmuteLeads();

    UFUNCTION()
    void HandleMusicSequencing();

public:
    FOnSegmentFinished SegmentFinishedDelegate;
    FOnMIDINote MIDINoteDelegate;
    FOnPartFinishedPercentChanged PartFinishedPercentDelegate;

private:

    UPROPERTY()
    UQuartzSubsystem* QuartzSubsystem;

    UPROPERTY()
    UQuartzClockHandle* ClockHandle;

    UPROPERTY()
    int32 NumBarsDelay { 1 };

    // The currently active audio component (set when PlayQuantized is called)
    UPROPERTY()
    UAudioComponent* ActiveAudioComponent{ nullptr };

    UPROPERTY()
    int32 MIDINote { -1 };

    UPROPERTY()
    FString PartNameFix;

    FString PartPlayed;

    UPROPERTY()
    bool bPartAlreadyPlayed { false };

    UPROPERTY()
    EPartFinish PartFinish { EPartFinish::Three };

    //The current segment to play
    UPROPERTY()
    int32 CurrentSegment { 1 };

    //Flag to see if you should repeat the current part
    UPROPERTY()
    bool bRepeatCurrentSegment { false };

    //We just play the part twice (one with, one without leads) this flag keeps track
    UPROPERTY()
    bool bPlayerTurn { false };

    //Flag that plays the next part (Set this outside of subsystem)
    UPROPERTY()
    bool bEnemySegmentHealthDepleted { false };

    //Necessary to play the right part once and then repeat correctly
    bool bFirstTimePlayingSegment { true };

    //Animation Stuff

    //Animation has a small startup, this is used to calculate that (should be 0.17f but somehow that doesn't work)
    // const float AnimStartupTime { 0.17f };
    const float AnimStartupTime { 0.f };
    //The Length of the idle Animation
    const float AnimLength { 1.f };
    //The Amount of bars the anim loops over
    const float LoopLength { 2.f };

    //The explicit Time of the animation, used to sync the idle anim to the BPM
    float AnimTime { 0.f };



    //Test for Input Latency
    double LastBeatAudioTimeSeconds { 0.0 };
    
    //The Time when the MetaSound Output is read
    double MetaSoundOutputTimeSeconds { 0.0 };

    double LatencyBetweenThreads { 0.0 };
};