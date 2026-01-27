// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MIDIDeviceInputController.h"
#include "GameFramework/PlayerController.h"
#include "../CoreTypes.h"
#include "GameController.generated.h"


class ASplineActor;
class ATestEnemyActor;
class ANodeActor;
class UInputAction;
class UInputMappingContext;
class AGameCharacter;
class AGameHUD;

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

	void InitData(class ULevelData* LevelData);

	float GetInstrumentDamageFactor(EInstrument Instrument);

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

#pragma region KeyboardAttacks

	UFUNCTION()
	void HandleC1Attack();

	UFUNCTION()
	void HandleCSharp1Attack();

	UFUNCTION()
	void HandleD1Attack();
	
	UFUNCTION()
	void HandleDSharp1Attack();

	UFUNCTION()
	void HandleE1Attack();

	UFUNCTION()
	void HandleF1Attack();
	
	UFUNCTION()
	void HandleFSharp1Attack();

	UFUNCTION()
	void HandleG1Attack();
	
	UFUNCTION()
	void HandleGSharp1Attack();
	
	UFUNCTION()
	void HandleA1Attack();
	
	UFUNCTION()
	void HandleASharp1Attack();
	
	UFUNCTION()
	void HandleB1Attack();
	
	UFUNCTION()
	void HandleC2Attack();

	UFUNCTION()
	void HandleCSharp2Attack();

	UFUNCTION()
	void HandleD2Attack();
	
	UFUNCTION()
	void HandleDSharp2Attack();

	UFUNCTION()
	void HandleE2Attack();

	UFUNCTION()
	void HandleF2Attack();
	
	UFUNCTION()
	void HandleFSharp2Attack();

	UFUNCTION()
	void HandleG2Attack();

	//Called by any of the bound Actions to the Input Actions
	//Remaps Midi Note to the scale and handles Movement and Attacks
	void HandleKeyboardAttack(int32 MidiNote);

#pragma endregion KeyboardAttacks

	UFUNCTION()
	void HandleAttack(ENote Note);
	
	UFUNCTION()
	void HandleNoteRelease();

	UFUNCTION()
	void HandlePause();

	UFUNCTION()
	bool SwitchInstrument(int32 Note);

	void MovePlayer(ENote Note);

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
	float FrameToSeconds();

	void DisablePlayerCollision();

public:
	EOctave CNoteOctave { EOctave::Four };

private:

	ENote RootNote { ENote::C };

	EScale Scale { EScale::Major };

	int32 NotePlayed { -1 };

	/**
	 * The damage factor each instrument's damage is multiplied by
	 */
	float PianoDamageFactor     { 1.f };
	float ViolinDamageFactor    { 1.f };
	float SaxophoneDamageFactor { 1.f };
	float GuitarDamageFactor    { 1.f };
	float SynthDamageFactor     { 1.f };

	EInstrument SelectedInstrument { EInstrument::Piano };

	//Buffers the Instrument when playing so that even when you switch instruments during a note
	//the note off command is always tied to the played note
	EInstrument InstrumentBuffer { EInstrument::None };

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

	UPROPERTY(EditAnywhere, DisplayName = "C1 Action", Category = "InputAction")
	UInputAction* C1Action { nullptr };

	UPROPERTY(EditAnywhere, DisplayName = "C#1 Action", Category = "InputAction")
	UInputAction* CSharp1Action { nullptr };

	UPROPERTY(EditAnywhere, DisplayName = "D1 Action", Category = "InputAction")
	UInputAction* D1Action { nullptr };
	
	UPROPERTY(EditAnywhere, DisplayName = "D#1 Action", Category = "InputAction")
	UInputAction* DSharp1Action { nullptr };

	UPROPERTY(EditAnywhere, DisplayName = "E1 Action", Category = "InputAction")
	UInputAction* E1Action { nullptr };

	UPROPERTY(EditAnywhere, DisplayName = "F1 Action", Category = "InputAction")
	UInputAction* F1Action { nullptr };
	
	UPROPERTY(EditAnywhere, DisplayName = "F#1 Action", Category = "InputAction")
	UInputAction* FSharp1Action { nullptr };

	UPROPERTY(EditAnywhere, DisplayName = "G1 Action", Category = "InputAction")
	UInputAction* G1Action { nullptr };
	
	UPROPERTY(EditAnywhere, DisplayName = "G#1 Action", Category = "InputAction")
	UInputAction* GSharp1Action { nullptr };

	UPROPERTY(EditAnywhere, DisplayName = "A1 Action", Category = "InputAction")
	UInputAction* A1Action { nullptr };
	
	UPROPERTY(EditAnywhere, DisplayName = "A#1 Action", Category = "InputAction")
	UInputAction* ASharp1Action { nullptr };

	UPROPERTY(EditAnywhere, DisplayName = "B1 Action", Category = "InputAction")
	UInputAction* B1Action { nullptr };

	UPROPERTY(EditAnywhere, DisplayName = "C2 Action", Category = "InputAction")
	UInputAction* C2Action { nullptr };

	UPROPERTY(EditAnywhere, DisplayName = "C#2 Action", Category = "InputAction")
	UInputAction* CSharp2Action { nullptr };

	UPROPERTY(EditAnywhere, DisplayName = "D2 Action", Category = "InputAction")
	UInputAction* D2Action { nullptr };
	
	UPROPERTY(EditAnywhere, DisplayName = "D#2 Action", Category = "InputAction")
	UInputAction* DSharp2Action { nullptr };

	UPROPERTY(EditAnywhere, DisplayName = "E2 Action", Category = "InputAction")
	UInputAction* E2Action { nullptr };

	UPROPERTY(EditAnywhere, DisplayName = "F2 Action", Category = "InputAction")
	UInputAction* F2Action { nullptr };
	
	UPROPERTY(EditAnywhere, DisplayName = "F#2 Action", Category = "InputAction")
	UInputAction* FSharp2Action { nullptr };

	UPROPERTY(EditAnywhere, DisplayName = "G2 Action", Category = "InputAction")
	UInputAction* G2Action { nullptr };
	
	UPROPERTY(EditAnywhere, DisplayName = "Pause Action", Category = "InputAction")
	UInputAction* PauseAction { nullptr };

#pragma endregion

	FTimerHandle CollisionTimerHandle;

	int32 FramesToEnableCollision { 30 };
};