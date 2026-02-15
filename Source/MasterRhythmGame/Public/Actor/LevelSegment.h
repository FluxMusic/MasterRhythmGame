// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Public/CoreTypes.h"
#include "LevelSegment.generated.h"

class AEndlessRunnerManager;

UCLASS()
class MASTERRHYTHMGAME_API ALevelSegment : public AActor
{
	GENERATED_BODY()

public:
	ALevelSegment();

	virtual void Tick(float DeltaTime) override;

	// Get segment type for spawning logic
	ESegmentType GetSegmentType() const { return SegmentType; }

	// Get segment length for precise positioning
	float GetSegmentLength() const { return SegmentLength; }

	UFUNCTION()
	void Rotate();

	bool ShouldStopAtTargetLocation() const { return bStopAtTargetLocation; }
	FVector GetTargetLocation() const { return TargetLocation; }
	float GetTargetLocationTolerance() const { return TargetLocationTolerance; }
	bool HasReachedTarget() const { return bTargetReached; }
	void SetTargetReached(bool bReached) { bTargetReached = bReached; }

	void SetRunnerManager(AEndlessRunnerManager* Manager) { RunnerManager = Manager; }

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, Category = "Default")
	TObjectPtr<USceneComponent> Root { nullptr };

	UPROPERTY(EditAnywhere, Category = "Segment")
	ESegmentType SegmentType { ESegmentType::Straight };

	UPROPERTY(EditAnywhere, Category = "Segment")
	float SegmentLength { 2700.0f };

	UPROPERTY(EditAnywhere, Category = "Segment")
	bool bCanRotate { false };

	UPROPERTY(EditAnywhere, Category = "Segment", meta = (EditCondition = "bCanRotate", EditConditionHides))
	float RotationSpeed { 90.0f };

	UPROPERTY(EditAnywhere, Category = "Segment", meta = (EditCondition = "bCanRotate", EditConditionHides))
	bool bRotateClockwise { true };

	UPROPERTY(EditAnywhere, Category = "Stop Condition")
	bool bStopAtTargetLocation { false };

	UPROPERTY(EditAnywhere, Category = "Stop Condition", meta = (EditCondition = "bStopAtTargetLocation", EditConditionHides))
	FVector TargetLocation { FVector::ZeroVector };

	UPROPERTY(EditAnywhere, Category = "Stop Condition", meta = (EditCondition = "bStopAtTargetLocation", EditConditionHides))
	float TargetLocationTolerance { 50.0f };

	UPROPERTY()
	TObjectPtr<class AEndlessRunnerManager> RunnerManager { nullptr };

	bool bTargetReached { false };
};