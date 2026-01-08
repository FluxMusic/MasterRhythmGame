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
	TObjectPtr<ALevelPath> GetSplineBackward() const { return SplineBackward; }

	// --- SplineRight Ref ---
	TObjectPtr<ALevelPath> GetSplineRight() const { return SplineRight; }

	// --- SplineLeft Ref ---
	TObjectPtr<ALevelPath> GetSplineLeft() const { return SplineLeft; }

private:

	UPROPERTY(EditAnywhere)
	bool bIsUnlocked { false };

	UPROPERTY()
	TObjectPtr<USceneComponent> DefaultSceneRoot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	TObjectPtr<UStaticMeshComponent> Mesh { nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	TArray<ALevelNode*> Neighbours { nullptr };

	UPROPERTY(EditAnywhere)
	FData SplineForward;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	TObjectPtr<ALevelPath> SplineBackward { nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	TObjectPtr<ALevelPath> SplineRight { nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	TObjectPtr<ALevelPath> SplineLeft { nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	FString LevelName;
};
