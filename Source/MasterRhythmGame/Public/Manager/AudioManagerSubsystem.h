// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "AudioManagerSubsystem.generated.h"

class ANodeActor;
class UQuartzSubsystem;
class UQuartzClockHandle;
struct FMetaSoundOutput;

/**
 * 
 */
UCLASS()
class MASTERRHYTHMGAME_API UAudioManagerSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
public:
    UAudioManagerSubsystem();

    UFUNCTION()
    void InitSubsystem();

    // --- Quartz control ---
    UFUNCTION(BlueprintCallable)
    void StartClock();

    UFUNCTION(BlueprintCallable)
    void StopClock();

    UFUNCTION()
    void ClockHandleInit(FName ClockName);

    UFUNCTION()
    void SetBeatsPerMinute(float InBPM, FQuartzQuantizationBoundary InBoundary, FOnQuartzCommandEventBP InDelegate);

    UFUNCTION()
    void PlayQuantized(UAudioComponent* AudioComponent);

    UFUNCTION()
	void WatchOutputMidiNoteChange(FName OutputName, const FMetaSoundOutput& Output);

    UFUNCTION()
    void WatchOutputPartFinishedName(FName OutputName, const FMetaSoundOutput& Output);

    UFUNCTION()
    void WatchOutputPartFinishedPercent(FName OutputName, const FMetaSoundOutput& Output);

    UFUNCTION()
    void StartIntro();

    UFUNCTION()
    void StartPart1Intro();

    UFUNCTION()
    void StartPart1();

    UFUNCTION()
    void StartPart2Intro();

    UFUNCTION()
    void StartPart2();

    UFUNCTION()
    void StartPart3Intro();

    UFUNCTION()
    void StartPart3();

    UFUNCTION()
    void OnQuartzClockBeat(FName ClockName, EQuartzCommandQuantization QuantizationType, int32 NumBars, int32 Beat, float BeatFraction);

    UFUNCTION()
    void StartMusic();

    // --- Note spawning ---
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TSubclassOf<ANodeActor> NoteActorClass;

private:

    UPROPERTY()
    UQuartzSubsystem* QuartzSubsystem;

    UPROPERTY()
    UQuartzClockHandle* ClockHandle;

    UPROPERTY()
    double Bpm { 120.f };

    UPROPERTY()
    int32 NumBarsDelay { 1 };

    // Map to find the AudioComponent for a named MetaSound output (populated in PlayQuantized)
    UPROPERTY()
    TMap<FName, UAudioComponent*> OutputAudioComponentMap;

    // The currently active audio component (set when PlayQuantized is called)
    UPROPERTY()
    UAudioComponent* ActiveAudioComponent{ nullptr };
};