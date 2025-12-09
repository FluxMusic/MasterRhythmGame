// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/GameCharacter.h"
#include "Camera/CameraActor.h"
#include "Controller/GameController.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AGameCharacter::AGameCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

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
	}
	//CreateAndStartQuartzClock(Bpm);
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

	FQuartzClockSettings Settings;
	UQuartzSubsystem* Subsystem = UQuartzSubsystem::Get(GetWorld());
	if (!Subsystem)
	{
		UE_LOG(LogTemp, Warning, TEXT("AGameCharacter::CreateAndStartQuartzClock - UQuartzSubsystem not available."));
		return;
	}

	// Create a new clock and store the handle (raw pointer for API calls)
	QuartzClockHandle = Subsystem->CreateNewClock(this, FName("Player"), Settings);
	if (!QuartzClockHandle)
	{
		UE_LOG(LogTemp, Warning, TEXT("AGameCharacter::CreateAndStartQuartzClock - failed to create clock."));
		return;
	}

	// Prepare valid parameters for SetBeatsPerMinute
	FQuartzQuantizationBoundary QuantBoundary(EQuartzCommandQuantization::Beat, 1.0f, EQuarztQuantizationReference::BarRelative, true);
	FOnQuartzCommandEventBP EmptyDelegate; // no-op delegate

	// Set BPM (passes raw pointer by reference)
	QuartzClockHandle->SetBeatsPerMinute(this, QuantBoundary, EmptyDelegate, QuartzClockHandle, InBpm);

	// Start the clock (passes the same raw pointer by reference)
	QuartzClockHandle->StartClock(this, QuartzClockHandle);
}

void AGameCharacter::ApplyDamage(int32 DamageAmount)
{
	CalcHealth(DamageAmount);
}

int32 AGameCharacter::CalcHealth(int32 InHealth)
{
	Health--;
	Health += InHealth;
	return Health;
}
