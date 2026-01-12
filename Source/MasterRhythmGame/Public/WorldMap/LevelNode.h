// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "../Public/CoreTypes.h"
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LevelNode.generated.h"

class ALevelPath;
class UTimelineComponent;
class USplineComponent;

USTRUCT()
struct FData
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	EDirectionWorldMap DirectionWorldMap;
	
	UPROPERTY(EditAnywhere)
	TObjectPtr<ALevelPath> Spline { nullptr };

	UPROPERTY(EditAnywhere)
	bool bIsUnlocked{ false };
};

UCLASS()
class MASTERRHYTHMGAME_API ALevelNode : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALevelNode();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// --- Neigbhours ---
	TArray<ALevelNode*> GetNeighbours() const { return Neighbours; }

	// --- SplineForward Ref ---
	FData GetSplineForward() const { return SplineForward; }

	// --- SplineBackward Ref ---
	FData GetSplineBackward() const { return SplineBackward; }

	// --- SplineRight Ref ---
	FData GetSplineRight() const { return SplineRight; }

	// --- SplineLeft Ref ---
	FData GetSplineLeft() const { return SplineLeft; }

	// --- Level Name ---
	FName GetLevelName() const { return LevelName; }

	UFUNCTION(BlueprintCallable)
	void SetIsUnlockedForward(bool bInUnlocked) { SplineForward.bIsUnlocked; }

	UFUNCTION(BlueprintCallable)
	void SetIsUnlockedBackward(bool bInUnlocked) { SplineBackward.bIsUnlocked; }

	UFUNCTION(BlueprintCallable)
	void SetIsUnlockedRight(bool bInUnlocked) { SplineRight.bIsUnlocked; }

	UFUNCTION(BlueprintCallable)
	void SetIsUnlockedLeft(bool bInUnlocked) { SplineLeft.bIsUnlocked; }

private:

	UPROPERTY()
	TObjectPtr<USceneComponent> DefaultSceneRoot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	TObjectPtr<UStaticMeshComponent> Mesh { nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	TArray<ALevelNode*> Neighbours { nullptr };

	UPROPERTY(EditAnywhere)
	FData SplineForward;

	UPROPERTY(EditAnywhere)
	FData SplineBackward;

	UPROPERTY(EditAnywhere)
	FData SplineRight;

	UPROPERTY(EditAnywhere)
	FData SplineLeft;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	FName LevelName;

	UPROPERTY(EditAnywhere)
	ELevels Level;

	UPROPERTY(EditAnywhere)
	bool bIsUnlocked { false };
};
