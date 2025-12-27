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

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void CreateAndStartQuartzClock(float Bpm);

	UFUNCTION()
	void ApplyDamage(int32 DamageAmount);

	int32 CalcHealth(int32 InHealth);

	// --- Health accessors ---
	int32 GetHealth() const { return Health; }
	void SetHealth(int32 InHealth) { Health = InHealth; }

	// --- Defended accessors ---
	int32 GetDefended() const { return Defended; }
	void SetDefended(int32 InDefended) { Defended = FMath::Clamp(InDefended, 0, 10); }

	// --- SceneComponent accessors ---
	USceneComponent* GetSceneComponent() const { return SceneComponent; }


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	TObjectPtr<UAudioComponent> AudioComponent { nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	TSoftObjectPtr<ACameraActor> CameraActor { nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	UQuartzClockHandle* QuartzClockHandle { nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	TObjectPtr<USceneComponent> SceneComponent { nullptr };

	UPROPERTY(EditAnywhere)
	int32 Health{ 50 };

	UPROPERTY(EditAnywhere)
	int32 Defended{ 0 };
};