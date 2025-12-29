// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MIDIDeviceInputController.h"
#include "GameFramework/PlayerController.h"
#include "GameController.generated.h"


class ASplineActor;
class ATestEnemyActor;
class ANodeActor;
class UInputAction;

enum class EControllerStateGame
{
	PauseMenu,
	Game,
	SettingMenu,
	GraphicsMenu,
	AudioMenu,
	DeathMenu
};

enum class EDeathState : uint8
{
	Respawn,
	WorldMap,
	MainMenu
};

class UInputMappingContext;
class AGameCharacter;
class AGameHUD;
enum class EAudioSettingItem : uint8;
enum class EGraphicSettingItem : uint8;
enum class ENote : uint8;
enum class EMainSettingItem : uint8;
enum class EPauseMenuItem;

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

	UFUNCTION()
	void HandleCAttack();

	UFUNCTION()
	void HandleDAttack();

	UFUNCTION()
	void HandleEAttack();

	UFUNCTION()
	void HandleFAttack();

	UFUNCTION()
	void HandleGAttack();

	UFUNCTION()
	void HandleAAttack();

	UFUNCTION()
	void HandleBAttack();

	UFUNCTION()
	void HandleAttack();

	UFUNCTION()
	void HandlePause();

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

	void DeathMenuControl(ENote Note);

	void DeathMenuSwitchButton(EDeathState InDeathState);

private:
	UPROPERTY()
	TObjectPtr<AGameHUD> GameHud { nullptr };

	UPROPERTY(VisibleAnywhere, DisplayName = "Midi Controller", Category = "Components")
	TObjectPtr<class UMIDIDeviceInputController> MidiController { nullptr };

	UPROPERTY(VisibleAnywhere, DisplayName = "GraphicMenuIndex", Category = "Components")
	int32 GraphicMenuIndex { 0 };

	UPROPERTY(VisibleAnywhere, DisplayName = "AudioMenuIndex", Category = "Components")
	int32 AudioMenuIndex { 0 };

	UPROPERTY(VisibleAnywhere, DisplayName = "SettingMenuIndex", Category = "Components")
	int32 SettingMenuIndex { 0 };

	UPROPERTY(VisibleAnywhere, DisplayName = "PauseMenuIndex", Category = "Components")
	int32 PauseMenuIndex { 0 };

	UPROPERTY(VisibleAnywhere, DisplayName = "DeathMenuIndex", Category = "Components")
	int32 DeathMenuIndex { 0 };

	EControllerStateGame ControllerState { EControllerStateGame::Game };

	UPROPERTY()
	TArray<FVector> Points;

	UPROPERTY()
	TObjectPtr<AGameCharacter> GameCharacter { nullptr };

	UPROPERTY(EditAnywhere, DisplayName = "GameCharacter Blueprint Class", Category = "Components")
	TSubclassOf<AGameCharacter> GameCharacterBPClass { nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	TSubclassOf<ANodeActor> NodeActor{ nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	TSubclassOf<ATestEnemyActor> EnemyClass { nullptr };

	UPROPERTY()
	TObjectPtr<ATestEnemyActor> Enemy { nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	TSubclassOf<ASplineActor> SplineClass { nullptr };

	UPROPERTY()
	TObjectPtr<ASplineActor> Spline { nullptr };

#pragma region InputAction

	UPROPERTY(EditAnywhere, DisplayName = "MappingContext", Category = "InputAction")
	TObjectPtr<UInputMappingContext> DefaultMappingContext { nullptr };

	UPROPERTY(EditAnywhere, DisplayName = "C Action", Category = "InputAction")
	UInputAction* CAction { nullptr };

	UPROPERTY(EditAnywhere, DisplayName = "D Action", Category = "InputAction")
	UInputAction* DAction { nullptr };

	UPROPERTY(EditAnywhere, DisplayName = "E Action", Category = "InputAction")
	UInputAction* EAction { nullptr };

	UPROPERTY(EditAnywhere, DisplayName = "F Action", Category = "InputAction")
	UInputAction* FAction { nullptr };

	UPROPERTY(EditAnywhere, DisplayName = "G Action", Category = "InputAction")
	UInputAction* GAction { nullptr };

	UPROPERTY(EditAnywhere, DisplayName = "A Action", Category = "InputAction")
	UInputAction* AAction { nullptr };

	UPROPERTY(EditAnywhere, DisplayName = "B Action", Category = "InputAction")
	UInputAction* BAction { nullptr };
	
	UPROPERTY(EditAnywhere, DisplayName = "Pause Action", Category = "InputAction")
	UInputAction* PauseAction { nullptr };

	UPROPERTY(EditAnywhere, DisplayName = "Attack Action", Category = "InputAction")
	UInputAction* AttackAction { nullptr };

#pragma endregion
};