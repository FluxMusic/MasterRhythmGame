// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/NodeActor.h"

// Sets default values
ANodeActor::ANodeActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ANodeActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ANodeActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

