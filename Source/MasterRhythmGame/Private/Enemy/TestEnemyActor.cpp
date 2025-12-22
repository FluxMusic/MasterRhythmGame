// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/TestEnemyActor.h"
#include "Actor/NodeActor.h"

// Sets default values
ATestEnemyActor::ATestEnemyActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BPM = 140;

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>("DefaultSceneRoot");
	Scene = CreateDefaultSubobject<USceneComponent>("Scene");

	SetRootComponent(DefaultSceneRoot);
	Scene->SetupAttachment(DefaultSceneRoot);
}

int32 ATestEnemyActor::CalcHealth(int32 Value)
{
	Value--;
	Health += Value;
	return Health;
}

void ATestEnemyActor::Attack(int32 InBPM)
{
	auto ActorLocation = GetActorLocation();
	auto SceneLocation = Scene->GetRelativeLocation();
	auto SpawnLocation = ActorLocation + SceneLocation;
	auto Note = GetWorld()->SpawnActor<ANodeActor>(SpawnLocation, GetActorRotation());
	
	if (Note != nullptr)
	{
		//Note->SetBarLength(GetBPM());
		//Note->SetSplineRef(SplineRef);
		UE_LOG(LogTemp, Warning, TEXT("Penis"));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Hallo"));
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

