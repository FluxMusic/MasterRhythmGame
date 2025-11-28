// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SplineComponent.h"
#include "GameFramework/Actor.h"
#include "WorldMapPath.generated.h"

class AWorldMapNode;

UCLASS()
class MASTERRHYTHMGAME_API AWorldMapPath : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWorldMapPath();

	FVector GetLocationAtProgress(float Progress) const;
	FRotator GetRotationAtProgress(float Progress) const;

	// --- Spline ---
	USplineComponent* GetSpline() const { return Spline; }

	// --- StartNode ---
	AWorldMapNode* GetStartNode() const { return StartNode; }

	// --- EndNode ---
	AWorldMapNode* GetEndNode() const { return EndNode; }

private:
	UPROPERTY(EditAnywhere)
	USplineComponent* Spline { nullptr };

	UPROPERTY(EditAnywhere)
	AWorldMapNode* StartNode { nullptr };

	UPROPERTY(EditAnywhere)
	AWorldMapNode* EndNode { nullptr };
};
