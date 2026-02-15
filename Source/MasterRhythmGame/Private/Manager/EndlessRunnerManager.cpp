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

	if (!bMovementStopped)
	{
		UpdateSegments(DeltaTime);
	}
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

void AEndlessRunnerManager::InitializeSegments()
{
	if (LevelSegmentPool.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("AEndlessRunnerManager::InitializeSegments - LevelSegmentPool is empty"));
		return;
	}

	// Clear existing segments
	ActiveSegments.Empty();

	// Get the movement direction vector for spawning
	FVector DirectionVector = GetMovementVector();
	
	// Spawn segments from the pool
	for (int32 i = 0; i < LevelSegmentPool.Num(); ++i)
	{
		if (LevelSegmentPool[i] == nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("AEndlessRunnerManager::InitializeSegments - Segment class at index %d is null"), i);
			continue;
		}

		// Calculate spawn position based on direction and default segment length
		FVector SpawnPosition = (DirectionVector * DefaultSegmentLength * i) + SpawnLocationOffset;
		
		// Spawn the segment
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

		ALevelSegment* NewSegment = GetWorld()->SpawnActor<ALevelSegment>(
			LevelSegmentPool[i],
			SpawnPosition,
			SpawnRotation,
			SpawnParams
		);

		if (NewSegment)
		{
			NewSegment->SetActorScale3D(SpawnScale);
			NewSegment->SetRunnerManager(this);
			ActiveSegments.Add(NewSegment);
			UE_LOG(LogTemp, Log, TEXT("AEndlessRunnerManager::InitializeSegments - Spawned segment at position: %s"), *SpawnPosition.ToString());
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("AEndlessRunnerManager::InitializeSegments - Failed to spawn segment at index %d"), i);
		}
	}
}

void AEndlessRunnerManager::UpdateSegments(float DeltaTime)
{
	if (ActiveSegments.Num() == 0)
	{
		return;
	}

	// Get spawn direction vector
	FVector SpawnDirectionVector = GetMovementVector();
	
	// Movement is opposite to spawn direction
	FVector MovementVector = -SpawnDirectionVector;
	FVector MovementDelta = MovementVector * MovementSpeed * DeltaTime;

	// Manager's position for distance calculation
	FVector ManagerPosition = GetActorLocation();

	// Maximum distance before recycling (2x segment length as threshold)
	float MaxDistance = DefaultSegmentLength * 2.0f;

	// Update all active segments
	for (int32 i = 0; i < ActiveSegments.Num(); ++i)
	{
		TObjectPtr<ALevelSegment> Segment = ActiveSegments[i];
		
		if (Segment && IsValid(Segment))
		{
			// Move segment (opposite to spawn direction)
			FVector CurrentLocation = Segment->GetActorLocation();
			FVector NewLocation = CurrentLocation + MovementDelta;
			Segment->SetActorLocation(NewLocation);

			// Check if segment should stop at target location
			if (Segment->ShouldStopAtTargetLocation() && !Segment->HasReachedTarget())
			{
				float Distance = FVector::Dist(NewLocation, Segment->GetTargetLocation());
				if (Distance <= Segment->GetTargetLocationTolerance())
				{
					Segment->SetTargetReached(true);
					bMovementStopped = true;
					UE_LOG(LogTemp, Log, TEXT("AEndlessRunnerManager::UpdateSegments - Segment reached target location! Stopping movement"));
					return;
				}
			}

			// Check if segment passed the manager (behind in spawn direction)
			FVector ToSegment = CurrentLocation - ManagerPosition;
			float Distance = FVector::DotProduct(ToSegment, SpawnDirectionVector);

			// If segment is behind the manager (negative distance in spawn direction)
			if (Distance < -MaxDistance)
			{
				// Find the last segment in the array
				FVector LastSegmentPosition = GetNextSegmentSpawnPosition();
				
				// Move this segment to the end
				Segment->SetActorLocation(LastSegmentPosition);

				UE_LOG(LogTemp, Log, TEXT("AEndlessRunnerManager::UpdateSegments - Recycled segment from %s to %s"), 
					*CurrentLocation.ToString(), *LastSegmentPosition.ToString());
			}
		}
	}
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
	if (ActiveSegments.Num() == 0)
	{
		return GetActorLocation();
	}

	// Find the segment furthest in the spawn direction
	FVector SpawnDirectionVector = GetMovementVector();
	FVector ManagerPosition = GetActorLocation();
	
	float FurthestDistance = -FLT_MAX;
	FVector FurthestPosition = FVector::ZeroVector;

	for (const TObjectPtr<ALevelSegment>& Segment : ActiveSegments)
	{
		if (Segment && IsValid(Segment))
		{
			FVector SegmentPosition = Segment->GetActorLocation();
			FVector ToSegment = SegmentPosition - ManagerPosition;
			float Distance = FVector::DotProduct(ToSegment, SpawnDirectionVector);

			if (Distance > FurthestDistance)
			{
				FurthestDistance = Distance;
				FurthestPosition = SegmentPosition;
			}
		}
	}

	// Place new segment one segment length ahead in spawn direction
	return FurthestPosition + (SpawnDirectionVector * DefaultSegmentLength);
}