// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/MainPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputSubsystemInterface.h"
#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"
#include "InputAction.h"

// Sets default values
AMainPlayerController::AMainPlayerController()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	InitInputAction();

	//MusicComponent = CreateDefaultSubobject<UAudioComponent>(*Music_Name);
}

//Called when the game starts or when spawned
void AMainPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		// add mapping context
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}
}

//Binds the Input Actions to their related Methods
void AMainPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent))
	{
		// All Input Actions in here
	}
}

//Initialize the Mapping Context and the Input Actions
void AMainPlayerController::InitInputAction()
{
	// Initialize everything
	DefaultMappingContext = LoadObject<UInputMappingContext>(nullptr, TEXT("/Game/Input/IMC_Character.IMC_Character"));

}

//Updates every frame
void AMainPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}
