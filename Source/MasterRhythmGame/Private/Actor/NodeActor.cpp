// Fill out your copyright notice in the Description page of Project Settings.

#include "Actor/NodeActor.h"
#include "Character/GameCharacter.h"
#include "Components/TimelineComponent.h"
#include "Components/SplineComponent.h"
#include "Curves/CurveFloat.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/StaticMeshComponent.h"
#include "Enemy/TestEnemyActor.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ANodeActor::ANodeActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	NoteMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("NoteMesh"));
	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>("DefaultSceneRoot");
	SetRootComponent(DefaultSceneRoot);

	// set overlap/collision behavior
	NoteMesh->SetGenerateOverlapEvents(true);
	NoteMesh->OnComponentBeginOverlap.AddDynamic(this, &ANodeActor::OnNoteBeginOverlap);
	NoteMesh->SetCollisionProfileName(TEXT("OverlapAllDynamic"));

	NoteMesh->SetupAttachment(RootComponent);

	// Create and initialize Timeline component so it's not nullptr at runtime
	Timeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("Timeline"));

	// initialize collision flags
	bCollidedWithPlayer = false;
	bCollidedWithEnemy = false;

	// default damage values (can be changed in editor)
	DamageToPlayer = 10;
	DamageToEnemy = 10;
}

void ANodeActor::OnNoteBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor == nullptr)
	{
		this->Destroy();
		return;
	}

	// Player collision: mark flag 
	AGameCharacter* OverlappingCharacter = Cast<AGameCharacter>(OtherActor);
	if (OverlappingCharacter != nullptr)
	{
		// mark that we collided with the player so we don't apply damage when timeline finishes
		bCollidedWithPlayer = true;

		// keep defended increment behavior if desired
		int32 Defended = OverlappingCharacter->GetDefended();
		Defended++;
		OverlappingCharacter->SetDefended(Defended);

		return;
	}

	// Enemy collision: mark flag and destroy the note, nothing else should happen
	ATestEnemyActor* OverlappingEnemy = Cast<ATestEnemyActor>(OtherActor);
	if (OverlappingEnemy != nullptr)
	{
		bCollidedWithEnemy = true;
		this->Destroy();
	}
}

void ANodeActor::MoveLeft()
{
	if (Timeline)
	{
		Timeline->PlayFromStart();
	}
}

void ANodeActor::SetBarLength(double BPM)
{
	BPM = BPM / 240.0f;
	BPM /= 8;

	if (Timeline)
	{
		Timeline->SetPlayRate(static_cast<float>(BPM));
	}
}

// Called when the game starts or when spawned
void ANodeActor::BeginPlay()
{
	Super::BeginPlay();

	// Create a runtime float curve that goes from (time=0, value=1) to (time=1, value=0)
	UCurveFloat* FloatCurve = NewObject<UCurveFloat>(this, TEXT("NoteFloatCurve"));
	if (FloatCurve)
	{
		FloatCurve->FloatCurve.AddKey(0.0f, 1.0f);
		FloatCurve->FloatCurve.AddKey(1.0f, 0.0f);
	}

	if (Timeline && FloatCurve)
	{
		// Bind update callback
		FOnTimelineFloat ProgressCallback;
		ProgressCallback.BindUFunction(this, FName(TEXT("HandleTimelineProgress")));

		Timeline->AddInterpFloat(FloatCurve, ProgressCallback);

		// Bind finished callback so actor is destroyed when timeline finishes
		FOnTimelineEvent FinishedCallback;
		FinishedCallback.BindUFunction(this, FName(TEXT("HandleTimelineFinished")));
		Timeline->SetTimelineFinishedFunc(FinishedCallback);

		Timeline->SetLooping(false);
		Timeline->SetTimelineLength(1.0f); // normalized length: x from 0..1
		Timeline->SetTimelineLengthMode(ETimelineLengthMode::TL_TimelineLength);
	}
}

// Called every frame
void ANodeActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ANodeActor::HandleTimelineProgress(float Value)
{
	float NormalizedX = 0.0f;

	if (Timeline)
	{
		const float Length = Timeline->GetTimelineLength();
		if (Length > KINDA_SMALL_NUMBER)
		{
			NormalizedX = Timeline->GetPlaybackPosition() / Length;
		}
	}

	if (SplineRef)
	{
		const float SplineLength = SplineRef->GetSplineLength();
		const float DistanceAlongSpline = FMath::Clamp(NormalizedX * SplineLength, 0.0f, SplineLength);
		const FVector NewLocation = SplineRef->GetLocationAtDistanceAlongSpline(DistanceAlongSpline, ESplineCoordinateSpace::World);
		SetActorLocation(NewLocation);
	}
}

void ANodeActor::HandleTimelineFinished()
{
	// If we did not collide with the player, apply damage to the player
	if (!bCollidedWithPlayer)
	{
		// find the player character and apply damage
		if (AGameCharacter* PlayerActor = Cast<AGameCharacter>(UGameplayStatics::GetActorOfClass(GetWorld(), AGameCharacter::StaticClass())))
		{
			AGameCharacter* GameChar = Cast<AGameCharacter>(PlayerActor);
			if (GameChar != nullptr)
			{
				GameChar->ApplyDamage(DamageToPlayer);
			}
		}
	}

	// If we did not collide with an enemy, find the first TestEnemyActor and apply damage to its HealthBar1
	if (!bCollidedWithEnemy)
	{
		AActor* Found = UGameplayStatics::GetActorOfClass(GetWorld(), ATestEnemyActor::StaticClass());
		ATestEnemyActor* Enemy = Cast<ATestEnemyActor>(Found);
		if (Enemy != nullptr)
		{
			Enemy->ApplyDamage(DamageToEnemy);
		}
	}

	// Destroy the note
	this->Destroy();
}