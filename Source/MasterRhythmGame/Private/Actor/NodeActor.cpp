// Fill out your copyright notice in the Description page of Project Settings.

#include "Actor/NodeActor.h"
#include "Character/GameCharacter.h"
#include "Components/TimelineComponent.h"
#include "Controller/GameController.h"
#include "Components/SplineComponent.h"
#include "Curves/CurveFloat.h"
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
}

void ANodeActor::OnNoteBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UAudioManagerSubsystem* AudioManager = GetWorld() ? GetWorld()->GetSubsystem<UAudioManagerSubsystem>() : nullptr;

	// Player collision
	AGameCharacter* OverlappingCharacter = Cast<AGameCharacter>(OtherActor);
	if (OverlappingCharacter != nullptr)
	{
		bCollidedWithPlayer = true;

		// Player successfully defended
		// Award score points for successful defense using configured base score
		if (AudioManager != nullptr)
		{
			int32 BaseScore = AudioManager->GetBaseScorePerNote();
			AudioManager->AddScore(BaseScore);
		}

		// Player collided with a note => increment defended and prevent damage.
		int32 Defended = OverlappingCharacter->GetDefended();
		Defended++;
		OverlappingCharacter->SetDefended(Defended);

		//Play Player Block Anim
		OverlappingCharacter->OnBlock();
	}
}

void ANodeActor::MoveLeft()
{
	// Reset collision flags for this new travel.
	bCollidedWithPlayer = false;

	if (Timeline)
	{
		Timeline->ReverseFromEnd();
	}
}

void ANodeActor::SetBarLength(double BPM, int32 NumBars)
{
	float PlayRate = Timeline->GetTimelineLength() / ( ( 240.f / BPM ) * NumBars );

	if (Timeline)
	{
		Timeline->SetPlayRate(static_cast<float>(PlayRate));
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
	UAudioManagerSubsystem* AudioManager = GetWorld() ? GetWorld()->GetSubsystem<UAudioManagerSubsystem>() : nullptr;

	// Player collision now damages enemy; no collision damages player
	if (bCollidedWithPlayer && AudioManager != nullptr)
	{
		// Player successfully hit the note -> damage enemy
		AGameController* GameController = Cast<AGameController>(UGameplayStatics::GetPlayerController(this, 0));
		AActor* Found = UGameplayStatics::GetActorOfClass(GetWorld(), ATestEnemyActor::StaticClass());
		ATestEnemyActor* Enemy = Cast<ATestEnemyActor>(Found);
		if (Enemy != nullptr && GameController)
		{
			UE_LOG(LogTemp, Log, TEXT("ANodeActor::HandleTimelineFinished - Player hit note, applying %d damage to enemy."), DamageToEnemy);
			Enemy->ApplyDamage(DamageToEnemy);
		}
	}
	else if (!bCollidedWithPlayer && AudioManager != nullptr)
	{
		// Player missed the note -> damage player and reset combo
		if (AudioManager)
		{
			AudioManager->ResetCombo();
		}

		if (AGameCharacter* PlayerActor = Cast<AGameCharacter>(UGameplayStatics::GetActorOfClass(GetWorld(), AGameCharacter::StaticClass())))
		{
			UE_LOG(LogTemp, Log, TEXT("ANodeActor::HandleTimelineFinished - Player missed note, applying %d damage to player."), DamageToPlayer);
			PlayerActor->ApplyDamage(DamageToPlayer);
		}
	}
	this->Destroy();
}