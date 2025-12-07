// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/TestEnemyActor.h"

// Sets default values
ATestEnemyActor::ATestEnemyActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

int32 ATestEnemyActor::CalcHealth(int32 Value)
{
	Value--;
	Health += Value;
	return Health;
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

