// Fill out your copyright notice in the Description page of Project Settings.


#include "Actor/NodeActor.h"
#include "Character/GameCharacter.h"
#include "Components/TimelineComponent.h"

// Sets default values
ANodeActor::ANodeActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	NoteMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("NoteMesh"));
	NoteMesh->SetGenerateOverlapEvents(true);
	NoteMesh->OnComponentBeginOverlap.AddDynamic(this, &ANodeActor::OnNoteBeginOverlap);
}

void ANodeActor::OnNoteBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == nullptr)
	{
		K2_DestroyActor();
	}
	AGameCharacter* OverlappinCharacter = Cast<AGameCharacter>(OtherActor);
	if (OverlappinCharacter != nullptr)
	{
		auto Defended = OverlappinCharacter->GetDefended();
		Defended++;
		OverlappinCharacter->SetDefended(Defended);
		K2_DestroyActor();
	}
}

void ANodeActor::MoveLeft()
{
	Timeline->PlayFromStart(); 
}

void ANodeActor::MoveRight()
{
	Timeline->ReverseFromEnd();
}

void ANodeActor::SetBarLength(double BPM)
{
	BPM = BPM / 240.0f;
	BPM = BPM / 1.0f;

	Timeline->SetPlayRate(BPM);
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

