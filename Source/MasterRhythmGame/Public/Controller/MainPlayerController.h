// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputMappingContext.h"
//#include "MIDIDevice/Public/MIDIDeviceInputController.h"
//#include "MIDIDevice/Public/MIDIDeviceManager.h"
#include "GameFramework/PlayerController.h"
#include "MainPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class MASTERRHYTHMGAME_API AMainPlayerController : public APlayerController
{
	GENERATED_BODY()
public:
	AMainPlayerController();

protected:

	virtual void BeginPlay() override;

	// Sets the Input Component
	virtual void SetupInputComponent() override;

	//Initialize the Mapping Context and the Input Actions
	void InitInputAction(void);

	////Initialize the MIDI Controller
	//void InitMidi(void);

	//// Finds MIDI Devices and logs them
	//void DisplayMidiDevicesIDs(void);

	//void CreateMidiController(void);

	// Called every frame
	void Tick(float DeltaSeconds) override;

private:
	const FString Music_Name = FString(TEXT("Music"));

	UPROPERTY(VisibleAnywhere, DisplayName = "Music", Category = "Components")
	TObjectPtr<class UAudioComponent> MusicComponent = nullptr;

	UPROPERTY(VisibleAnywhere, DisplayName = "UI SFX", Category = "Components")
	TObjectPtr<class UAudioComponent> USfxComponent = nullptr;

	//UPROPERTY(VisibleAnywhere, DisplayName = "Midi Controller", Category = "Components")
	//TObjectPtr<class UMIDIDeviceInputController> MidiController = nullptr;

	UPROPERTY(VisibleAnywhere, DisplayName = "Audio Component Auto Activation", Category = "Components")
	bool bAutoActivate = false;

#pragma region InputAction

	UPROPERTY(EditAnywhere, DisplayName = "MappingContext", Category = "InputAction", meta = (AllowPrivateAccess = true))
	UInputMappingContext* DefaultMappingContext = nullptr;


#pragma endregion
};
