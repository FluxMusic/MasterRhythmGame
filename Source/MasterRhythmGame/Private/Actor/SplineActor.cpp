// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/SplineActor.h"

#include "Components/SplineComponent.h"
#include "Components/InstancedStaticMeshComponent.h"
#include "Controller/MainMenuController.h"
#include "Data/LevelData.h"
#include "Enemy/TestEnemyActor.h"
#include "Character/GameCharacter.h"
#include "Controller/GameController.h"
#include "Kismet/GameplayStatics.h"
#include "Manager/AudioManagerSubsystem.h"

// Sets default values
ASplineActor::ASplineActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>("DefaultSceneRoot");

	SetRootComponent(DefaultSceneRoot);

	MeshInstances = CreateDefaultSubobject<UInstancedStaticMeshComponent>("MeshInstances");
	MeshInstances->SetupAttachment(RootComponent);

	//if (Mesh && Material)
	//{
	//	Mesh->SetMaterial(0, Material);
	//}
	

	Spline11 = CreateDefaultSubobject<USplineComponent>("Spline11");
	Spline10 = CreateDefaultSubobject<USplineComponent>("Spline10");
	Spline9  = CreateDefaultSubobject<USplineComponent>("Spline9");
	Spline8  = CreateDefaultSubobject<USplineComponent>("Spline8");
	Spline7  = CreateDefaultSubobject<USplineComponent>("Spline7");
	Spline6  = CreateDefaultSubobject<USplineComponent>("Spline6");
	Spline5  = CreateDefaultSubobject<USplineComponent>("Spline5");
	Spline4  = CreateDefaultSubobject<USplineComponent>("Spline4");
	Spline3  = CreateDefaultSubobject<USplineComponent>("Spline3");
	Spline2  = CreateDefaultSubobject<USplineComponent>("Spline2");
	Spline1  = CreateDefaultSubobject<USplineComponent>("Spline1");
	Spline0  = CreateDefaultSubobject<USplineComponent>("Spline0");

	NoteDisplay11 = CreateDefaultSubobject<UStaticMeshComponent>("NoteDisplay11");
	NoteDisplay10 = CreateDefaultSubobject<UStaticMeshComponent>("NoteDisplay10");
	NoteDisplay9  = CreateDefaultSubobject<UStaticMeshComponent>("NoteDisplay9");
	NoteDisplay8  = CreateDefaultSubobject<UStaticMeshComponent>("NoteDisplay8");
	NoteDisplay7  = CreateDefaultSubobject<UStaticMeshComponent>("NoteDisplay7");
	NoteDisplay6  = CreateDefaultSubobject<UStaticMeshComponent>("NoteDisplay6");
	NoteDisplay5  = CreateDefaultSubobject<UStaticMeshComponent>("NoteDisplay5");
	NoteDisplay4  = CreateDefaultSubobject<UStaticMeshComponent>("NoteDisplay4");
	NoteDisplay3  = CreateDefaultSubobject<UStaticMeshComponent>("NoteDisplay3");
	NoteDisplay2  = CreateDefaultSubobject<UStaticMeshComponent>("NoteDisplay2");
	NoteDisplay1  = CreateDefaultSubobject<UStaticMeshComponent>("NoteDisplay1");
	NoteDisplay0  = CreateDefaultSubobject<UStaticMeshComponent>("NoteDisplay0");

	Spline11->SetupAttachment(RootComponent);
	Spline10->SetupAttachment(RootComponent);
	Spline9 ->SetupAttachment(RootComponent);
	Spline8 ->SetupAttachment(RootComponent);
	Spline7 ->SetupAttachment(RootComponent);
	Spline6 ->SetupAttachment(RootComponent);
	Spline5 ->SetupAttachment(RootComponent);
	Spline4 ->SetupAttachment(RootComponent);
	Spline3 ->SetupAttachment(RootComponent);
	Spline2 ->SetupAttachment(RootComponent);
	Spline1 ->SetupAttachment(RootComponent);
	Spline0 ->SetupAttachment(RootComponent);

	NoteDisplay11->SetupAttachment(RootComponent);
	NoteDisplay10->SetupAttachment(RootComponent);
	NoteDisplay9 ->SetupAttachment(RootComponent);
	NoteDisplay8 ->SetupAttachment(RootComponent);
	NoteDisplay7 ->SetupAttachment(RootComponent);
	NoteDisplay6 ->SetupAttachment(RootComponent);
	NoteDisplay5 ->SetupAttachment(RootComponent);
	NoteDisplay4 ->SetupAttachment(RootComponent);
	NoteDisplay3 ->SetupAttachment(RootComponent);
	NoteDisplay2 ->SetupAttachment(RootComponent);
	NoteDisplay1 ->SetupAttachment(RootComponent);
	NoteDisplay0 ->SetupAttachment(RootComponent);
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
			return Spline0;
		}
		case ENote::CSharp:
		{
			return Spline1;
		}
		case ENote::D:
		{
			return Spline2;
		}
		case ENote::DSharp:
		{
			return Spline3;
		}
		case ENote::E:
		{
			return Spline4;
		}
		case ENote::F:
		{
			return Spline5;
		}
		case ENote::FSharp:
		{
			return Spline6;
		}
		case ENote::G:
		{
			return Spline7;
		}
		case ENote::GSharp:
		{
			return Spline8;
		}
		case ENote::A:
		{
			return Spline9;
		}
		case ENote::ASharp:
		{
			return Spline10;
		}
		case ENote::B:
		{
			return Spline11;
		}
		default:
		{
			return nullptr;
		}
	}
}

UStaticMeshComponent* ASplineActor::GetNoteDisplay(int32 Idx)
{
	Idx = Idx % 12;

	ENote Note = static_cast<ENote>(Idx);

	switch (Note)
	{
		case ENote::C:
		{
			return NoteDisplay0;
		}
		case ENote::CSharp:
		{
			return NoteDisplay1;
		}
		case ENote::D:
		{
			return NoteDisplay2;
		}
		case ENote::DSharp:
		{
			return NoteDisplay3;
		}
		case ENote::E:
		{
			return NoteDisplay4;
		}
		case ENote::F:
		{
			return NoteDisplay5;
		}
		case ENote::FSharp:
		{
			return NoteDisplay6;
		}
		case ENote::G:
		{
			return NoteDisplay7;
		}
		case ENote::GSharp:
		{
			return NoteDisplay8;
		}
		case ENote::A:
		{
			return NoteDisplay9;
		}
		case ENote::ASharp:
		{
			return NoteDisplay10;
		}
		case ENote::B:
		{
			return NoteDisplay11;
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

TArray<int32> ASplineActor::GetScaleDegrees()
{
	TArray<int32> ScaleDegrees;

	if (LevelData)
	{
		ScaleDegrees = ScaleDegreeLookupTable.Find(LevelData->Scale)->ScaleDegrees;
	}
	
	return ScaleDegrees;
}

// Called when the game starts or when spawned
void ASplineActor::BeginPlay()
{
	Super::BeginPlay();

	//Spawn Player and Enemy
	if (!GetWorld()) return;
	
	if (LevelData)
	{
		AGameCharacter* Player = GetWorld()->SpawnActor<AGameCharacter> (LevelData->PlayerClass, Spline0->GetLocationAtSplinePoint(0, ESplineCoordinateSpace::World), GetActorRotation());
		Player->SetSplineRef(Spline0);
		ATestEnemyActor* Enemy = GetWorld()->SpawnActor<ATestEnemyActor>(LevelData->EnemyClass,  Spline0->GetLocationAtSplinePoint(1, ESplineCoordinateSpace::World), GetActorRotation());
		Enemy->Init(LevelData);

		if (AGameController* Controller = Cast<AGameController>(UGameplayStatics::GetPlayerController(GetWorld(), 0)))
		{
			Controller->InitData(LevelData);
		}
		
		UAudioManagerSubsystem* AudioManager = GetWorld()->GetSubsystem<UAudioManagerSubsystem>();
		if (AudioManager)
		{
			AudioManager->InitSubsystem(LevelData);
			AudioManager->ClockHandleInit(FName(TEXT("PlayerClock")));
			FQuartzQuantizationBoundary QuantBoundary(EQuartzCommandQuantization::Bar, 1.0f, EQuarztQuantizationReference::BarRelative, true);
			FOnQuartzCommandEventBP Delegate;
			AudioManager->SetBeatsPerMinute(LevelData->BPM, QuantBoundary, Delegate);
			AudioManager->StartClock();
			AudioManager->PlayQuantized(Enemy->GetAudioComponent());
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("ASplineActor::BeginPlay - UAudioManagerSubsystem not available."));
		}
	}
}

void ASplineActor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	Spline0 ->SetRelativeLocation(FVector(0.0f, 0.0f, LineSpacing *  0.f));
	Spline1 ->SetRelativeLocation(FVector(0.0f, 0.0f, LineSpacing *  1.f));
	Spline2 ->SetRelativeLocation(FVector(0.0f, 0.0f, LineSpacing *  2.f));
	Spline3 ->SetRelativeLocation(FVector(0.0f, 0.0f, LineSpacing *  3.f));
	Spline4 ->SetRelativeLocation(FVector(0.0f, 0.0f, LineSpacing *  4.f));
	Spline5 ->SetRelativeLocation(FVector(0.0f, 0.0f, LineSpacing *  5.f));
	Spline6 ->SetRelativeLocation(FVector(0.0f, 0.0f, LineSpacing *  6.f));
	Spline7 ->SetRelativeLocation(FVector(0.0f, 0.0f, LineSpacing *  7.f));
	Spline8 ->SetRelativeLocation(FVector(0.0f, 0.0f, LineSpacing *  8.f));
	Spline9 ->SetRelativeLocation(FVector(0.0f, 0.0f, LineSpacing *  9.f));
	Spline10->SetRelativeLocation(FVector(0.0f, 0.0f, LineSpacing * 10.f));
	Spline11->SetRelativeLocation(FVector(0.0f, 0.0f, LineSpacing * 11.f));
	
	NoteDisplay0 ->SetRelativeLocation(FVector(-DisplayOffset, 0.0f, LineSpacing *  0.f));
	NoteDisplay1 ->SetRelativeLocation(FVector(-DisplayOffset, 0.0f, LineSpacing *  1.f));
	NoteDisplay2 ->SetRelativeLocation(FVector(-DisplayOffset, 0.0f, LineSpacing *  2.f));
	NoteDisplay3 ->SetRelativeLocation(FVector(-DisplayOffset, 0.0f, LineSpacing *  3.f));
	NoteDisplay4 ->SetRelativeLocation(FVector(-DisplayOffset, 0.0f, LineSpacing *  4.f));
	NoteDisplay5 ->SetRelativeLocation(FVector(-DisplayOffset, 0.0f, LineSpacing *  5.f));
	NoteDisplay6 ->SetRelativeLocation(FVector(-DisplayOffset, 0.0f, LineSpacing *  6.f));
	NoteDisplay7 ->SetRelativeLocation(FVector(-DisplayOffset, 0.0f, LineSpacing *  7.f));
	NoteDisplay8 ->SetRelativeLocation(FVector(-DisplayOffset, 0.0f, LineSpacing *  8.f));
	NoteDisplay9 ->SetRelativeLocation(FVector(-DisplayOffset, 0.0f, LineSpacing *  9.f));
	NoteDisplay10->SetRelativeLocation(FVector(-DisplayOffset, 0.0f, LineSpacing * 10.f));
	NoteDisplay11->SetRelativeLocation(FVector(-DisplayOffset, 0.0f, LineSpacing * 11.f));

	FVector Location;

	//Spline0
	Location = Spline0->GetLocationAtSplinePoint(1, ESplineCoordinateSpace::Local);
	Location.X = SplineLength;
	Spline0->SetLocationAtSplinePoint(1, Location, ESplineCoordinateSpace::Local);
	
	//Spline1
	Location = Spline1->GetLocationAtSplinePoint(1, ESplineCoordinateSpace::Local);
	Location.X = SplineLength;
	Spline1->SetLocationAtSplinePoint(1, Location, ESplineCoordinateSpace::Local);

	//Spline2
	Location = Spline2->GetLocationAtSplinePoint(1, ESplineCoordinateSpace::Local);
	Location.X = SplineLength;
	Spline2->SetLocationAtSplinePoint(1, Location, ESplineCoordinateSpace::Local);
	
	//Spline3
	Location = Spline3->GetLocationAtSplinePoint(1, ESplineCoordinateSpace::Local);
	Location.X = SplineLength;
	Spline3->SetLocationAtSplinePoint(1, Location, ESplineCoordinateSpace::Local);
	
	//Spline4
	Location = Spline4->GetLocationAtSplinePoint(1, ESplineCoordinateSpace::Local);
	Location.X = SplineLength;
	Spline4->SetLocationAtSplinePoint(1, Location, ESplineCoordinateSpace::Local);
	
	//Spline5
	Location = Spline5->GetLocationAtSplinePoint(1, ESplineCoordinateSpace::Local);
	Location.X = SplineLength;
	Spline5->SetLocationAtSplinePoint(1, Location, ESplineCoordinateSpace::Local);
	
	//Spline6
	Location = Spline6->GetLocationAtSplinePoint(1, ESplineCoordinateSpace::Local);
	Location.X = SplineLength;
	Spline6->SetLocationAtSplinePoint(1, Location, ESplineCoordinateSpace::Local);
	
	//Spline7
	Location = Spline7->GetLocationAtSplinePoint(1, ESplineCoordinateSpace::Local);
	Location.X = SplineLength;
	Spline7->SetLocationAtSplinePoint(1, Location, ESplineCoordinateSpace::Local);
	
	//Spline8
	Location = Spline8->GetLocationAtSplinePoint(1, ESplineCoordinateSpace::Local);
	Location.X = SplineLength;
	Spline8->SetLocationAtSplinePoint(1, Location, ESplineCoordinateSpace::Local);
	
	//Spline9
	Location = Spline9->GetLocationAtSplinePoint(1, ESplineCoordinateSpace::Local);
	Location.X = SplineLength;
	Spline9->SetLocationAtSplinePoint(1, Location, ESplineCoordinateSpace::Local);
	
	//Spline10
	Location = Spline10->GetLocationAtSplinePoint(1, ESplineCoordinateSpace::Local);
	Location.X = SplineLength;
	Spline10->SetLocationAtSplinePoint(1, Location, ESplineCoordinateSpace::Local);
	
	//Spline11
	Location = Spline11->GetLocationAtSplinePoint(1, ESplineCoordinateSpace::Local);
	Location.X = SplineLength;
	Spline11->SetLocationAtSplinePoint(1, Location, ESplineCoordinateSpace::Local);

	Spline11->SetVisibility(false);
	Spline10->SetVisibility(false);
	Spline9 ->SetVisibility(false);
	Spline8 ->SetVisibility(false);
	Spline7 ->SetVisibility(false);
	Spline6 ->SetVisibility(false);
	Spline5 ->SetVisibility(false);
	Spline4 ->SetVisibility(false);
	Spline3 ->SetVisibility(false);
	Spline2 ->SetVisibility(false);
	Spline1 ->SetVisibility(false);
	Spline0 ->SetVisibility(false);
	
	NoteDisplay11->SetVisibility(false);
	NoteDisplay10->SetVisibility(false);
	NoteDisplay9 ->SetVisibility(false);
	NoteDisplay8 ->SetVisibility(false);
	NoteDisplay7 ->SetVisibility(false);
	NoteDisplay6 ->SetVisibility(false);
	NoteDisplay5 ->SetVisibility(false);
	NoteDisplay4 ->SetVisibility(false);
	NoteDisplay3 ->SetVisibility(false);
	NoteDisplay2 ->SetVisibility(false);
	NoteDisplay1 ->SetVisibility(false);
	NoteDisplay0 ->SetVisibility(false);

	if (MeshInstances && Mesh && LevelData)
	{
		MeshInstances->ClearInstances();
		MeshInstances->SetStaticMesh(Mesh);

		const float MeshLength = Mesh->GetBounds().BoxExtent.X * 2.f;
		
		for (int32 ScaleDegree : ScaleDegreeLookupTable.Find(LevelData->Scale)->ScaleDegrees)
		{
			auto* Spline = GetSplines(ScaleDegree);
			Spline->SetVisibility(true);

			auto* NoteDisplay = GetNoteDisplay(ScaleDegree);
			NoteDisplay->SetVisibility(true);
			ENote Note = static_cast<ENote>((ScaleDegree + static_cast<int32>(LevelData->RootNote)) % 12);
			NoteDisplay->SetMaterial(0, *MaterialLookupTable.Find(Note));
			
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
