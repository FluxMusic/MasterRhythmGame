// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/TestEnemyActor.h"
#include "Actor/NodeActor.h"

// Sets default values
ATestEnemyActor::ATestEnemyActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SetBPM(141);

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>("DefaultSceneRoot");
	Scene = CreateDefaultSubobject<USceneComponent>("Scene");

	SetRootComponent(DefaultSceneRoot);
	Scene->SetupAttachment(RootComponent);

	SetHealth1(50);
	SetHealth2(50);
	SetHealth3(50);
}

//int32 ATestEnemyActor::CalcHealth(int32 Value)
//{
//	Value--;
//	Health += Value;
//	return Health;
//}

void ATestEnemyActor::Attack(int32 InBPM)
{

	FVector SpawnLocation;
	auto ActorLocation = GetActorLocation();
	auto SceneLocation = Scene->GetRelativeLocation();
	SpawnLocation.X = -2300.f;
	SpawnLocation.Y = -3000.f + SceneLocation.Y;
	SpawnLocation.Z = ActorLocation.Z;
	auto Note = GetWorld()->SpawnActor<ANodeActor>(NodeActor, SpawnLocation, GetActorRotation());
	
	if (Note != nullptr)
	{
		Note->SetBarLength(InBPM);
		Note->SetSplineRef(SplineRef);
		Note->MoveLeft();
	}
}

// Called when the game starts or when spawned
void ATestEnemyActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATestEnemyActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

