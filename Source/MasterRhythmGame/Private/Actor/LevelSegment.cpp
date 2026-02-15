// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/LevelSegment.h"

// Sets default values
ALevelSegment::ALevelSegment()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);
}

void ALevelSegment::Rotate()
{
	float RotationDirection = bRotateClockwise ? -1.0f : 1.0f;
	FRotator NewRotation = GetActorRotation();
	NewRotation.Yaw += RotationSpeed * RotationDirection;
	SetActorRotation(NewRotation);
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

	if (bCanRotate)
	{
		float RotationDirection = bRotateClockwise ? -1.0f : 1.0f;
		FRotator NewRotation = GetActorRotation();
		NewRotation.Yaw += RotationSpeed * RotationDirection * DeltaTime;
		SetActorRotation(NewRotation);
	}
}