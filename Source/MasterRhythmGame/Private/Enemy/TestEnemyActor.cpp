// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemy/TestEnemyActor.h"
#include "Actor/NodeActor.h"
#include "Manager/AudioManagerSubsystem.h"
#include "Components/AudioComponent.h"
#include "HUD/GameHUD.h"
#include "Kismet/GameplayStatics.h"
#include "Components/TimelineComponent.h"
#include "Math/UnrealMathUtility.h"

// Sets default values
ATestEnemyActor::ATestEnemyActor()
{
	// Set this actor to call Tick() every frame.
	PrimaryActorTick.bCanEverTick = true;

	DefaultSceneRoot = CreateDefaultSubobject<USceneComponent>("DefaultSceneRoot");
	Scene = CreateDefaultSubobject<USceneComponent>("Scene");

	SetRootComponent(DefaultSceneRoot);
	Scene->SetupAttachment(RootComponent);

	// Create AudioComponent and attach it to the actor's RootComponent
	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	if (AudioComponent != nullptr)
	{
		AudioComponent->SetupAttachment(RootComponent);
		AudioComponent->bAutoActivate = false;
	}
}

void ATestEnemyActor::Init(ULevelData* LevelDataIn)
{
	LevelData = LevelDataIn;

	BPM = LevelData->BPM;
	HealthBar1 = (LevelData->NumNotesMelody1 * LevelData->HealthPointsPerNote);
	HealthBar2 = (LevelData->NumNotesMelody2 * LevelData->HealthPointsPerNote);
	HealthBar3 = (LevelData->NumNotesMelody3 * LevelData->HealthPointsPerNote);
	HealthBar4 = (LevelData->NumNotesMelody4 * LevelData->HealthPointsPerNote);
	HealthBar5 = (LevelData->NumNotesMelody5 * LevelData->HealthPointsPerNote);

	if (AudioComponent)
	{
		AudioComponent->SetSound(LevelData->Music);
	}
	

	OnInit();
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

void ATestEnemyActor::ApplyDamage(int32 DamageValue)
{
	UAudioManagerSubsystem* AudioManager = GetWorld()->GetSubsystem<UAudioManagerSubsystem>();

	if (AudioManager != nullptr)
	{
		auto PartName = AudioManager->GetPartNameFix();

		if (PartName == "Part1End")
		{
			SetHealth1(CalcHealth1(DamageValue));
			// update UI
			if (GameHUD != nullptr && GameHUD->GetMainGameInstance() != nullptr)
			{
				GameHUD->GetMainGameInstance()->SetHealthEnemy1(GetHealth1());
			}
		}
		else if (PartName == "Part2End")
		{
			SetHealth2(CalcHealth2(DamageValue));
			if (GameHUD != nullptr && GameHUD->GetMainGameInstance() != nullptr)
			{
				GameHUD->GetMainGameInstance()->SetHealthEnemy2(GetHealth2());
			}
		}
		else if (PartName == "Part3End")
		{
			SetHealth3(CalcHealth3(DamageValue));
			if (GameHUD != nullptr && GameHUD->GetMainGameInstance() != nullptr)
			{
				GameHUD->GetMainGameInstance()->SetHealthEnemy3(GetHealth3());
			}
		}
		else if (PartName == "Part4End")
		{
			SetHealth4(CalcHealth4(DamageValue));
			if (GameHUD != nullptr && GameHUD->GetMainGameInstance() != nullptr)
			{
				GameHUD->GetMainGameInstance()->SetHealthEnemy4(GetHealth4());
			}
		}
		else if (PartName == "Part5End")
		{
			SetHealth5(CalcHealth5(DamageValue));
			if (GameHUD != nullptr && GameHUD->GetMainGameInstance() != nullptr)
			{
				GameHUD->GetMainGameInstance()->SetHealthEnemy5(GetHealth5());
			}
		}
		// TODO: If enemy health reaches zero do sth


		//Play Hit Anim
		PlayHitAnimation();
	}
}

void ATestEnemyActor::Attack(int32 InBPM)
{
	auto ActorLocation = AActor::GetActorLocation();
	auto SceneLocation = Scene->GetRelativeLocation();
	auto SpawnLocation = ActorLocation + SceneLocation;
	auto Note = GetWorld()->SpawnActor<ANodeActor>(NodeActor, SpawnLocation, GetActorRotation());

	if (Note != nullptr)
	{
		Note->SetBarLength(InBPM);
		Note->SetSplineRef(SplineRef);
		Note->MoveLeft();

		//Play Attack Anim
		PlayAttackAnimation();
	}
}

// Called when the game starts or when spawned
void ATestEnemyActor::BeginPlay()
{
	Super::BeginPlay();

	// Properly get and cast the HUD here (world and player controller are valid in BeginPlay).
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0))
	{
		GameHUD = Cast<AGameHUD>(PC->GetHUD());
	}

	if (GetWorld())
	{
		GetWorld()->GetTimerManager().SetTimerForNextTick(this, &ATestEnemyActor::SetupHUD);
	}
}

// Called every frame
void ATestEnemyActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATestEnemyActor::SetupHUD()
{
	if (GameHUD != nullptr && GameHUD->GetMainGameInstance() != nullptr)
	{
		// Initialize the UI with the current actor health values.
		// SetMaxHealthEnemyN will show/hide bars depending on max (0 = hidden).
		GameHUD->GetMainGameInstance()->SetMaxHealthEnemy1(GetHealth1());
		GameHUD->GetMainGameInstance()->SetMaxHealthEnemy2(GetHealth2());
		GameHUD->GetMainGameInstance()->SetMaxHealthEnemy3(GetHealth3());
		GameHUD->GetMainGameInstance()->SetMaxHealthEnemy4(GetHealth4());
		GameHUD->GetMainGameInstance()->SetMaxHealthEnemy5(GetHealth5());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("ATestEnemyActor::SetupHUD - GameHUD or MainGameInstance not available."));
	}
}