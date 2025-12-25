// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "NodeActor.generated.h"

class USplineComponent;
class UTimelineComponent;

UCLASS()
class MASTERRHYTHMGAME_API ANodeActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ANodeActor();

	UFUNCTION()
	void OnNoteBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void MoveLeft();

	UFUNCTION()
	void MoveRight();

	UFUNCTION()
	void SetBarLength(double BPM);

	UFUNCTION()
	void HandleTimelineProgress(float Value);

	UFUNCTION()
	void HandleTimelineFinished();

	// Delayed destroy called by timer
	UFUNCTION()
	void OnDelayedDestroy();

	// --- Spline Ref ---
	TObjectPtr<USplineComponent> GetSplineRef() const { return SplineRef; }
	void SetSplineRef(TObjectPtr<USplineComponent> InSplineRef) { SplineRef = InSplineRef; }

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Default", DisplayName = "Mesh", meta = (AllowPrivateAccess = true))
	TObjectPtr<UStaticMeshComponent> NoteMesh { nullptr };

	UPROPERTY()
	TObjectPtr<USceneComponent> DefaultSceneRoot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TObjectPtr<UTimelineComponent> Timeline;

	UPROPERTY()
	TObjectPtr<USplineComponent> SplineRef;

	UPROPERTY()
	bool bIsSentBack;

	// Flags to track collisions during lifetime
	UPROPERTY()
	bool bCollidedWithPlayer { false };

	UPROPERTY()
	bool bCollidedWithEnemy { false };

	// Damage values (configurable in editor)
	UPROPERTY(EditAnywhere, Category = "Damage")
	int32 DamageToPlayer { 10 };

	UPROPERTY(EditAnywhere, Category = "Damage")
	int32 DamageToEnemy { 10 };

	// Timer handle used for delayed destroy (max 60ms)
	UPROPERTY()
	FTimerHandle DestroyTimerHandle;
};
