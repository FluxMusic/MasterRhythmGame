// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "Quartz/QuartzTypes.h"
#include "AudioManagerSubsystem.generated.h"

class ANodeActor;
class UQuartzSubsystem;
class UQuartzClockHandle;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnQuartzClockBar);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnQuartzClockBeat);

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

    // --- Note spawning ---
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TSubclassOf<ANodeActor> NoteActorClass;

    // Events
    UPROPERTY(BlueprintAssignable)
    FOnQuartzClockBar OnQuartzClockBar;

    UPROPERTY(BlueprintAssignable)
    FOnQuartzClockBeat OnQuartzClockBeat;

private:

    UPROPERTY()
    UQuartzSubsystem* QuartzSubsystem;

    UPROPERTY()
    UQuartzClockHandle* ClockHandle;

    UPROPERTY()
    double Bpm { 120.f };

    // Handlers called by Quartz subscriptions
    UFUNCTION()
    void HandleBarEvent(const FQuartzQuantizationEventArgs& InArgs);

    UFUNCTION()
    void HandleBeatEvent(const FQuartzQuantizationEventArgs& InArgs);
};
