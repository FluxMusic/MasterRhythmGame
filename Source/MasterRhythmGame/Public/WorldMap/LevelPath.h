// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SplineComponent.h"
#include "GameFramework/Actor.h"
#include "LevelPath.generated.h"

class AWorldMapNode;

UCLASS()
class MASTERRHYTHMGAME_API ALevelPath : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALevelPath();

	// --- Spline ---
	USplineComponent* GetSpline() const { return Spline; }

private:
	UPROPERTY()
	TObjectPtr<USceneComponent> DefaultSceneRoot;

	UPROPERTY(EditAnywhere)
	USplineComponent* Spline { nullptr };
};
