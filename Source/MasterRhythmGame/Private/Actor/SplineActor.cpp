// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/SplineActor.h"

#include "Components/SplineComponent.h"
#include "Controller/MainMenuController.h"

// Sets default values
ASplineActor::ASplineActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>("DefaultSceneRoot");

	SetRootComponent(DefaultSceneRoot);

	Spline = CreateDefaultSubobject<USplineComponent>("Spline");
	Spline1 = CreateDefaultSubobject<USplineComponent>("Spline1");
	Spline2 = CreateDefaultSubobject<USplineComponent>("Spline2");
	Spline3 = CreateDefaultSubobject<USplineComponent>("Spline3");
	Spline4 = CreateDefaultSubobject<USplineComponent>("Spline4");
	Spline5 = CreateDefaultSubobject<USplineComponent>("Spline5");
	Spline6 = CreateDefaultSubobject<USplineComponent>("Spline6");

	Spline->SetupAttachment(RootComponent);
	Spline1->SetupAttachment(RootComponent);
	Spline2->SetupAttachment(RootComponent);
	Spline3->SetupAttachment(RootComponent);
	Spline4->SetupAttachment(RootComponent);
	Spline5->SetupAttachment(RootComponent);
	Spline6->SetupAttachment(RootComponent);

	Spline1->SetRelativeLocation(FVector(0.0f, 0.0f, 150.0f));
	Spline2->SetRelativeLocation(FVector(0.0f, 0.0f, 300.0f));
	Spline3->SetRelativeLocation(FVector(0.0f, 0.0f, 450.0f));
	Spline4->SetRelativeLocation(FVector(0.0f, 0.0f, 650.0f));
	Spline5->SetRelativeLocation(FVector(0.0f, 0.0f, 750.0f));
	Spline6->SetRelativeLocation(FVector(0.0f, 0.0f, 900.0f));
}

USplineComponent* ASplineActor::GetSplines(int32 Int)
{
	Int = Int % 12;

	ENote Note = static_cast<ENote>(Int);

	switch (Note)
	{
		case ENote::C:
		{
			return Spline;
		}
		case ENote::D:
		{
			return Spline1;
		}
		case ENote::E:
		{
			return Spline2;
		}
		case ENote::F:
		{
			return Spline3;
		}
		case ENote::G:
		{
			return Spline4;
		}
		case ENote::A:
		{
			return Spline5;
		}
		case ENote::B:
		{
			return Spline6;
		}
		default:
		{
			return nullptr;
		}
	}
}

FVector ASplineActor::GetSplinesY(int32 Int, FVector& NewParam)
{
	Int = Int % 12;

	ENote Note = static_cast<ENote>(Int);

	switch (Note)
	{
		case ENote::C:
		{
			return FVector(0.0, 0.0, 0.0);
		}
		case ENote::D:
		{
			return FVector(0.0, 0.0, 150.0);
		}
		case ENote::E:
		{
			return FVector(0.0, 0.0, 300.0);;
		}
		case ENote::F:
		{
			return FVector(0.0, 0.0, 450.0);;
		}
		case ENote::G:
		{
			return FVector(0.0, 0.0, 600.0);;
		}
		case ENote::A:
		{
			return FVector(0.0, 0.0, 750.0);;
		}
		case ENote::B:
		{
			return FVector(0.0, 0.0, 900.0);;
		}
		default:
		{
			return FVector(0.0f, 0.0f, 0.0f);
		}
	}
}

// Called when the game starts or when spawned
void ASplineActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASplineActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

