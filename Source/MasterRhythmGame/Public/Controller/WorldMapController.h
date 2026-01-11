// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MIDIDeviceInputController.h"
#include "GameFramework/PlayerController.h"
#include "HUD/WorldMapHUD.h"
#include "../CoreTypes.h"
#include "WorldMapController.generated.h"

class UInputAction;
class UInputMappingContext;
class AWorldMapPlayerCharacter;
class ALevelNode;
/**
 * 
 */
UCLASS()
class MASTERRHYTHMGAME_API AWorldMapController : public APlayerController
{
	GENERATED_BODY()
public:
	AWorldMapController();

	EControllerStateWorldMap GetControllerState() const { return ControllerStateWorldMap; }

	void SetControllerState(EControllerStateWorldMap NewState) { ControllerStateWorldMap = NewState; }

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

	UFUNCTION()
	void HandleMoveForward();

	UFUNCTION()
	void HandleMoveBackward();

	UFUNCTION()
	void HandleMoveLeft();

	UFUNCTION()
	void HandleMoveRight();

	UFUNCTION()
	void HandlePause();

	UFUNCTION()
	void HandleOpenLevel();

	void WorldMapControl(ENote Note);

	void PauseMenuControl(ENote Note);

	void AudioMenuControl(ENote Note);

	void SettingMenuControl(ENote Note);

	void GraphicMenuControl(ENote Note);

	void MainSettingSwitchButton(EMainSettingItem InMainSettingItem);

	void GraphicMenuSwitchButton(EGraphicSettingItem InGraphicSettingItem);

	void SwitchAudioMenuButton(EAudioSettingItem InAudioSettingItem);

	void AudioSoundControl(int32 Type, float SoundValue);

	void WorldMapSwitchButton(EWorldMapItem InWorldMapItem);

	void PauseMenuSwitchButton(EPauseMenuItem InPauseMenuItem);
private:
	UPROPERTY(VisibleAnywhere, DisplayName = "Midi Controller", Category = "Components")
	TObjectPtr<class UMIDIDeviceInputController> MidiController { nullptr };

	UPROPERTY()
	TObjectPtr<AWorldMapHUD> WorldMapHUD { nullptr };

	UPROPERTY(VisibleAnywhere, DisplayName = "AudioMenuIndex", Category = "Components")
	int32 AudioMenuIndex{ 0 };

	UPROPERTY(VisibleAnywhere, DisplayName = "GraphicMenuIndex", Category = "Components")
	int32 GraphicMenuIndex{ 0 };

	UPROPERTY(VisibleAnywhere, DisplayName = "SettingMenuIndex", Category = "Components")
	int32 SettingMenuIndex{ 0 };

	UPROPERTY(VisibleAnywhere, DisplayName = "PauseMenuIndex", Category = "Components")
	int32 PauseMenuIndex { 0 };

	EControllerStateWorldMap ControllerStateWorldMap { EControllerStateWorldMap::WorldMap };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	TSubclassOf<AWorldMapPlayerCharacter> PlayerCharacterClass { nullptr };

	UPROPERTY(EditAnywhere)
	TObjectPtr<AWorldMapPlayerCharacter> PlayerCharacter { nullptr };

#pragma region InputAction

	UPROPERTY(EditAnywhere, DisplayName = "MappingContext", Category = "InputAction")
	TObjectPtr<UInputMappingContext> DefaultMappingContext { nullptr };

	UPROPERTY(EditAnywhere, DisplayName = "Move Forward", Category = "InputAction")
	UInputAction* MoveForwardAction { nullptr };

	UPROPERTY(EditAnywhere, DisplayName = "Move Backward", Category = "InputAction")
	UInputAction* MoveBackwardAction { nullptr };

	UPROPERTY(EditAnywhere, DisplayName = "Move Right", Category = "InputAction")
	UInputAction* MoveRightAction { nullptr };

	UPROPERTY(EditAnywhere, DisplayName = "Move Left", Category = "InputAction")
	UInputAction* MoveLeftAction { nullptr };

	UPROPERTY(EditAnywhere, DisplayName = "Pause", Category = "InputAction")
	UInputAction* PauseAction { nullptr };

	UPROPERTY(EditAnywhere, DisplayName = "OpenLevel", Category = "InputAction")
	UInputAction* OpenLevelAction { nullptr };

#pragma endregion
};
