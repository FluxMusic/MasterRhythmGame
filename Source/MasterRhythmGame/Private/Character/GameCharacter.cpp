// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/GameCharacter.h"
#include "Camera/CameraActor.h"
#include "Controller/GameController.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Manager/AudioManagerSubsystem.h"
#include "Components/AudioComponent.h"
#include "HUD/GameHUD.h"
#include "Components/TimelineComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SplineComponent.h"
#include "Actor/SplineActor.h"

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

	if (GetCapsuleComponent())
	{
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	
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

	//SetupGameHUD next frame
	if (GetWorld())
	{
		GetWorld()->GetTimerManager().SetTimerForNextTick(this, &AGameCharacter::SetupHUD);
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

void AGameCharacter::ApplyDamage(int32 DamageAmount)
{
	int32 NewHealth = CalcHealth(DamageAmount);
	UE_LOG(LogTemp, Warning, TEXT("AGameCharacter::ApplyDamage - Damage=%d Health=%d"), DamageAmount, NewHealth);

	//Play Hit Anim
	PlayHitAnimation();

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
	//return Health;
	return 100;
}

void AGameCharacter::SetupHUD()
{
	if (GameHUD != nullptr && GameHUD->GetMainGameInstance() != nullptr)
	{
		GameHUD->GetMainGameInstance()->SetMaxHealthPlayer(Health);

		if (SplineRef)
		{
			TArray<int32> ScaleDegrees = Cast<ASplineActor>(SplineRef->GetOwner())->GetScaleDegrees();
			
			GameHUD->GetMainGameInstance()->GetKeyboardWidget()->SetScaleHighlights(ScaleDegrees);
		}
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