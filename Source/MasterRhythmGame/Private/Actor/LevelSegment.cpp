// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/LevelSegment.h"

// Sets default values
ALevelSegment::ALevelSegment()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);

	SegmentMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BaseSegment"));
	SegmentMesh->SetupAttachment(Root);
}

void ALevelSegment::OnSegmentActivated()
{
}

void ALevelSegment::OnSegmentDeactivated()
{
}

// Called when the game starts or when spawned
void ALevelSegment::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALevelSegment::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!bIsOutOfBounds)
	{
		UpdateMovement(FVector::BackwardVector);
	}
	else
	{
		if (bIsLoopable)
		{
			SegmentMesh->SetVisibility(false);
		}
		else
		{
			this->Destroy();
		}
	}
}

void ALevelSegment::UpdateMovement(FVector MovementDelta)
{
	FVector CurrentLocation = GetActorLocation();
	SetActorLocation(CurrentLocation + MovementDelta);
}

void ALevelSegment::ResetToPosition(FVector NewPosition)
{
}

