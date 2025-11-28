// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WorldMapNode.generated.h"

class AWorldMapPath;

UCLASS()
class MASTERRHYTHMGAME_API AWorldMapNode : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWorldMapNode();

	// --- Connected Nodes ---
	const TArray<AWorldMapNode*>& GetConnectedNodes() const { return ConnectedNodes; }

	// --- Connected Paths ---
	const TArray<AWorldMapPath*>& GetConnectedPaths() const { return ConnectedPaths; }

	// --- LevelName ---
	const FName& GetLevelName() const { return LevelName; }

private:

	UPROPERTY(EditAnywhere)
	TArray<AWorldMapNode*> ConnectedNodes { nullptr };

	UPROPERTY(EditAnywhere)
	TArray<AWorldMapPath*> ConnectedPaths { nullptr };

	UPROPERTY(EditAnywhere)
	FName LevelName { TEXT("")};

};
