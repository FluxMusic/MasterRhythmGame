// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LevelNode.generated.h"

class UTimelineComponent;
class USplineComponent;

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

private:

	UPROPERTY(EditAnywhere)
	bool bIsUnlocked { false };

	UPROPERTY()
	TObjectPtr<USceneComponent> DefaultSceneRoot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	TObjectPtr<UStaticMeshComponent> Mesh { nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	TArray<ALevelNode*> Neighbours { nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	FString LevelName;
};
