// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "InputMappingContext.h"
#include "MIDIDevice/Public/MIDIDeviceInputController.h"
#include "GameFramework/PlayerController.h"
#include "HUD/MainMenuHUD.h"
#include "MainMenuController.generated.h"

enum class ENote : uint8
{
	C      = 0,
	CSharp = 1,
	D      = 2,
	DSharp = 3,
	E      = 4,
	F      = 5,
	FSharp = 6,
	G      = 7,
	GSharp = 8,
	A      = 9,
	ASharp = 10,
	B      = 11
};

enum class EMainMenuItem : uint8
{
	NewGame = 0,
	LoadGame = 1,
	Setting = 2,
	Credits = 3,
	Escape = 4
};

enum class EMainSettingItem : uint8
{
	Graphic = 0,
	Audio = 1,
	MainMenu = 2
};

enum class EGraphicSettingItem : uint8
{
	WindowModeDown = 0,
	WindowModeUp = 1,
	ResolutionDown = 2,
	ResolutionUp = 3,
	GraphicDown = 4,
	GraphicUp = 5,
	VSyncDown = 6,
	VSyncUp = 7,
	Apply = 8,
	Return = 9
};

enum class EAudioSettingItem : uint8
{
	MasterVolumeSlider = 0,
	MusicVolumeSlider = 1,
	SfxVolumeSlider = 2,
	Return = 3
};

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

	EControllerState GetControllerState() const { return ControllerState; }

	void SetControllerState(EControllerState NewState) { ControllerState = NewState; }

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



#pragma region InputAction

	UPROPERTY(EditAnywhere, DisplayName = "MappingContext", Category = "InputAction", meta = (AllowPrivateAccess = true))
	UInputMappingContext* DefaultMappingContext { nullptr };


#pragma endregion
};