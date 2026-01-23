// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Public/CoreTypes.h"
#include "LevelSegment.generated.h"

UCLASS()
class MASTERRHYTHMGAME_API ALevelSegment : public AActor
{
	GENERATED_BODY()

public:
	ALevelSegment();

	virtual void Tick(float DeltaTime) override;

	// Update the segment position based on world movement
	void UpdateMovement(FVector MovementDelta);

	// Reset segment to new spawn position
	void ResetToPosition(FVector NewPosition);

	// Get segment type for spawning logic
	ESegmentType GetSegmentType() const { return SegmentType; }

	// Get segment length for precise positioning
	float GetSegmentLength() const { return SegmentLength; }

	UFUNCTION()
	void OnSegmentActivated();

	UFUNCTION()
	void OnSegmentDeactivated();

	// --- Is Out of Bounds ---
	bool IsOutOfBounds() const { return bIsOutOfBounds; }
	void SetOutOfBounds(bool bOutOfBounds) { bIsOutOfBounds = bOutOfBounds; }

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, Category = "Default")
	TObjectPtr<USceneComponent> Root { nullptr };

	UPROPERTY(EditAnywhere, Category = "Segment")
	TObjectPtr<UStaticMeshComponent> SegmentMesh { nullptr };

	UPROPERTY(EditAnywhere, Category = "Segment")
	ESegmentType SegmentType { ESegmentType::Straight };

	UPROPERTY(EditAnywhere, Category = "Segment")
	float SegmentLength { 2700.0f };

	UPROPERTY()
	bool bIsActive { false };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	bool bIsLoopable { false };

	UPROPERTY()
	bool bIsOutOfBounds { false };
};