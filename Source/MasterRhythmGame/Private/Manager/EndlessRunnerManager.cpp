// Fill out your copyright notice in the Description page of Project Settings.

#include "Manager/EndlessRunnerManager.h"

#include "Actor/LevelSegment.h"

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

}

void AEndlessRunnerManager::UpdateSegments(float DeltaTime)
{
}

FVector AEndlessRunnerManager::GetMovementVector() const
{
	switch (MovementDirection)
	{
		case EMovementDirection::Forward:
		{
			return FVector::ForwardVector;
		}
		case EMovementDirection::Backward:
		{
			return FVector::BackwardVector;
		}
		case EMovementDirection::Up:
		{
			return FVector::UpVector;
		}
		case EMovementDirection::Down:
		{
			return FVector::DownVector;
		}
		case EMovementDirection::Right:
		{
			return FVector::RightVector;
		}
		case EMovementDirection::Left:
		{
			return FVector::LeftVector;
		}
		default:
		{
			// Should not land here
			return FVector::ZeroVector;
		}
	}
}

FVector AEndlessRunnerManager::GetNextSegmentSpawnPosition() const
{
	return FVector::ZeroVector;
}