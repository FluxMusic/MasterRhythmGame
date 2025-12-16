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
    void PlayQuantized();

    UFUNCTION()
	void WatchOutputMidiNoteChange(FName OutputName, const FMetaSoundOutput& Output);

    UFUNCTION()
    void WatchOutputOnPartFinished(FName OutputName, const FMetaSoundOutput& Output);

    UFUNCTION()
    void WatchOutputPartFinishedName(FName OutputName, const FMetaSoundOutput& Output);

    UFUNCTION()
    void WatchOutputPartFinishedPercent(FName OutputName, const FMetaSoundOutput& Output);

	UFUNCTION()
    void StartPartOneIntro(FName ClockName, EQuartzCommandQuantization QuantizationType, int32 NumBars, int32 Beat, float BeatFraction);

    UFUNCTION()
    void StartPartOne(FName ClockName, EQuartzCommandQuantization QuantizationType, int32 NumBars, int32 Beat, float BeatFraction);

    UFUNCTION()
    void StartPartTwoIntro(FName ClockName, EQuartzCommandQuantization QuantizationType, int32 NumBars, int32 Beat, float BeatFraction);

    UFUNCTION()
    void StartPartThreeIntro(FName ClockName, EQuartzCommandQuantization QuantizationType, int32 NumBars, int32 Beat, float BeatFraction);

    UFUNCTION()
    void StartPartThree(FName ClockName, EQuartzCommandQuantization QuantizationType, int32 NumBars, int32 Beat, float BeatFraction);

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

    UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	TObjectPtr<UAudioComponent> AudioComponent;

    UPROPERTY()
    double Bpm { 120.f };

    UPROPERTY()
    int32 NumBarsDelay { 1 };
};