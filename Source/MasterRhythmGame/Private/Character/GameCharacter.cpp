// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/GameCharacter.h"
#include "Camera/CameraActor.h"
#include "Controller/GameController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Manager/AudioManagerSubsystem.h"
#include "Components/AudioComponent.h"
#include "HUD/GameHUD.h"
#include "Components/TimelineComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AGameCharacter::AGameCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create AudioComponent and attach it to the actor's RootComponent
	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	if (AudioComponent != nullptr)
	{
		AudioComponent->SetupAttachment(RootComponent);
		AudioComponent->bAutoActivate = false;
	}

	SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	SceneComponent->SetupAttachment(RootComponent);

	GetCharacterMovement()->GravityScale = 0;
	GetCharacterMovement()->bApplyGravityWhileJumping = false;

	// Create and initialize Timeline component so it's not nullptr at runtime
	HealthTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("Health Timeline"));
}

// Called when the game starts or when spawned
void AGameCharacter::BeginPlay()
{
	Super::BeginPlay();

	AGameController* PlayerController = Cast<AGameController>(GEngine->GetFirstLocalPlayerController(GetWorld()));

	if (PlayerController != nullptr)
	{
		// Resolve TSoftObjectPtr<ACameraActor> to a live ACameraActor*
		ACameraActor* ResolvedCamera = nullptr;

		// If already loaded, Get() will return the pointer; otherwise attempt a synchronous load
		if (CameraActor.IsValid())
		{
			ResolvedCamera = CameraActor.Get();
		}
		else if (CameraActor.ToSoftObjectPath().IsValid())
		{
			// LoadSynchronous will load the asset if it's not already loaded and return the pointer.
			ResolvedCamera = CameraActor.LoadSynchronous();
		}

		if (ResolvedCamera != nullptr)
		{
			// Use float literals and existing blend enum as before
			PlayerController->SetViewTargetWithBlend(ResolvedCamera, 0.0f, VTBlend_Linear, 0.0f, false);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("AGameCharacter::BeginPlay - CameraActor soft reference could not be resolved."));
		}

		GameHUD = Cast<AGameHUD>(PlayerController->GetHUD());
	}
	SetupHealthTimeline();
	HealthTimeline->PlayFromStart();
}

// Called every frame
void AGameCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AGameCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AGameCharacter::CreateAndStartQuartzClock(float InBpm)
{
	UAudioManagerSubsystem* AudioManager = GetWorld()->GetSubsystem<UAudioManagerSubsystem>();
	if (AudioManager != nullptr)
	{
		AudioManager->InitSubsystem();
		AudioManager->ClockHandleInit(FName(TEXT("PlayerClock")));
		FQuartzQuantizationBoundary QuantBoundary(EQuartzCommandQuantization::Bar, 1.0f, EQuarztQuantizationReference::BarRelative, true);
		FOnQuartzCommandEventBP Delegate;
		AudioManager->SetBeatsPerMinute(InBpm, QuantBoundary, Delegate);
		AudioManager->StartClock();
		AudioManager->PlayQuantized(AudioComponent);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("AGameCharacter::BeginPlay - UAudioManagerSubsystem not available."));
	}
}

void AGameCharacter::ApplyDamage(int32 DamageAmount)
{
	int32 NewHealth = CalcHealth(DamageAmount);
	UE_LOG(LogTemp, Warning, TEXT("AGameCharacter::ApplyDamage - Damage=%d Health=%d"), DamageAmount, NewHealth);

	if (GetHealth() <= 0)
	{
		if (GameHUD != nullptr)
		{
			AGameController* PlayerController = Cast<AGameController>(GEngine->GetFirstLocalPlayerController(GetWorld()));
			GameHUD->GetDeathWidgetInstance()->SetVisibility(ESlateVisibility::Visible);
			PlayerController->SetControllerState(EControllerStateGame::DeathMenu);
			// Toggle pause state
			bool bCurrentlyPaused = UGameplayStatics::IsGamePaused(GetWorld());
			UGameplayStatics::SetGamePaused(GetWorld(), !bCurrentlyPaused);

			if (bCurrentlyPaused == false)
			{
				if (GameHUD != nullptr)
				{
					GameHUD->GetPauseMenuInstance()->SetVisibility(ESlateVisibility::Visible);
				}
				// Show mouse cursor and switch to GameAndUI input so widgets receive focus
				PlayerController->bShowMouseCursor = true;
				FInputModeGameAndUI InputMode;
				InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
				PlayerController->SetInputMode(InputMode);
			}
		}
	}
}
	
int32 AGameCharacter::CalcHealth(int32 InHealth)
{
	Health -= InHealth;
	if (GameHUD != nullptr)
	{
		GameHUD->GetMainGameInstance()->SetHealthPlayer(Health);
	}
	return Health;
}

// Timeline setup and callbacks
void AGameCharacter::SetupHealthTimeline()
{
	if (HealthTimeline == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("AGameCharacter::SetupHealthTimeline - HealthTimeline is null."));
		return;
	}

	// Determine a single-frame duration. If world delta is available, use it; otherwise default to 1/60s.
	float FrameTime = 1.0f / 60.0f;
	if (GetWorld() && GetWorld()->GetDeltaSeconds() > 0.0f)
	{
		FrameTime = GetWorld()->GetDeltaSeconds();
	}

	// Create a short runtime curve that goes from 0 to 1 across one frame.
	if (HealthCurve == nullptr)
	{
		HealthCurve = NewObject<UCurveFloat>(this, TEXT("HealthCurve_Dyn"));
		if (HealthCurve != nullptr)
		{
			HealthCurve->FloatCurve.AddKey(0.0f, 0.0f);
			HealthCurve->FloatCurve.AddKey(FrameTime, 1.0f);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("AGameCharacter::SetupHealthTimeline - Failed to create HealthCurve."));
			return;
		}
	}

	// Bind tick delegate (no-op, but required to drive timeline) and finished delegate.
	FOnTimelineFloat TickDelegate;
	TickDelegate.BindUFunction(this, FName("OnHealthTimelineTick"));
	HealthTimeline->AddInterpFloat(HealthCurve, TickDelegate);

	FOnTimelineEvent FinishDelegate;
	FinishDelegate.BindUFunction(this, FName("OnHealthTimelineFinished"));
	HealthTimeline->SetTimelineFinishedFunc(FinishDelegate);

	HealthTimeline->SetLooping(false);
	HealthTimeline->SetTimelineLength(FrameTime);
	HealthTimeline->SetTimelineLengthMode(ETimelineLengthMode::TL_TimelineLength);

	// Ensure component is registered and start it.
	if (!HealthTimeline->IsRegistered())
	{
		HealthTimeline->RegisterComponent();
	}
	HealthTimeline->PlayFromStart();
}

void AGameCharacter::OnHealthTimelineTick(float Value)
{
	// Intentionally left empty � we only need the finished event one frame later.
}

void AGameCharacter::OnHealthTimelineFinished()
{
	if (GameHUD != nullptr && GameHUD->GetMainGameInstance() != nullptr)
	{
		GameHUD->GetMainGameInstance()->SetMaxHealthPlayer(Health);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("AGameCharacter::OnHealthTimelineFinished - GameHUD or MainGameInstance not available."));
	}
}

void AGameCharacter::PlayNote(int32 Note)
{
	if (AudioComponent && AudioComponent->GetSound())
	{
		if (!AudioComponent->IsPlaying())
		{
			AudioComponent->Play();
		}
		AudioComponent->SetTriggerParameter("MIDINoteOn");
		AudioComponent->SetIntParameter("Note", Note);
	}
	
}

void AGameCharacter::StopNote()
{
	if (AudioComponent && AudioComponent->GetSound())
	{
		if (!AudioComponent->IsPlaying())
		{
			AudioComponent->Play();
		}
		AudioComponent->SetTriggerParameter("MIDINoteOff");
	}
	
}