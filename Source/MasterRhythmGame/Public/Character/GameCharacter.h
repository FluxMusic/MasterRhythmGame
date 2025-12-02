// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "UObject/SoftObjectPtr.h"
#include "Quartz/AudioMixerClockHandle.h"
#include "GameCharacter.generated.h"

class ACameraActor;
class UQuartzClockHandle;

UCLASS()
class MASTERRHYTHMGAME_API AGameCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AGameCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	//// --- CameraActor accessors ---
	//TSoftObjectPtr<ACameraActor> GetCameraActorSoft() const { return CameraActor; }
	//ACameraActor* GetCameraActor() const { return CameraActor.Get(); }
	//void SetCameraActorSoft(const TSoftObjectPtr<ACameraActor>& InCameraActor) { CameraActor = InCameraActor; }

	// --- Health accessors ---
	int32 GetHealth() const { return Health; }
	void SetHealth(int32 InHealth) { Health = InHealth; }

	// --- Defended accessors ---
	int32 GetDefended() const { return Defended; }
	void SetDefended(int32 InDefended) { Defended = InDefended; }

	//// --- QuartzClockHandle accessors ---
	//TObjectPtr<UQuartzClockHandle> GetQuartzClockHandle() const { return QuartzClockHandle; }
	//UQuartzClockHandle* GetQuartzClock() const { return QuartzClockHandle.Get(); }
	//void SetQuartzClockHandle(TObjectPtr<UQuartzClockHandle> InHandle) { QuartzClockHandle = InHandle; }

	// --- BPM accessors ---
	float GetBPM() const { return BPM; }
	void SetBPM(float InBPM) { BPM = InBPM; }

private:
	//UPROPERTY(EditAnywhere)
	//TSoftObjectPtr<ACameraActor> CameraActor { nullptr };

	UPROPERTY(EditAnywhere)
	int32 Health { 50 };

	UPROPERTY(EditAnywhere)
	int32 Defended { 0 };

	//UPROPERTY(EditAnywhere)
	//TObjectPtr<UQuartzClockHandle> QuartzClockHandle { nullptr };

	UPROPERTY(EditAnywhere)
	float BPM { 0 };
};
