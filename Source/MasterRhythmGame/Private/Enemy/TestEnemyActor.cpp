// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/TestEnemyActor.h"
#include "Actor/NodeActor.h"
#include "Manager/AudioManagerSubsystem.h"
#include "Components/AudioComponent.h"


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

	// Create AudioComponent and attach it to the actor's RootComponent
	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	if (AudioComponent != nullptr)
	{
		AudioComponent->SetupAttachment(RootComponent);
		AudioComponent->bAutoActivate = false;
	}

	// Ensure the enemy starts in Part 1
	SetCurrentPart(1);
}

int32 ATestEnemyActor::CalcHealth1(int32 Value)
{
	HealthBar1 = FMath::Max(HealthBar1 - Value, 0);
	return HealthBar1;
}

int32 ATestEnemyActor::CalcHealth2(int32 Value)
{
	HealthBar2 = FMath::Max(HealthBar2 - Value, 0);
	return HealthBar2;
}

int32 ATestEnemyActor::CalcHealth3(int32 Value)
{
	HealthBar3 = FMath::Max(HealthBar3 - Value, 0);
	return HealthBar3;
}

int32 ATestEnemyActor::CalcHealth4(int32 Value)
{
	HealthBar4 = FMath::Max(HealthBar4 - Value, 0);
	return HealthBar4;
}

int32 ATestEnemyActor::CalcHealth5(int32 Value)
{
	HealthBar5 = FMath::Max(HealthBar5 - Value, 0);
	return HealthBar5;
}

int32 ATestEnemyActor::CalcHealth6(int32 Value)
{
	HealthBar6 = FMath::Max(HealthBar6 - Value, 0);
	return HealthBar6;
}

int32 ATestEnemyActor::CalcHealth7(int32 Value)
{
	HealthBar7 = FMath::Max(HealthBar7 - Value, 0);
	return HealthBar7;
}

int32 ATestEnemyActor::CalcHealth8(int32 Value)
{
	HealthBar8 = FMath::Max(HealthBar8 - Value, 0);
	return HealthBar8;
}

void ATestEnemyActor::ApplyDamage(int32 DamageValue)
{
	// Apply damage only to the health bar that corresponds to CurrentPart.
	// If that health bar drops to zero or below, advance to the next part (up to 8).
	switch (GetCurrentPart())
	{
		case 1:
		{
			SetHealth1(CalcHealth1(DamageValue));
			if (GetHealth1() <= 0)
			{
				SetCurrentPart(GetCurrentPart() + 1);
			}
			break;
		}
		case 2:
		{
			SetHealth2(CalcHealth2(DamageValue));
			if (GetHealth2() <= 0)
			{
				SetCurrentPart(GetCurrentPart() + 1);
			}
			break;
		}
		case 3:
		{
			SetHealth3(CalcHealth3(DamageValue));
			if (GetHealth3() <= 0)
			{
				SetCurrentPart(GetCurrentPart() + 1);
			}
			break;
		}
		case 4:
		{
			SetHealth4(CalcHealth4(DamageValue));
			if (GetHealth4() <= 0)
			{
				SetCurrentPart(GetCurrentPart() + 1);
			}
			break;
		}
		case 5:
		{
			SetHealth5(CalcHealth5(DamageValue));
			if (GetHealth5() <= 0)
			{
				SetCurrentPart(GetCurrentPart() + 1);
			}
			break;
		}
		case 6:
		{
			SetHealth6(CalcHealth6(DamageValue));
			if (GetHealth6() <= 0)
			{
				SetCurrentPart(GetCurrentPart() + 1);
			}
			break;
		}
		case 7:
		{
			SetHealth7(CalcHealth7(DamageValue));
			if (GetHealth7() <= 0)
			{
				SetCurrentPart(GetCurrentPart() + 1);
			}
			break;
		}
		case 8:
		{
			SetHealth8(CalcHealth8(DamageValue));
			break;
		}
		default:
		{
			// Safety: clamp to valid range
			SetCurrentPart(FMath::Clamp(GetCurrentPart(), 1, 8));
			break;
		}
	}
}

void ATestEnemyActor::CreateAndStartQuartzClock(int32 InBPM)
{
	UAudioManagerSubsystem* AudioManager = GetWorld()->GetSubsystem<UAudioManagerSubsystem>();
	if (AudioManager != nullptr)
	{
		AudioManager->InitSubsystem();
		AudioManager->ClockHandleInit(FName(TEXT("PlayerClock")));
		FQuartzQuantizationBoundary QuantBoundary(EQuartzCommandQuantization::Bar, 1.0f, EQuarztQuantizationReference::BarRelative, true);
		FOnQuartzCommandEventBP Delegate;
		AudioManager->SetBeatsPerMinute(BPM, QuantBoundary, Delegate);
		AudioManager->StartClock();
		AudioManager->PlayQuantized(AudioComponent);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("AGameCharacter::BeginPlay - UAudioManagerSubsystem not available."));
	}
}

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

	CreateAndStartQuartzClock(BPM);
}

// Called every frame
void ATestEnemyActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}