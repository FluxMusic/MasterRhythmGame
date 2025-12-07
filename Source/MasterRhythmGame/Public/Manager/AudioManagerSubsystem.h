// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "AudioManagerSubsystem.generated.h"

class UQuartzSubsystem;
class UQuartzClockHandle;
/**
 * 
 */
UCLASS()
class MASTERRHYTHMGAME_API UAudioManagerSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()
public:
    UAudioManagerSubsystem();

    // === LIFECYCLE ===
    virtual void Initialize(FSubsystemCollectionBase& Collection) override;
    virtual void Deinitialize() override;

    // === QUARTZ CONTROL ===
    UFUNCTION(BlueprintCallable)
    void StartClock(double InBPM);

    UFUNCTION(BlueprintCallable)
    void StopClock();

    // === NOTE SPAWNING ===
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TSubclassOf<AActor> NoteActorClass;

private:

    UPROPERTY()
    UQuartzSubsystem* QuartzSubsystem;

    UPROPERTY()
    UQuartzClockHandle* ClockHandle;

    UPROPERTY()
    double Bpm { 120.f };
};
