// Fill out your copyright notice in the Description page of Project Settings.

#include "Manager/EndlessRunnerManager.h"

AEndlessRunnerManager::AEndlessRunnerManager()
{
	PrimaryActorTick.bCanEverTick = true;
}

void AEndlessRunnerManager::BeginPlay()
{
	Super::BeginPlay();

	InitializeSegments();
}

void AEndlessRunnerManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateSegments(DeltaTime);
}

void AEndlessRunnerManager::StartMovement()
{
	UE_LOG(LogTemp, Log, TEXT("AEndlessRunnerManager::StartMovement - Endless runner started"));
}

void AEndlessRunnerManager::StopMovement()
{
	UE_LOG(LogTemp, Log, TEXT("AEndlessRunnerManager::StopMovement - Endless runner stopped"));
}

void AEndlessRunnerManager::SetMovementDirection(EMovementDirection NewDirection)
{
	MovementDirection = NewDirection;
	UE_LOG(LogTemp, Log, TEXT("AEndlessRunnerManager::SetMovementDirection - Direction changed"));
}

void AEndlessRunnerManager::SetMovementSpeed(float NewSpeed)
{
	MovementSpeed = NewSpeed;
}

void AEndlessRunnerManager::AddSegmentType()
{
	UE_LOG(LogTemp, Log, TEXT("AEndlessRunnerManager::AddSegmentType - Added new segment type"));
}

void AEndlessRunnerManager::InitializeSegments()
{

	FVector SpawnPosition = GetActorLocation();
	FVector DirectionVector = GetMovementVector().GetSafeNormal();

	for (int32 i = 0; i < MaxActiveSegments; ++i)
	{
	}
}

void AEndlessRunnerManager::UpdateSegments(float DeltaTime)
{
	FVector MovementDelta = GetMovementVector() * MovementSpeed * DeltaTime;
}

FVector AEndlessRunnerManager::GetMovementVector() const
{
	switch (MovementDirection)
	{
		case EMovementDirection::Forward:
		{
			return FVector(-1.0f, 0.0f, 0.0f);
		}
		case EMovementDirection::Backward:
		{
			return FVector(1.0f, 0.0f, 0.0f);
		}
		case EMovementDirection::Up:
		{
			return FVector(0.0f, 0.0f, -1.0f);
		}
		case EMovementDirection::Down:
		{
			return FVector(0.0f, 0.0f, 1.0f);
		}
		case EMovementDirection::Right:
		{
			return FVector(0.0f, -1.0f, 0.0f);
		}
		case EMovementDirection::Left:
		{
			return FVector(0.0f, 1.0f, 0.0f);
		}
		default:
		{
			// Should not land here#
			return FVector::ZeroVector;
		}
	}
}

FVector AEndlessRunnerManager::GetNextSegmentSpawnPosition() const
{
	return FVector::ZeroVector;
}