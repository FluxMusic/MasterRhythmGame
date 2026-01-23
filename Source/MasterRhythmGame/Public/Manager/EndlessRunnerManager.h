// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Public/CoreTypes.h"
#include "EndlessRunnerManager.generated.h"

class AGameCharacter;
class ALevelSegment;

UCLASS()
class MASTERRHYTHMGAME_API AEndlessRunnerManager : public AActor
{
	GENERATED_BODY()

public:
	AEndlessRunnerManager();

	virtual void Tick(float DeltaTime) override;

	// Change movement direction
	UFUNCTION()
	void SetMovementDirection(EMovementDirection NewDirection);

	UFUNCTION()
	void SetMovementSpeed(float NewSpeed);

protected:
	virtual void BeginPlay() override;

private:
	// Initialize segments using the pool system
	void InitializeSegments();

	// Update all segments
	void UpdateSegments(float DeltaTime);

	// Calculate movement vector based on direction
	FVector GetMovementVector() const;

	// Get spawn position for new/recycled segment
	FVector GetNextSegmentSpawnPosition() const;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true), Category = "Setup")
	TArray<TSubclassOf<ALevelSegment>> LevelSegmentPool { nullptr };

	UPROPERTY()
	TArray<TObjectPtr<ALevelSegment>> ActiveSegments;

	UPROPERTY(EditAnywhere, Category = "Setup")
	int32 MaxActiveSegments { 4 };

	UPROPERTY(EditAnywhere, Category = "Setup")
	float DefaultSegmentLength { 2700.0f };

	UPROPERTY(EditAnywhere, Category = "Movement")
	EMovementDirection MovementDirection { EMovementDirection::Forward };

	UPROPERTY(EditAnywhere, Category = "Movement")
	float MovementSpeed { 500.0f };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true), Category = "Setup")
	TSubclassOf<AGameCharacter> PlayerCharacterClass { nullptr };

	// Reference to player for distance calculations
	UPROPERTY(VisibleAnywhere, Category = "Setup")
	TObjectPtr<AGameCharacter> PlayerCharacter { nullptr };
};