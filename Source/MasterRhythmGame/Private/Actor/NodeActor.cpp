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
#include "TimerManager.h"
#include "Manager/AudioManagerSubsystem.h"

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

	// default movement direction (will be set by MoveLeft/MoveRight when node is sent)
	bMovingLeft = false;
}

void ANodeActor::OnNoteBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UAudioManagerSubsystem* AudioManager = GetWorld() ? GetWorld()->GetSubsystem<UAudioManagerSubsystem>() : nullptr;

	// Player collision
	AGameCharacter* OverlappingCharacter = Cast<AGameCharacter>(OtherActor);
	if (OverlappingCharacter != nullptr)
	{
		// If the node is moving left, it targets the player side.
		if (bMovingLeft)
		{
			// If player is attacking, treat as a collision that prevents damage but do not increment defended.
			if (AudioManager != nullptr && AudioManager->GetPlayerCanAttack())
			{
				bCollidedWithPlayer = true;
				return;
			}

			// Player collided with a left-moving node => increment defended and prevent damage.
			bCollidedWithPlayer = true;
			int32 Defended = OverlappingCharacter->GetDefended();
			Defended++;
			OverlappingCharacter->SetDefended(Defended);
			return;
		}

		// If node is moving right (targets enemy), a player overlap should not cause player damage.
		// Mark as collided to ensure player won't be damaged by this note later.
		bCollidedWithPlayer = true;
	}

	// Enemy collision
	ATestEnemyActor* OverlappingEnemy = Cast<ATestEnemyActor>(OtherActor);
	if (OverlappingEnemy != nullptr)
	{
		// If the node is moving right, it targets the enemy side.
		if (!bMovingLeft)
		{
			// If enemy is attacking, just mark collided to prevent damage.
			if (AudioManager != nullptr && AudioManager->GetEnemyCanAttack())
			{
				bCollidedWithEnemy = true;
				return;
			}

			// Per requirement: when MoveRight executed and enemy collides with the note, nothing happens.
			// Mark collided so the enemy will not receive damage from this note when it finishes.
			bCollidedWithEnemy = true;
			return;
		}

		// If node is moving left (targets player), preserve original behavior for enemy overlaps:
		// if enemy overlaps with a left-moving node, mark and destroy.
		if (AudioManager != nullptr && AudioManager->GetEnemyCanAttack())
		{
			bCollidedWithEnemy = true;
			return;
		}

		bCollidedWithEnemy = true;
	}
}

void ANodeActor::MoveLeft()
{
	// This node will travel left and therefore may damage the player (unless collided).
	bMovingLeft = true;

	// Reset collision flags for this new travel.
	bCollidedWithPlayer = false;
	bCollidedWithEnemy = false;

	if (Timeline)
	{
		Timeline->PlayFromStart();
	}
}

void ANodeActor::MoveRight()
{
	// This node will travel right and therefore may damage the enemy (unless collided).
	bMovingLeft = false;

	// Reset collision flags for this new travel.
	bCollidedWithPlayer = false;
	bCollidedWithEnemy = false;

	if (Timeline)
	{
		Timeline->ReverseFromEnd();
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
	UAudioManagerSubsystem* AudioManager = GetWorld() ? GetWorld()->GetSubsystem<UAudioManagerSubsystem>() : nullptr;

	if (bMovingLeft)
	{
		if (!bCollidedWithPlayer && AudioManager != nullptr && !AudioManager->GetPlayerCanAttack())
		{
			// find the player character and apply damage
			if (AGameCharacter* PlayerActor = Cast<AGameCharacter>(UGameplayStatics::GetActorOfClass(GetWorld(), AGameCharacter::StaticClass())))
			{
				AGameCharacter* GameChar = Cast<AGameCharacter>(PlayerActor);
				if (GameChar != nullptr)
				{
					UE_LOG(LogTemp, Log, TEXT("ANodeActor::HandleTimelineFinished - Applying %d damage to player."), DamageToPlayer);
					GameChar->ApplyDamage(DamageToPlayer);
				}
			}
		}
	}
	else
	{
		if (!bCollidedWithEnemy && AudioManager != nullptr && !AudioManager->GetEnemyCanAttack())
		{
			AActor* Found = UGameplayStatics::GetActorOfClass(GetWorld(), ATestEnemyActor::StaticClass());
			ATestEnemyActor* Enemy = Cast<ATestEnemyActor>(Found);
			if (Enemy != nullptr)
			{
				UE_LOG(LogTemp, Log, TEXT("ANodeActor::HandleTimelineFinished - Applying %d damage to enemy."), DamageToEnemy);
				Enemy->ApplyDamage(DamageToEnemy);
			}
		}
	}

	if (GetWorld())
	{
		const double DelaySeconds = LatencyOffsetMs / 1000.0;

		// Ensure any previous timer is cleared, then set the new timer
		GetWorld()->GetTimerManager().ClearTimer(DestroyTimerHandle);
		GetWorld()->GetTimerManager().SetTimer(DestroyTimerHandle, this, &ANodeActor::OnDelayedDestroy, DelaySeconds, false);
	}
}

void ANodeActor::OnDelayedDestroy()
{
	this->Destroy();
}