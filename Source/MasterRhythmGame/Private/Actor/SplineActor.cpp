// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/SplineActor.h"

#include "Components/SplineComponent.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Controller/MainMenuController.h"

// Sets default values
ASplineActor::ASplineActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>("DefaultSceneRoot");

	SetRootComponent(DefaultSceneRoot);

	MeshInstances = CreateDefaultSubobject<UInstancedStaticMeshComponent>("MeshInstances");
	MeshInstances->SetupAttachment(RootComponent);

	if (Mesh && Material)
	{
		Mesh->SetMaterial(0, Material);
	}
	

	SplineC      = CreateDefaultSubobject<USplineComponent>("SplineC");
	SplineCSharp = CreateDefaultSubobject<USplineComponent>("SplineCSharp");
	SplineD      = CreateDefaultSubobject<USplineComponent>("SplineD");
	SplineDSharp = CreateDefaultSubobject<USplineComponent>("SplineDSharp");
	SplineE      = CreateDefaultSubobject<USplineComponent>("SplineE");
	SplineF      = CreateDefaultSubobject<USplineComponent>("SplineF");
	SplineFSharp = CreateDefaultSubobject<USplineComponent>("SplineFSharp");
	SplineG      = CreateDefaultSubobject<USplineComponent>("SplineG");
	SplineGSharp = CreateDefaultSubobject<USplineComponent>("SplineGSharp");
	SplineA      = CreateDefaultSubobject<USplineComponent>("SplineA");
	SplineASharp = CreateDefaultSubobject<USplineComponent>("SplineASharp");
	SplineB      = CreateDefaultSubobject<USplineComponent>("SplineB");

	SplineC->     SetupAttachment(RootComponent);
	SplineCSharp->SetupAttachment(RootComponent);
	SplineD->     SetupAttachment(RootComponent);
	SplineDSharp->SetupAttachment(RootComponent);
	SplineE->     SetupAttachment(RootComponent);
	SplineF->     SetupAttachment(RootComponent);
	SplineFSharp->SetupAttachment(RootComponent);
	SplineG->     SetupAttachment(RootComponent);
	SplineGSharp->SetupAttachment(RootComponent);
	SplineA->     SetupAttachment(RootComponent);
	SplineASharp->SetupAttachment(RootComponent);
	SplineB->     SetupAttachment(RootComponent);
}

// Called every frame
void ASplineActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

USplineComponent* ASplineActor::GetSplines(int32 Int)
{
	Int = Int % 12;

	ENote Note = static_cast<ENote>(Int);

	switch (Note)
	{
		case ENote::C:
		{
			return SplineC;
		}
		case ENote::CSharp:
		{
			return SplineCSharp;
		}
		case ENote::D:
		{
			return SplineD;
		}
		case ENote::DSharp:
		{
			return SplineDSharp;
		}
		case ENote::E:
		{
			return SplineE;
		}
		case ENote::F:
		{
			return SplineF;
		}
		case ENote::FSharp:
		{
			return SplineFSharp;
		}
		case ENote::G:
		{
			return SplineG;
		}
		case ENote::GSharp:
		{
			return SplineGSharp;
		}
		case ENote::A:
		{
			return SplineA;
		}
		case ENote::ASharp:
		{
			return SplineASharp;
		}
		case ENote::B:
		{
			return SplineB;
		}
		default:
		{
			return nullptr;
		}
	}
}

FVector ASplineActor::GetSplinesY(int32 Int)
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

void ASplineActor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	SplineC->     SetRelativeLocation(FVector(0.0f, 0.0f, LineSpacing *  0.f));
	SplineCSharp->SetRelativeLocation(FVector(0.0f, 0.0f, LineSpacing *  1.f));
	SplineD->     SetRelativeLocation(FVector(0.0f, 0.0f, LineSpacing *  2.f));
	SplineDSharp->SetRelativeLocation(FVector(0.0f, 0.0f, LineSpacing *  3.f));
	SplineE->     SetRelativeLocation(FVector(0.0f, 0.0f, LineSpacing *  4.f));
	SplineF->     SetRelativeLocation(FVector(0.0f, 0.0f, LineSpacing *  5.f));
	SplineFSharp->SetRelativeLocation(FVector(0.0f, 0.0f, LineSpacing *  6.f));
	SplineG->     SetRelativeLocation(FVector(0.0f, 0.0f, LineSpacing *  7.f));
	SplineGSharp->SetRelativeLocation(FVector(0.0f, 0.0f, LineSpacing *  8.f));
	SplineA->     SetRelativeLocation(FVector(0.0f, 0.0f, LineSpacing *  9.f));
	SplineASharp->SetRelativeLocation(FVector(0.0f, 0.0f, LineSpacing * 10.f));
	SplineB->     SetRelativeLocation(FVector(0.0f, 0.0f, LineSpacing * 11.f));

	FVector Location;

	//SplineC
	Location = SplineC->GetLocationAtSplinePoint(1, ESplineCoordinateSpace::Local);
	Location.X = SplineLength;
	SplineC->SetLocationAtSplinePoint(1, Location, ESplineCoordinateSpace::Local);
	
	//SplineCSharp
	Location = SplineCSharp->GetLocationAtSplinePoint(1, ESplineCoordinateSpace::Local);
	Location.X = SplineLength;
	SplineCSharp->SetLocationAtSplinePoint(1, Location, ESplineCoordinateSpace::Local);

	//SplineD
	Location = SplineD->GetLocationAtSplinePoint(1, ESplineCoordinateSpace::Local);
	Location.X = SplineLength;
	SplineD->SetLocationAtSplinePoint(1, Location, ESplineCoordinateSpace::Local);
	
	//SplineDSharp
	Location = SplineDSharp->GetLocationAtSplinePoint(1, ESplineCoordinateSpace::Local);
	Location.X = SplineLength;
	SplineDSharp->SetLocationAtSplinePoint(1, Location, ESplineCoordinateSpace::Local);
	
	//SplineE
	Location = SplineE->GetLocationAtSplinePoint(1, ESplineCoordinateSpace::Local);
	Location.X = SplineLength;
	SplineE->SetLocationAtSplinePoint(1, Location, ESplineCoordinateSpace::Local);
	
	//SplineF
	Location = SplineF->GetLocationAtSplinePoint(1, ESplineCoordinateSpace::Local);
	Location.X = SplineLength;
	SplineF->SetLocationAtSplinePoint(1, Location, ESplineCoordinateSpace::Local);
	
	//SplineFSharp
	Location = SplineFSharp->GetLocationAtSplinePoint(1, ESplineCoordinateSpace::Local);
	Location.X = SplineLength;
	SplineFSharp->SetLocationAtSplinePoint(1, Location, ESplineCoordinateSpace::Local);
	
	//SplineG
	Location = SplineG->GetLocationAtSplinePoint(1, ESplineCoordinateSpace::Local);
	Location.X = SplineLength;
	SplineG->SetLocationAtSplinePoint(1, Location, ESplineCoordinateSpace::Local);
	
	//SplineGSharp
	Location = SplineGSharp->GetLocationAtSplinePoint(1, ESplineCoordinateSpace::Local);
	Location.X = SplineLength;
	SplineGSharp->SetLocationAtSplinePoint(1, Location, ESplineCoordinateSpace::Local);
	
	//SplineA
	Location = SplineA->GetLocationAtSplinePoint(1, ESplineCoordinateSpace::Local);
	Location.X = SplineLength;
	SplineA->SetLocationAtSplinePoint(1, Location, ESplineCoordinateSpace::Local);
	
	//SplineASharp
	Location = SplineASharp->GetLocationAtSplinePoint(1, ESplineCoordinateSpace::Local);
	Location.X = SplineLength;
	SplineASharp->SetLocationAtSplinePoint(1, Location, ESplineCoordinateSpace::Local);
	
	//SplineB
	Location = SplineB->GetLocationAtSplinePoint(1, ESplineCoordinateSpace::Local);
	Location.X = SplineLength;
	SplineB->SetLocationAtSplinePoint(1, Location, ESplineCoordinateSpace::Local);

	SplineC->     SetVisibility(false);
	SplineCSharp->SetVisibility(false);
	SplineD->     SetVisibility(false);
	SplineDSharp->SetVisibility(false);
	SplineE->     SetVisibility(false);
	SplineF->     SetVisibility(false);
	SplineFSharp->SetVisibility(false);
	SplineG->     SetVisibility(false);
	SplineGSharp->SetVisibility(false);
	SplineA->     SetVisibility(false);
	SplineASharp->SetVisibility(false);
	SplineB->     SetVisibility(false);

	if (MeshInstances && Mesh)
	{
		MeshInstances->ClearInstances();
		MeshInstances->SetStaticMesh(Mesh);

		const float MeshLength = Mesh->GetBounds().BoxExtent.X * 2.f;
		
		for (int32 ScaleDegree : ScaleLookupTable.Find(Scale)->Scale)
		{
			auto* Spline = GetSplines(ScaleDegree);
			Spline->SetVisibility(true);
			
			FVector Start = Spline->GetLocationAtSplinePoint(0, ESplineCoordinateSpace::Local);
			FVector End   = Spline->GetLocationAtSplinePoint(1, ESplineCoordinateSpace::Local);

			Start.Z = LineSpacing * ScaleDegree;
			End.Z   = LineSpacing * ScaleDegree;

			FVector Direction = End - Start;
			float Length = Direction.Size();
			Direction.Normalize();

			FTransform InstanceTransform;
			InstanceTransform.SetLocation((Start + End) * 0.5f);
			InstanceTransform.SetRotation(FRotationMatrix::MakeFromX(Direction).ToQuat());

			FVector InstanceScale = FVector(Length / MeshLength, 1.f, 1.f);
			InstanceTransform.SetScale3D(InstanceScale);

			MeshInstances->AddInstance(InstanceTransform);
		}
	}
}
