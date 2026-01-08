// Fill out your copyright notice in the Description page of Project Settings.


#include "WorldMap/LevelNode.h"

// Sets default values
ALevelNode::ALevelNode()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>("DefaultSceneRoot");
	SetRootComponent(DefaultSceneRoot);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);

	Mesh->SetGenerateOverlapEvents(false);
	Mesh->SetCollisionProfileName(TEXT("NoCollision"));
}
// Called when the game starts or when spawned
void ALevelNode::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ALevelNode::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

