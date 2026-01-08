// Fill out your copyright notice in the Description page of Project Settings.

#include "Character/WorldMapPlayerCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/AudioComponent.h"
#include "Components/TimelineComponent.h"
#include "../Public/CoreTypes.h"
#include "WorldMap/LevelNode.h"
#include "Curves/CurveFloat.h"
#include "Kismet/GameplayStatics.h"
#include "WorldMap/LevelPath.h"

// Sets default values
AWorldMapPlayerCharacter::AWorldMapPlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create AudioComponent and attach it to the actor's RootComponent
	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	if (AudioComponent != nullptr)
	{
		AudioComponent->SetupAttachment(RootComponent);
		AudioComponent->bAutoActivate = false;
	}

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	SceneComponent->SetupAttachment(RootComponent);

	GetCharacterMovement()->GravityScale = 0;
	GetCharacterMovement()->bApplyGravityWhileJumping = false;

	// Create and initialize Timeline component so it's not nullptr at runtime
	Timeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("Timeline"));
}

// Called when the game starts or when spawned
void AWorldMapPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	// Create a runtime float curve that goes from (time=0, value=2) to (time=2, value=0)
	UCurveFloat* FloatCurve = NewObject<UCurveFloat>(this, TEXT("NoteFloatCurve"));
	if (FloatCurve)
	{
		FloatCurve->FloatCurve.AddKey(0.0f, 2.0f);
		FloatCurve->FloatCurve.AddKey(2.0f, 0.0f);
	}

	if (Timeline && FloatCurve)
	{
		// Bind update callback
		FOnTimelineFloat ProgressCallback;
		ProgressCallback.BindUFunction(this, FName(TEXT("HandleTimelineProgress")));

		Timeline->AddInterpFloat(FloatCurve, ProgressCallback);

		// Bind finished callback so actor is destroyed when timeline finishes
		FOnTimelineEvent FinishedCallback;
		FinishedCallback.BindUFunction(this, FName(TEXT("HandleTimelineFinished")));
		Timeline->SetTimelineFinishedFunc(FinishedCallback);

		Timeline->SetLooping(false);
		Timeline->SetTimelineLength(2.0f); // normalized length: x from 0..2
		Timeline->SetTimelineLengthMode(ETimelineLengthMode::TL_TimelineLength);
	}

	if (LevelNodeClass)
	{
		// Try to find an existing actor of the specified Blueprint class
		AActor* Found = UGameplayStatics::GetActorOfClass(GetWorld(), LevelNodeClass);
		LevelNodeRef = Cast<ALevelNode>(Found);
	}
}

// Called every frame
void AWorldMapPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

// Called to bind functionality to input
void AWorldMapPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AWorldMapPlayerCharacter::HandleTimelineProgress(float Value)
{
	float NormalizedX = 0.0f;

	if (Timeline != nullptr)
	{
		const float Length = Timeline->GetTimelineLength();
		if (Length > KINDA_SMALL_NUMBER)
		{
			NormalizedX = Timeline->GetPlaybackPosition() / Length;
		}
	}

	if (LevelNodeRef == nullptr)
	{
		return;
	}

	switch (CurrentDirection)
	{
		case EDirectionWorldMap::Forward:
		{
			const float SplineLength = LevelNodeRef->GetSplineForward().Spline->GetSpline()->GetSplineLength();
			const float DistanceAlongSpline = FMath::Clamp(NormalizedX * SplineLength, 0.0f, SplineLength);
			const FVector NewLocation = LevelNodeRef->GetSplineForward().Spline->GetSpline()->GetLocationAtDistanceAlongSpline(DistanceAlongSpline, ESplineCoordinateSpace::World);
			SetActorLocation(NewLocation);
			break;
		}
		case EDirectionWorldMap::Backward:
		{
			const float SplineLength = LevelNodeRef->GetSplineBackward()->GetSpline()->GetSplineLength();
			const float DistanceAlongSpline = FMath::Clamp(NormalizedX * SplineLength, 0.0f, SplineLength);
			const FVector NewLocation = LevelNodeRef->GetSplineBackward()->GetSpline()->GetLocationAtDistanceAlongSpline(DistanceAlongSpline, ESplineCoordinateSpace::World);
			SetActorLocation(NewLocation);
			break;
		}
		case EDirectionWorldMap::Left:
		{
			const float SplineLength = LevelNodeRef->GetSplineLeft()->GetSpline()->GetSplineLength();
			const float DistanceAlongSpline = FMath::Clamp(NormalizedX * SplineLength, 0.0f, SplineLength);
			const FVector NewLocation = LevelNodeRef->GetSplineLeft()->GetSpline()->GetLocationAtDistanceAlongSpline(DistanceAlongSpline, ESplineCoordinateSpace::World);
			SetActorLocation(NewLocation);
			break;
		}
		case EDirectionWorldMap::Right:
		{
			const float SplineLength = LevelNodeRef->GetSplineRight()->GetSpline()->GetSplineLength();
			const float DistanceAlongSpline = FMath::Clamp(NormalizedX * SplineLength, 0.0f, SplineLength);
			const FVector NewLocation = LevelNodeRef->GetSplineRight()->GetSpline()->GetLocationAtDistanceAlongSpline(DistanceAlongSpline, ESplineCoordinateSpace::World);
			SetActorLocation(NewLocation);
			break;
		}
	}
}

void AWorldMapPlayerCharacter::HandleTimelineFinished()
{
	if (LevelNodeRef == nullptr)
	{
		return;
	}

	// Find the neighbour node that is closest to the player's current location
	const FVector CurrentLocation = GetActorLocation();
	ALevelNode* ClosestNeighbour = nullptr;
	float ClosestDistSq = TNumericLimits<float>::Max();
		
	TArray<ALevelNode*> Neighbours = LevelNodeRef->GetNeighbours();
	for (ALevelNode* Neighbour : Neighbours)
	{
		if (Neighbour == nullptr)
		{
			continue;
		}

		const float DistSq = FVector::DistSquared(Neighbour->GetActorLocation(), CurrentLocation);
		if (DistSq < ClosestDistSq)
		{
			ClosestDistSq = DistSq;
			ClosestNeighbour = Neighbour;
		}
	}

	// If we found a close neighbour, update the LevelNodeRef and snap to its actor location
	const float MaxAcceptableDistance = 40000.0f; // squared distance threshold (~200 units)
	if (ClosestNeighbour != nullptr && ClosestDistSq <= MaxAcceptableDistance)
	{
		LevelNodeRef = ClosestNeighbour;
		// Snap to exact node location to avoid drifting due to spline interpolation
		SetActorLocation(LevelNodeRef->GetActorLocation());
		UE_LOG(LogTemp, Log, TEXT("AWorldMapPlayerCharacter::HandleTimelineFinished - Moved to new LevelNode: %s"), *LevelNodeRef->GetName());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("AWorldMapPlayerCharacter::HandleTimelineFinished - Could not find a suitable neighbour to set as new LevelNodeRef."));
	}
}

void AWorldMapPlayerCharacter::MoveForward(EDirectionWorldMap InDirection)
{
	CurrentDirection = InDirection;

	if (Timeline != nullptr)
	{
		Timeline->PlayFromStart();
	}
}

void AWorldMapPlayerCharacter::MoveBackward()
{
	CurrentDirection = EDirectionWorldMap::Backward;

	if (Timeline != nullptr)
	{
		Timeline->ReverseFromEnd();
	}
}
