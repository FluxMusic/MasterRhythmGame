// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../CoreTypes.h"
#include "MIDIDevice/Public/MIDIDeviceInputController.h"
#include "GameFramework/PlayerController.h"
#include "HUD/MainMenuHUD.h"
#include "MainMenuController.generated.h"

/**
 * 
 */
UCLASS()
class MASTERRHYTHMGAME_API AMainMenuController : public APlayerController
{
	GENERATED_BODY()
public:
	AMainMenuController();

	EControllerState GetControllerState() const { return ControllerState; }

	void SetControllerState(EControllerState NewState) { ControllerState = NewState; }

protected:

	virtual void BeginPlay() override;

	// Sets the Input Component
	virtual void SetupInputComponent() override;

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

	void MainMenuControl(ENote Note);

	void AudioMenuControl(ENote Note);

	void SettingMenuControl(ENote Note);

	void CreditMenuControl(ENote Note);

	void GraphicMenuControl(ENote Note);

	void LoadMenuControl(ENote Note);

	// Now use strongly-typed enums for the switch helpers
	void MainMenuSwitchButton(EMainMenuItem InMainMenuItem);

	void MainSettingSwitchButton(EMainSettingItem InMainSettingItem);

	void GraphicMenuSwitchButton(EGraphicSettingItem InGraphicSettingItem);

	void LoadMenuSwitchButton(ELoadGameMenuItem InLoadGameItem);

	void SwitchAudioMenuButton(EAudioSettingItem InAudioSettingItem);

	void AudioSoundControl(int32 Type, float SoundValue);

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
};