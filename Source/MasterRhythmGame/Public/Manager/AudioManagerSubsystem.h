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

    // --- Lifecycle ---
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;

    // --- Quartz control ---
    UFUNCTION(BlueprintCallable)
    void StartClock(double InBPM);

    UFUNCTION(BlueprintCallable)
    void StopClock();

    UFUNCTION()
	void OnQuartzClockBar(FName ClockName, EQuartzCommandQuantization QuantizationType, int32 NumBars, int32 Beat, float BeatFraction);

    UFUNCTION()
    void OnQuartzClockBeat(FName ClockName, EQuartzCommandQuantization QuantizationType, int32 NumBars, int32 Beat, float BeatFraction);

    UFUNCTION()
    void OnMidiNoteChange(FName OutputName, const FMetaSoundOutput& Output);

    // --- Note spawning ---
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TSubclassOf<ANodeActor> NoteActorClass;

private:

    UPROPERTY()
    UQuartzSubsystem* QuartzSubsystem;

    UPROPERTY()
    UQuartzClockHandle* ClockHandle;

    UPROPERTY()
	TObjectPtr<UAudioComponent> AudioComponent;

    UPROPERTY()
    double Bpm { 120.f };

    UPROPERTY()
    int32 NumBarsDelay { 1 };
};
