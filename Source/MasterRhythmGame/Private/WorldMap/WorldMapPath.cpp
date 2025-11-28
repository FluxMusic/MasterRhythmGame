// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldMap/WorldMapPath.h"

// Sets default values
AWorldMapPath::AWorldMapPath()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Spline = CreateDefaultSubobject<USplineComponent>(TEXT("Spline"));
	RootComponent = Spline;
}

FVector AWorldMapPath::GetLocationAtProgress(float Progress) const
{
	return Spline->GetLocationAtTime(Progress, ESplineCoordinateSpace::World);
}

FRotator AWorldMapPath::GetRotationAtProgress(float Progress) const
{
	return Spline->GetRotationAtTime(Progress, ESplineCoordinateSpace::World);
}