// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../CoreTypes.h"
#include "GameFramework/Actor.h"
#include "NodeActor.generated.h"

class ULevelData;
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
	void SetBarLength(double BPM, int32 NumBars);

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

	// --- Timeline ---
	TObjectPtr<UTimelineComponent> GetTimeline() const { return Timeline; }

	EInstrument GetInstrument() const { return Instrument; }
	void SetInstrument(EInstrument InstrumentIn) { Instrument = InstrumentIn; }


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
	EInstrument Instrument { EInstrument::Piano };

	// Damage values (configurable in editor)
	UPROPERTY(EditAnywhere, Category = "Damage")
	int32 DamageToPlayer { 10 };

	UPROPERTY(EditAnywhere, Category = "Damage")
	int32 DamageToEnemy { 10 };

	// Timer handle used for delayed destroy (max 60ms)
	UPROPERTY()
	FTimerHandle DestroyTimerHandle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ULevelData* LevelData;

	/**
	 * The Time added for the note to stay by the player to account for latency
	 * Max Latency between Audio and Game Thread: approx. 45 Ms
	 * Input Latency with good timing: about 120 Ms
	 * Altogether, an Offset of 200 should be good for players with a good feel of Rhythm
	 * For Players with a worse sense of Rhythm, we should keep the offset higher, like 250 Ms
	 */
	UPROPERTY(EditDefaultsOnly)
	double LatencyOffsetMs { 250.0 };
};
