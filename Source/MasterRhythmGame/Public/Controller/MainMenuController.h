// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputMappingContext.h"
#include "MIDIDevice/Public/MIDIDeviceInputController.h"
#include "GameFramework/PlayerController.h"
#include "HUD/MainMenuHUD.h"
#include "MainMenuController.generated.h"

enum class EControllerState
{
	MainMenu,
	CreditsMenu,
	AudioMenu,
	GraphicsMenu,
	LoadMenu,
	SettingMenu
};

/**
 * 
 */
UCLASS()
class MASTERRHYTHMGAME_API AMainMenuController : public APlayerController
{
	GENERATED_BODY()
public:
	AMainMenuController();

protected:

	virtual void BeginPlay() override;

	// Sets the Input Component
	virtual void SetupInputComponent() override;

	//Initialize the Mapping Context and the Input Actions
	void InitInputAction();

	//Initialize the MIDI Controller
	void InitMidi();

	// Finds MIDI Devices and logs them
	static void DisplayMidiDevicesIDs();

	void CreateMidiController();

	// Called every frame
	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION()
	void HandleNoteOn(UMIDIDeviceInputController* MIDIDeviceController, int32 Timestamp, int32 Channel, int32 Note, int32 Velocity);

	UFUNCTION()
	void HandleNoteOff(UMIDIDeviceInputController* MIDIDeviceController, int32 Timestamp, int32 Channel, int32 Note, int32 Velocity);

	UFUNCTION()
	void HandlePitchBend(UMIDIDeviceInputController* MIDIDeviceController, int32 Timestamp, int32 Channel, int32 Pitch);
	
	UFUNCTION()
	void HandleAftertouch(UMIDIDeviceInputController* MIDIDeviceController, int32 Timestamp, int32 Channel, int32 Note, int32 Amount);

	UFUNCTION()
	void HandleControlChange(UMIDIDeviceInputController* MIDIDeviceController, int32 Timestamp, int32 Channel, int32 Type, int32 Value);

	void MainMenuControl(int32 Note);

	void AudioMenuControl(int32 Note);

	void SettingMenuControl(int32 Note);

	void CreditMenuControl(int32 Note);

	void GraphicMenuControl(int32 Note);

	void LoadMenuControl(int32 Note);

	void MainMenuSwitchButton(int32 InMainMenuIndex);

private:
	const FString MusicName = FString(TEXT("Music"));

	UPROPERTY(VisibleAnywhere, DisplayName = "Music", Category = "Components")
	TObjectPtr<class UAudioComponent> MusicComponent { nullptr };

	UPROPERTY(VisibleAnywhere, DisplayName = "UI SFX", Category = "Components")
	TObjectPtr<class UAudioComponent> USfxComponent { nullptr };

	UPROPERTY(VisibleAnywhere, DisplayName = "Midi Controller", Category = "Components")
	TObjectPtr<class UMIDIDeviceInputController> MidiController { nullptr };

	UPROPERTY()
	TObjectPtr<AMainMenuHUD> MainMenuHud { nullptr };

	UPROPERTY(VisibleAnywhere, DisplayName = "Audio Component Auto Activation", Category = "Components")
	bool bAutoActivate = false;

	UPROPERTY(VisibleAnywhere, DisplayName = "MainMenuIndex", Category = "Components")
	int32 MainMenuIndex { 0 };

	UPROPERTY(VisibleAnywhere, DisplayName = "AudioMenuIndex", Category = "Components")
	int32 AudioMenuIndex { 0 };

	UPROPERTY(VisibleAnywhere, DisplayName = "SettingMenuIndex", Category = "Components")
	int32 SettingMenuIndex { 0 };

	UPROPERTY(VisibleAnywhere, DisplayName = "SettingMenuIndex", Category = "Components")
	int32 CreditMenuIndex { 0 };

	UPROPERTY(VisibleAnywhere, DisplayName = "GraphicMenuIndex", Category = "Components")
	int32 GraphicMenuIndex { 0 };

	UPROPERTY(VisibleAnywhere, DisplayName = "LoadMenuIndex", Category = "Components")
	int32 LoadMenuIndex { 0 };

	EControllerState ControllerState{ EControllerState::MainMenu };



#pragma region InputAction

	UPROPERTY(EditAnywhere, DisplayName = "MappingContext", Category = "InputAction", meta = (AllowPrivateAccess = true))
	UInputMappingContext* DefaultMappingContext { nullptr };


#pragma endregion
};
