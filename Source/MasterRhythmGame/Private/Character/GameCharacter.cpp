// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/GameCharacter.h"
#include "Camera/CameraActor.h"
#include "Controller/GameController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Manager/AudioManagerSubsystem.h"
#include "Components/AudioComponent.h"
#include "HUD/GameHUD.h"
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

		if (GameHUD != nullptr && GameHUD->GetMainGameInstance() != nullptr)
		{
			GameHUD->GetMainGameInstance()->SetHealthPlayer(Health);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("Fotze"));
		}
	}
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
			GameHUD->GetDeathWidgetInstance()->SetVisibility(ESlateVisibility::Visible);
			// Toggle pause state
			bool bCurrentlyPaused = UGameplayStatics::IsGamePaused(GetWorld());
			UGameplayStatics::SetGamePaused(GetWorld(), !bCurrentlyPaused);

			if (bCurrentlyPaused == false)
			{
				if (GameHUD != nullptr)
				{
					GameHUD->GetPauseMenuInstance()->SetVisibility(ESlateVisibility::Visible);
				}
				AGameController* PlayerController = Cast<AGameController>(GEngine->GetFirstLocalPlayerController(GetWorld()));



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
