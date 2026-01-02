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

	// Create Timeline component so it's available at runtime
	HealthTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("Health Timeline"));
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
		// TODO: If enemy health reaches zero do sth
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
	auto ActorLocation = AActor::GetActorLocation();
	auto SceneLocation = Scene->GetRelativeLocation();
	auto SpawnLocation = ActorLocation + SceneLocation;
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

	// Properly get and cast the HUD here (world and player controller are valid in BeginPlay).
	if (APlayerController* PC = UGameplayStatics::GetPlayerController(this, 0))
	{
		GameHUD = Cast<AGameHUD>(PC->GetHUD());
	}

	CreateAndStartQuartzClock(BPM);

	// Setup timeline that will execute one frame later to initialize enemy health UI
	SetupHealthTimeline();
	if (HealthTimeline != nullptr)
	{
		HealthTimeline->PlayFromStart();
	}
}

// Called every frame
void ATestEnemyActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATestEnemyActor::SetupHealthTimeline()
{
	if (HealthTimeline == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("ATestEnemyActor::SetupHealthTimeline - HealthTimeline is null."));
		return;
	}

	// Determine a single-frame duration. Default to 1/60s if delta not available.
	float FrameTime = 1.0f / 60.0f;
	if (GetWorld() && GetWorld()->GetDeltaSeconds() > 0.0f)
	{
		FrameTime = GetWorld()->GetDeltaSeconds();
	}

	// Create a short runtime curve that goes from 0 to 1 across one frame.
	if (HealthCurve == nullptr)
	{
		HealthCurve = NewObject<UCurveFloat>(this, TEXT("EnemyHealthCurve_Dyn"));
		if (HealthCurve != nullptr)
		{
			HealthCurve->FloatCurve.AddKey(0.0f, 0.0f);
			HealthCurve->FloatCurve.AddKey(FrameTime, 1.0f);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("ATestEnemyActor::SetupHealthTimeline - Failed to create HealthCurve."));
			return;
		}
	}

	// Bind tick delegate (no-op but required) and finished delegate.
	FOnTimelineFloat TickDelegate;
	TickDelegate.BindUFunction(this, FName("OnHealthTimelineTick"));
	HealthTimeline->AddInterpFloat(HealthCurve, TickDelegate);

	FOnTimelineEvent FinishDelegate;
	FinishDelegate.BindUFunction(this, FName("OnHealthTimelineFinished"));
	HealthTimeline->SetTimelineFinishedFunc(FinishDelegate);

	HealthTimeline->SetLooping(false);
	HealthTimeline->SetTimelineLength(FrameTime);
	HealthTimeline->SetTimelineLengthMode(ETimelineLengthMode::TL_TimelineLength);

	if (!HealthTimeline->IsRegistered())
	{
		HealthTimeline->RegisterComponent();
	}
}

void ATestEnemyActor::OnHealthTimelineTick(float Value)
{
	// no action required on tick; we only need the finished callback after one frame
}

void ATestEnemyActor::OnHealthTimelineFinished()
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
		GameHUD->GetMainGameInstance()->SetMaxHealthEnemy6(GetHealth6());
		GameHUD->GetMainGameInstance()->SetMaxHealthEnemy7(GetHealth7());
		GameHUD->GetMainGameInstance()->SetMaxHealthEnemy8(GetHealth8());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("ATestEnemyActor::OnHealthTimelineFinished - GameHUD or MainGameInstance not available."));
	}
}