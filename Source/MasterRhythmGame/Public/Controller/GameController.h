// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MIDIDeviceInputController.h"
#include "GameFramework/PlayerController.h"
#include "GameController.generated.h"

enum class EMainSettingItem : uint8;
enum class EPauseMenuItem;

enum class EControllerStateGame
{
	PauseMenu,
	Game,
	SettingMenu,
	GraphicsMenu,
	AudioMenu
};

class AGameHUD;
enum class EAudioSettingItem : uint8;
enum class EGraphicSettingItem : uint8;
enum class ENote : uint8;
/**
 * 
 */
UCLASS()
class MASTERRHYTHMGAME_API AGameController : public APlayerController
{
	GENERATED_BODY()
public:
	AGameController();

	EControllerStateGame GetControllerState() const { return ControllerState; }

	void SetControllerState(EControllerStateGame NewState) { ControllerState = NewState; }

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

	void GameControl(ENote Note);

	void PauseMenuControl(ENote Note);

	void SettingMenuControl(ENote Note);

	void GraphicMenuControl(ENote Note);

	void AudioMenuControl(ENote Note);

	void GraphicMenuSwitchButton(EGraphicSettingItem InGraphicSettingItem);

	void SwitchAudioMenuButton(EAudioSettingItem InAudioSettingItem);

	void AudioSoundControl(int32 Type, float SoundValue);

	void PauseMenuSwitchButton(EPauseMenuItem InPauseMenuItem);

	void MainSettingSwitchButton(EMainSettingItem InMainSettingItem);

private:
	UPROPERTY()
	TObjectPtr<AGameHUD> GameHud { nullptr };

	UPROPERTY(VisibleAnywhere, DisplayName = "Midi Controller", Category = "Components")
	TObjectPtr<class UMIDIDeviceInputController> MidiController{ nullptr };

	UPROPERTY(VisibleAnywhere, DisplayName = "GraphicMenuIndex", Category = "Components")
	int32 GraphicMenuIndex { 0 };

	UPROPERTY(VisibleAnywhere, DisplayName = "AudioMenuIndex", Category = "Components")
	int32 AudioMenuIndex { 0 };

	UPROPERTY(VisibleAnywhere, DisplayName = "SettingMenuIndex", Category = "Components")
	int32 SettingMenuIndex { 0 };

	UPROPERTY(VisibleAnywhere, DisplayName = "PauseMenuIndex", Category = "Components")
	int32 PauseMenuIndex{ 0 };

	EControllerStateGame ControllerState { EControllerStateGame::Game };
};