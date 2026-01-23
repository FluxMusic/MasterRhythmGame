// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/SegmentCollision.h"
#include "Actor/LevelSegment.h"

// Sets default values
ASegmentCollision::ASegmentCollision()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	SetRootComponent(CollisionBox);
	
	CollisionBox->SetBoxExtent(FVector(100.0f, 1500.0f, 500.0f));
	CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionBox->SetCollisionResponseToAllChannels(ECR_Overlap);
}

// Called when the game starts or when spawned
void ASegmentCollision::BeginPlay()
{
	Super::BeginPlay();
	
	CollisionBox->OnComponentBeginOverlap.AddDynamic(this, &ASegmentCollision::OnCollisionBoxOverlap);
}

void ASegmentCollision::OnCollisionBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (ALevelSegment* LevelSegment = Cast<ALevelSegment>(OtherActor))
	{
		LevelSegment->SetOutOfBounds(true);
	}
}

// Called every frame
void ASegmentCollision::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

