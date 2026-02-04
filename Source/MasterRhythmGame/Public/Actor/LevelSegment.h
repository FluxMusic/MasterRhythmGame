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

	// Get segment type for spawning logic
	ESegmentType GetSegmentType() const { return SegmentType; }

	// Get segment length for precise positioning
	float GetSegmentLength() const { return SegmentLength; }

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(VisibleAnywhere, Category = "Default")
	TObjectPtr<USceneComponent> Root { nullptr };

	UPROPERTY(EditAnywhere, Category = "Segment")
	ESegmentType SegmentType { ESegmentType::Straight };

	UPROPERTY(EditAnywhere, Category = "Segment")
	float SegmentLength { 2700.0f };
};