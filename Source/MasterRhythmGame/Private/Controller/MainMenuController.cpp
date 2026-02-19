// Fill out your copyright notice in the Description page of Project Settings.		


#include "Controller/MainMenuController.h"
#include "EnhancedInputComponent.h"
#include "Components/Button.h"
#include "UI/ButtonWidget.h"
#include "Components/Slider.h"
#include "MIDIDevice/Public/MIDIDeviceInputController.h"
#include "MIDIDevice/Public/MIDIDeviceManager.h"

// Sets default values
AMainMenuController::AMainMenuController()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bShowMouseCursor = true;
}

//Called when the game starts or when spawned
void AMainMenuController::BeginPlay()
{
	Super::BeginPlay();

	InitMidi();

	AMainMenuController* PlayerController = Cast<AMainMenuController>(this);

	if (PlayerController != nullptr)
	{
		MainMenuHud = Cast<AMainMenuHUD>(PlayerController->GetHUD());
	}
}

//Binds the Input Actions to their related Methods
void AMainMenuController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent))
	{
		// All Input Actions in here
	}
}

void AMainMenuController::InitMidi()
{
	DisplayMidiDevicesIDs();

	CreateMidiController();
}

void AMainMenuController::DisplayMidiDevicesIDs()
{
	TArray<FFoundMIDIDevice> InputDevices;

	UMIDIDeviceManager::FindMIDIDevices(InputDevices);

	for (auto InputDevice : InputDevices)
	{
		UE_LOG(LogTemp, Log, TEXT("ID: %d"), InputDevice.DeviceID);
		UE_LOG(LogTemp, Log, TEXT("ID: %s"), *InputDevice.DeviceName);
	}
}

void AMainMenuController::CreateMidiController()
{
	MidiController = UMIDIDeviceManager::CreateMIDIDeviceInputController(1, 1024);

	if (MidiController != nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("Successfully connected with MIDI Controller."));
		MidiController->OnMIDINoteOn.AddDynamic(this, &AMainMenuController::HandleNoteOn);
		MidiController->OnMIDINoteOff.AddDynamic(this, &AMainMenuController::HandleNoteOff);
		MidiController->OnMIDIPitchBend.AddDynamic(this, &AMainMenuController::HandlePitchBend);
		MidiController->OnMIDIAftertouch.AddDynamic(this, &AMainMenuController::HandleAftertouch);
		MidiController->OnMIDIControlChange.AddDynamic(this, &AMainMenuController::HandleControlChange);
	}
	else
	{
		// Should not happen
		UE_LOG(LogTemp, Log, TEXT("Failed to connect with MIDI Controller."));
	}
}

//Updates every frame
void AMainMenuController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void AMainMenuController::HandleNoteOn(UMIDIDeviceInputController* MIDIDeviceController, int32 Timestamp, int32 Channel, int32 Note, int32 Velocity)
{
	// Convert incoming MIDI note number to semitone class (0-11) and map to ENote.
	const int32 Semitone = Note % 12;
	
	ENote NoteEnum = static_cast<ENote>(Semitone);

	switch (GetControllerState())
	{
		case EControllerState::MainMenu:
		{
			MainMenuControl(NoteEnum);
			break;
		}
		case EControllerState::AudioMenu:
		{
			AudioMenuControl(NoteEnum);
			break;
		}
		case EControllerState::CreditsMenu:
		{
			CreditMenuControl(NoteEnum);
			break;
		}
		case EControllerState::GraphicsMenu:
		{
			GraphicMenuControl(NoteEnum);
			break;
		}
		case EControllerState::LoadMenu:
		{
			LoadMenuControl(NoteEnum);
			break;
		}
		case EControllerState::SettingMenu:
		{
			SettingMenuControl(NoteEnum);
			break;
		}
	}
}

void AMainMenuController::HandleNoteOff(UMIDIDeviceInputController* MIDIDeviceController, int32 Timestamp, int32 Channel, int32 Note, int32 Velocity)
{
	//UE_LOG(LogTemp, Log, TEXT("Channel: %d"),  Channel);
	//UE_LOG(LogTemp, Log, TEXT("Note: %d"),     Note);
	//UE_LOG(LogTemp, Log, TEXT("Velocity: %d"), Velocity);
}

void AMainMenuController::HandlePitchBend(UMIDIDeviceInputController* MIDIDeviceController, int32 Timestamp, int32 Channel, int32 Pitch)
{
	//UE_LOG(LogTemp, Log, TEXT("Channel: %d"),  Channel);
	//UE_LOG(LogTemp, Log, TEXT("Pitch: %d"),    Pitch);
}

void AMainMenuController::HandleAftertouch(UMIDIDeviceInputController* MIDIDeviceController, int32 Timestamp, int32 Channel, int32 Note, int32 Amount)
{
	//UE_LOG(LogTemp, Log, TEXT("Channel: %d"),  Channel);
	//UE_LOG(LogTemp, Log, TEXT("Note: %d"),     Note);
	//UE_LOG(LogTemp, Log, TEXT("Amount: %d"),   Amount);
}

void AMainMenuController::HandleControlChange(UMIDIDeviceInputController* MIDIDeviceController, int32 Timestamp, int32 Channel, int32 Type, int32 Value)
{
	float NormalizedValue = static_cast<float>(Value) / 127.0f;
	NormalizedValue = FMath::Clamp(NormalizedValue, 0.0f, 1.0f);

	AudioSoundControl(Type, NormalizedValue);
}

void AMainMenuController::MainMenuControl(ENote Note)
{
	if (MainMenuHud != nullptr)
	{
		switch (Note)
		{
			case ENote::C:
			{
				MainMenuIndex++;
				MainMenuIndex = FMath::Clamp(MainMenuIndex, 0, 4);
				UE_LOG(LogTemp, Log, TEXT("MainMenuIndex: %d"), MainMenuIndex);

				break;
			}
			case ENote::D:
			{
				MainMenuIndex--;
				MainMenuIndex = FMath::Clamp(MainMenuIndex, 0, 4);
				UE_LOG(LogTemp, Log, TEXT("MainMenuIndex: %d"), MainMenuIndex);

				break;
			}
			case ENote::B:
			{
				if (MainMenuHud->GetMainMenuInstance()->GetNewGameButton()->HasKeyboardFocus())
				{
					MainMenuIndex = 0;
					MainMenuHud->GetMainMenuInstance()->NewGameButtonClicked();
					break;
				}
				if (MainMenuHud->GetMainMenuInstance()->GetLoadGameButton()->HasKeyboardFocus())
				{
					MainMenuIndex = 0;
					MainMenuHud->GetMainMenuInstance()->GetLoadGameButton()->HandleButtonUnhovered();
					MainMenuHud->GetMainMenuInstance()->LoadGameButtonClicked();
					break;
				}
				if (MainMenuHud->GetMainMenuInstance()->GetSettingButton()->HasKeyboardFocus())
				{
					MainMenuIndex = 0;
					MainMenuHud->GetMainMenuInstance()->GetSettingButton()->HandleButtonUnhovered();
					MainMenuHud->GetMainMenuInstance()->SettingButtonClicked();
					break;
				}
				if (MainMenuHud->GetMainMenuInstance()->GetCreditsButton()->HasKeyboardFocus())
				{
					MainMenuIndex = 0;
					MainMenuHud->GetMainMenuInstance()->GetCreditsButton()->HandleButtonUnhovered();
					MainMenuHud->GetMainMenuInstance()->CreditsButtonClicked();
					break;
				}
				if (MainMenuHud->GetMainMenuInstance()->GetEscapeButton()->HasKeyboardFocus())
				{
					MainMenuIndex = 0;
					MainMenuHud->GetMainMenuInstance()->EscapeButtonClicked();
					break;
				}
			}
			default:
			{
				// Should not land here
				break;
			}
		}
	}
	// use enum overload, mapping index -> enum
	MainMenuSwitchButton(static_cast<EMainMenuItem>(MainMenuIndex));
}

void AMainMenuController::AudioMenuControl(ENote Note)
{
	switch (Note)
	{
		case ENote::C:
		{
			AudioMenuIndex++;
			AudioMenuIndex = FMath::Clamp(AudioMenuIndex, 0, 3);
			UE_LOG(LogTemp, Log, TEXT("AudioMenuIndex: %d"), AudioMenuIndex);

			//Switch
			break;

		}
		case ENote::D:
		{
			AudioMenuIndex--;
			AudioMenuIndex = FMath::Clamp(AudioMenuIndex, 0, 3);
			UE_LOG(LogTemp, Log, TEXT("AudioMenuIndex: %d"), AudioMenuIndex);

			//Switch
			break;
		}
		case ENote::B:
		{
			if (MainMenuHud != nullptr)
			{
				if (MainMenuHud->GetMainMenuSettingInstance()->GetAudioSettingWidget()->GetMasterVolumeSlider()->HasKeyboardFocus())
				{
					AudioMenuIndex = 0;
					break;
				}
				if (MainMenuHud->GetMainMenuSettingInstance()->GetAudioSettingWidget()->GetMusicVolumeSlider()->HasKeyboardFocus())
				{
					AudioMenuIndex = 0;
					break;
				}
				if (MainMenuHud->GetMainMenuSettingInstance()->GetAudioSettingWidget()->GetSfxVolumeSlider()->HasKeyboardFocus())
				{
					AudioMenuIndex = 0;
					break;
				}
				if (MainMenuHud->GetMainMenuSettingInstance()->GetAudioSettingWidget()->GetMainMenuButton()->HasKeyboardFocus())
				{
					AudioMenuIndex = 0;
					MainMenuHud->GetMainMenuSettingInstance()->GetAudioSettingWidget()->ReturnMenu();
					break;
				}
			}
		}
		default:
		{
			// Should not land here
			break;
		}
	}
	SwitchAudioMenuButton(static_cast<EAudioSettingItem>(AudioMenuIndex));
}

void AMainMenuController::SettingMenuControl(ENote Note)
{
	switch (Note)
	{
		case ENote::C:
		{
			SettingMenuIndex++;
			SettingMenuIndex = FMath::Clamp(SettingMenuIndex, 0, 2);
			UE_LOG(LogTemp, Log, TEXT("SettingMenuIndex: %d"), SettingMenuIndex);
			break;
		}
		case ENote::D:
		{
			SettingMenuIndex--;
			SettingMenuIndex = FMath::Clamp(SettingMenuIndex, 0, 2);
			UE_LOG(LogTemp, Log, TEXT("SettingMenuIndex: %d"), SettingMenuIndex);
			break;
		}
		case ENote::B:
		{
			if (MainMenuHud != nullptr)
			{
				if (MainMenuHud->GetMainMenuSettingInstance()->GetGraphicButton()->HasKeyboardFocus())
				{
					SetControllerState(EControllerState::GraphicsMenu);
					SettingMenuIndex = 0;
					GraphicMenuIndex = 0;
					MainMenuHud->GetMainMenuSettingInstance()->GraphicSettingClicked();
					break;
				}
				if (MainMenuHud->GetMainMenuSettingInstance()->GetAudioButton()->HasKeyboardFocus())
				{
					SetControllerState(EControllerState::AudioMenu);
					SettingMenuIndex = 0;
					AudioMenuIndex = 0;
					MainMenuHud->GetMainMenuSettingInstance()->AudioSettingClicked();
					break;
				}
				if (MainMenuHud->GetMainMenuSettingInstance()->GetMainMenuButton()->HasKeyboardFocus())
				{
					SetControllerState(EControllerState::MainMenu);
					SettingMenuIndex = 0;
					MainMenuHud->GetMainMenuSettingInstance()->ReturnMainMenuClicked();
					break;
				}
			}
		}
		default:
		{
			// Should not land here
			break;
		}
	}
	// map index -> enum
	MainSettingSwitchButton(static_cast<EMainSettingItem>(SettingMenuIndex));
}

void AMainMenuController::CreditMenuControl(ENote Note)
{
	if (MainMenuHud != nullptr)
	{
		MainMenuHud->GetCreditsMenuInstance()->GetReturnMainMenuButton()->SetKeyboardFocus();
		switch (Note)
		{
			case ENote::B:
			{
				if (MainMenuHud->GetCreditsMenuInstance()->GetReturnMainMenuButton()->HasKeyboardFocus())
				{
					MainMenuIndex = 0;
					MainMenuHud->GetCreditsMenuInstance()->ReturnMainMenuButtonClicked();
					break;
				}
			}
			default:
			{
				// Should not land here
				break;
			}
		}
	}
}

void AMainMenuController::GraphicMenuControl(ENote Note)
{
	switch (Note)
	{
		case ENote::C:
		{
			GraphicMenuIndex++;
			GraphicMenuIndex = FMath::Clamp(GraphicMenuIndex, 0, 9);
			UE_LOG(LogTemp, Log, TEXT("GraphicMenuIndex: %d"), GraphicMenuIndex);
			break;
		}
		case ENote::D:
		{
			GraphicMenuIndex--;
			GraphicMenuIndex = FMath::Clamp(GraphicMenuIndex, 0, 9);
			UE_LOG(LogTemp, Log, TEXT("GraphicMenuIndex: %d"), GraphicMenuIndex);
			break;
		}
		case ENote::B:
		{
			if (MainMenuHud != nullptr)
			{
				if (MainMenuHud->GetMainMenuSettingInstance()->GetGraphicSettingWidget()->GetWindowModeDownButton()->HasKeyboardFocus())
				{
					MainMenuHud->GetMainMenuSettingInstance()->GetGraphicSettingWidget()->WindowModeDownClicked();
					break;
				}
				if (MainMenuHud->GetMainMenuSettingInstance()->GetGraphicSettingWidget()->GetWindowModeUpButton()->HasKeyboardFocus())
				{
					MainMenuHud->GetMainMenuSettingInstance()->GetGraphicSettingWidget()->WindowModeUpClicked();
					break;
				}
				if (MainMenuHud->GetMainMenuSettingInstance()->GetGraphicSettingWidget()->GetResolutionDownButton()->HasKeyboardFocus())
				{
					MainMenuHud->GetMainMenuSettingInstance()->GetGraphicSettingWidget()->ResolutionDownClicked();
					break;
				}
				if (MainMenuHud->GetMainMenuSettingInstance()->GetGraphicSettingWidget()->GetResolutionUpButton()->HasKeyboardFocus())
				{
					MainMenuHud->GetMainMenuSettingInstance()->GetGraphicSettingWidget()->ResolutionUpClicked();
					break;
				}
				if (MainMenuHud->GetMainMenuSettingInstance()->GetGraphicSettingWidget()->GetGraphicDownButton()->HasKeyboardFocus())
				{
					MainMenuHud->GetMainMenuSettingInstance()->GetGraphicSettingWidget()->GraphicDownClicked();
					break;
				}
				if (MainMenuHud->GetMainMenuSettingInstance()->GetGraphicSettingWidget()->GetGraphicUpButton()->HasKeyboardFocus())
				{
					MainMenuHud->GetMainMenuSettingInstance()->GetGraphicSettingWidget()->GraphicUpClicked();
					break;
				}
				if (MainMenuHud->GetMainMenuSettingInstance()->GetGraphicSettingWidget()->GetVSyncDownButton()->HasKeyboardFocus())
				{
					MainMenuHud->GetMainMenuSettingInstance()->GetGraphicSettingWidget()->VSyncDownClicked();
					break;
				}
				if (MainMenuHud->GetMainMenuSettingInstance()->GetGraphicSettingWidget()->GetVSyncUpButton()->HasKeyboardFocus())
				{
					MainMenuHud->GetMainMenuSettingInstance()->GetGraphicSettingWidget()->VSyncUpClicked();
					break;
				}
				if (MainMenuHud->GetMainMenuSettingInstance()->GetGraphicSettingWidget()->GetApplyButton()->HasKeyboardFocus())
				{
					MainMenuHud->GetMainMenuSettingInstance()->GetGraphicSettingWidget()->ApplyClicked();
					break;
				}
				if (MainMenuHud->GetMainMenuSettingInstance()->GetGraphicSettingWidget()->GetMainMenuButton()->HasKeyboardFocus())
				{
					GraphicMenuIndex = 0;
					MainMenuHud->GetMainMenuSettingInstance()->GetGraphicSettingWidget()->GetMainMenuButton()->HandleButtonUnhovered();
					MainMenuHud->GetMainMenuSettingInstance()->GetGraphicSettingWidget()->ReturnMainMenuClicked();
					break;
				}
			}
		}
		default:
		{
			//Should not land here
			break;
		}
	}
	// map index -> enum
	GraphicMenuSwitchButton(static_cast<EGraphicSettingItem>(GraphicMenuIndex));
}

void AMainMenuController::LoadMenuControl(ENote Note)
{
	switch (Note)
	{
		case ENote::C:
		{
			LoadMenuIndex++;
			LoadMenuIndex = FMath::Clamp(LoadMenuIndex, 0, 4);
			UE_LOG(LogTemp, Log, TEXT("LoadMenuIndex: %d"), LoadMenuIndex);
			break;
		}
		case ENote::D:
		{
			LoadMenuIndex--;
			LoadMenuIndex = FMath::Clamp(LoadMenuIndex, 0, 4);
			UE_LOG(LogTemp, Log, TEXT("LoadMenuIndex: %d"), LoadMenuIndex);
			break;
		}
		case ENote::B:
		{	if (MainMenuHud != nullptr)
			{

				if (MainMenuHud->GetLoadMenuInstance()->GetLoadFirstButton()->HasKeyboardFocus())
				{
					MainMenuHud->GetLoadMenuInstance()->LoadFirstSaveButtonClicked();
					break;
				}
				if (MainMenuHud->GetLoadMenuInstance()->GetLoadSecondButton()->HasKeyboardFocus())
				{
					MainMenuHud->GetLoadMenuInstance()->LoadSecondSaveButtonClicked();
					break;
				}
				if (MainMenuHud->GetLoadMenuInstance()->GetLoadThirdButton()->HasKeyboardFocus())
				{
					MainMenuHud->GetLoadMenuInstance()->LoadThirdSaveButtonClicked();
					break;
				}
				if (MainMenuHud->GetLoadMenuInstance()->GetReturnMainMenuButton()->HasKeyboardFocus())
				{
					LoadMenuIndex = 0;
					MainMenuHud->GetLoadMenuInstance()->GetReturnMainMenuButton()->HandleButtonUnhovered();
					MainMenuHud->GetLoadMenuInstance()->ReturnMainMenuButtonClicked();
					break;
				}
			}
		}
		default:
		{
			//Should not land here
			break;
		}
	}
	LoadMenuSwitchButton(static_cast<ELoadGameMenuItem>(LoadMenuIndex));
}

void AMainMenuController::MainMenuSwitchButton(EMainMenuItem InMainMenuItem)
{
	if (MainMenuHud != nullptr)
	{
		MainMenuHud->GetMainMenuInstance()->GetNewGameButton()->HandleButtonUnhovered();
		MainMenuHud->GetMainMenuInstance()->GetLoadGameButton()->HandleButtonUnhovered();
		MainMenuHud->GetMainMenuInstance()->GetSettingButton()->HandleButtonUnhovered();
		MainMenuHud->GetMainMenuInstance()->GetCreditsButton()->HandleButtonUnhovered();
		MainMenuHud->GetMainMenuInstance()->GetEscapeButton()->HandleButtonUnhovered();

		switch (InMainMenuItem)
		{
			case EMainMenuItem::NewGame:
			{
				MainMenuHud->GetMainMenuInstance()->GetNewGameButton()->SetKeyboardFocus();
				MainMenuHud->GetMainMenuInstance()->GetNewGameButton()->HandleButtonHovered();
				break;
			}
			case EMainMenuItem::LoadGame:
			{
				MainMenuHud->GetMainMenuInstance()->GetLoadGameButton()->SetKeyboardFocus();
				MainMenuHud->GetMainMenuInstance()->GetLoadGameButton()->HandleButtonHovered();
				break;
			}
			case EMainMenuItem::Setting:
			{
				MainMenuHud->GetMainMenuInstance()->GetSettingButton()->SetKeyboardFocus();
				MainMenuHud->GetMainMenuInstance()->GetSettingButton()->HandleButtonHovered();
				break;
			}
			case EMainMenuItem::Credits:
			{
				MainMenuHud->GetMainMenuInstance()->GetCreditsButton()->SetKeyboardFocus();
				MainMenuHud->GetMainMenuInstance()->GetCreditsButton()->HandleButtonHovered();
				break;
			}
			case EMainMenuItem::Escape:
			{
				MainMenuHud->GetMainMenuInstance()->GetEscapeButton()->SetKeyboardFocus();
				MainMenuHud->GetMainMenuInstance()->GetEscapeButton()->HandleButtonHovered();
				break;
			}
			default:
			{
				// Should not land here
				break;
			}
		}
	}
}

void AMainMenuController::MainSettingSwitchButton(EMainSettingItem InMainSettingItem)
{
	if (MainMenuHud != nullptr)
	{
		MainMenuHud->GetMainMenuSettingInstance()->GetGraphicButton()->HandleButtonUnhovered();
		MainMenuHud->GetMainMenuSettingInstance()->GetAudioButton()->HandleButtonUnhovered();
		MainMenuHud->GetMainMenuSettingInstance()->GetMainMenuButton()->HandleButtonUnhovered();

		switch (InMainSettingItem)
		{
			case EMainSettingItem::Graphic:
			{
				MainMenuHud->GetMainMenuSettingInstance()->GetGraphicButton()->SetKeyboardFocus();
				MainMenuHud->GetMainMenuSettingInstance()->GetGraphicButton()->HandleButtonHovered();
				break;
			}
			case EMainSettingItem::Audio:
			{
				MainMenuHud->GetMainMenuSettingInstance()->GetAudioButton()->SetKeyboardFocus();
				MainMenuHud->GetMainMenuSettingInstance()->GetAudioButton()->HandleButtonHovered();
				break;
			}
			case EMainSettingItem::MainMenu:
			{
				MainMenuHud->GetMainMenuSettingInstance()->GetMainMenuButton()->SetKeyboardFocus();
				MainMenuHud->GetMainMenuSettingInstance()->GetMainMenuButton()->HandleButtonHovered();
				break;
			}
			default:
			{
				// Should not land here
				break;
			}
		}
	}
}

void AMainMenuController::GraphicMenuSwitchButton(EGraphicSettingItem InGraphicSettingItem)
{
	if (MainMenuHud != nullptr)
	{
		MainMenuHud->GetMainMenuSettingInstance()->GetGraphicSettingWidget()->GetWindowModeDownButton()->HandleButtonUnhovered();
		MainMenuHud->GetMainMenuSettingInstance()->GetGraphicSettingWidget()->GetWindowModeUpButton()->HandleButtonUnhovered();
		MainMenuHud->GetMainMenuSettingInstance()->GetGraphicSettingWidget()->GetResolutionDownButton()->HandleButtonUnhovered();
		MainMenuHud->GetMainMenuSettingInstance()->GetGraphicSettingWidget()->GetResolutionUpButton()->HandleButtonUnhovered();
		MainMenuHud->GetMainMenuSettingInstance()->GetGraphicSettingWidget()->GetGraphicDownButton()->HandleButtonUnhovered();
		MainMenuHud->GetMainMenuSettingInstance()->GetGraphicSettingWidget()->GetGraphicUpButton()->HandleButtonUnhovered();
		MainMenuHud->GetMainMenuSettingInstance()->GetGraphicSettingWidget()->GetVSyncDownButton()->HandleButtonUnhovered();
		MainMenuHud->GetMainMenuSettingInstance()->GetGraphicSettingWidget()->GetVSyncUpButton()->HandleButtonUnhovered();
		MainMenuHud->GetMainMenuSettingInstance()->GetGraphicSettingWidget()->GetApplyButton()->HandleButtonUnhovered();
		MainMenuHud->GetMainMenuSettingInstance()->GetGraphicSettingWidget()->GetMainMenuButton()->HandleButtonUnhovered();

		switch (InGraphicSettingItem)
		{
			case EGraphicSettingItem::WindowModeDown:
			{
				MainMenuHud->GetMainMenuSettingInstance()->GetGraphicSettingWidget()->GetWindowModeDownButton()->SetKeyboardFocus();
				MainMenuHud->GetMainMenuSettingInstance()->GetGraphicSettingWidget()->GetWindowModeDownButton()->HandleButtonHovered();
				break;
			}
			case EGraphicSettingItem::WindowModeUp:
			{
				MainMenuHud->GetMainMenuSettingInstance()->GetGraphicSettingWidget()->GetWindowModeUpButton()->SetKeyboardFocus();
				MainMenuHud->GetMainMenuSettingInstance()->GetGraphicSettingWidget()->GetWindowModeUpButton()->HandleButtonHovered();
				break;
			}
			case EGraphicSettingItem::ResolutionDown:
			{
				MainMenuHud->GetMainMenuSettingInstance()->GetGraphicSettingWidget()->GetResolutionDownButton()->SetKeyboardFocus();
				MainMenuHud->GetMainMenuSettingInstance()->GetGraphicSettingWidget()->GetResolutionDownButton()->HandleButtonHovered();
				break;
			}
			case EGraphicSettingItem::ResolutionUp:
			{
				MainMenuHud->GetMainMenuSettingInstance()->GetGraphicSettingWidget()->GetResolutionUpButton()->SetKeyboardFocus();
				MainMenuHud->GetMainMenuSettingInstance()->GetGraphicSettingWidget()->GetResolutionUpButton()->HandleButtonHovered();
				break;
			}
			case EGraphicSettingItem::GraphicDown:
			{
				MainMenuHud->GetMainMenuSettingInstance()->GetGraphicSettingWidget()->GetGraphicDownButton()->SetKeyboardFocus();
				MainMenuHud->GetMainMenuSettingInstance()->GetGraphicSettingWidget()->GetGraphicDownButton()->HandleButtonHovered();
				break;
			}
			case EGraphicSettingItem::GraphicUp:
			{
				MainMenuHud->GetMainMenuSettingInstance()->GetGraphicSettingWidget()->GetGraphicUpButton()->SetKeyboardFocus();
				MainMenuHud->GetMainMenuSettingInstance()->GetGraphicSettingWidget()->GetGraphicUpButton()->HandleButtonHovered();
				break;
			}
			case EGraphicSettingItem::VSyncDown:
			{
				MainMenuHud->GetMainMenuSettingInstance()->GetGraphicSettingWidget()->GetVSyncDownButton()->SetKeyboardFocus();
				MainMenuHud->GetMainMenuSettingInstance()->GetGraphicSettingWidget()->GetVSyncDownButton()->HandleButtonHovered();
				break;
			}
			case EGraphicSettingItem::VSyncUp:
			{
				MainMenuHud->GetMainMenuSettingInstance()->GetGraphicSettingWidget()->GetVSyncUpButton()->SetKeyboardFocus();
				MainMenuHud->GetMainMenuSettingInstance()->GetGraphicSettingWidget()->GetVSyncUpButton()->HandleButtonHovered();
				break;
			}
			case EGraphicSettingItem::Apply:
			{
				MainMenuHud->GetMainMenuSettingInstance()->GetGraphicSettingWidget()->GetApplyButton()->SetKeyboardFocus();
				MainMenuHud->GetMainMenuSettingInstance()->GetGraphicSettingWidget()->GetApplyButton()->HandleButtonHovered();
				break;
			}
			case EGraphicSettingItem::Return:
			{
				MainMenuHud->GetMainMenuSettingInstance()->GetGraphicSettingWidget()->GetMainMenuButton()->SetKeyboardFocus();
				MainMenuHud->GetMainMenuSettingInstance()->GetGraphicSettingWidget()->GetMainMenuButton()->HandleButtonHovered();
				break;
			}
			default:
			{
				// Should not land here
				break;
			}
		}
	}
}

void AMainMenuController::LoadMenuSwitchButton(ELoadGameMenuItem InLoadGameItem)
{
	if (MainMenuHud != nullptr)
	{
		MainMenuHud->GetLoadMenuInstance()->GetLoadFirstButton()->HandleButtonUnhovered();
		MainMenuHud->GetLoadMenuInstance()->GetLoadSecondButton()->HandleButtonUnhovered();
		MainMenuHud->GetLoadMenuInstance()->GetLoadThirdButton()->HandleButtonUnhovered();
		MainMenuHud->GetLoadMenuInstance()->GetReturnMainMenuButton()->HandleButtonUnhovered();

		switch (InLoadGameItem)
		{
			case ELoadGameMenuItem::FirstLevel:
			{
				MainMenuHud->GetLoadMenuInstance()->GetLoadFirstButton()->SetKeyboardFocus();
				MainMenuHud->GetLoadMenuInstance()->GetLoadFirstButton()->HandleButtonHovered();
				break;
			}
			case ELoadGameMenuItem::SecondLevel:
			{
				MainMenuHud->GetLoadMenuInstance()->GetLoadSecondButton()->SetKeyboardFocus();
				MainMenuHud->GetLoadMenuInstance()->GetLoadSecondButton()->HandleButtonHovered();
				break;
			}
			case ELoadGameMenuItem::ThirdLevel:
			{
				MainMenuHud->GetLoadMenuInstance()->GetLoadThirdButton()->SetKeyboardFocus();
				MainMenuHud->GetLoadMenuInstance()->GetLoadThirdButton()->HandleButtonHovered();
				break;
			}
			case ELoadGameMenuItem::MainMenu:
			{
				MainMenuHud->GetLoadMenuInstance()->GetReturnMainMenuButton()->SetKeyboardFocus();
				MainMenuHud->GetLoadMenuInstance()->GetReturnMainMenuButton()->HandleButtonHovered();
				break;
			}
			default:
			{
				// Should not land here
				break;
			}
		}
	}
}

void AMainMenuController::SwitchAudioMenuButton(EAudioSettingItem InAudioSettingItem)
{
	if (MainMenuHud != nullptr)
	{
		MainMenuHud->GetMainMenuSettingInstance()->GetAudioSettingWidget()->MasterVolumeSliderUnhovered();
		MainMenuHud->GetMainMenuSettingInstance()->GetAudioSettingWidget()->MusicVolumeSliderUnhovered();
		MainMenuHud->GetMainMenuSettingInstance()->GetAudioSettingWidget()->SfxVolumeSliderUnhovered();
		MainMenuHud->GetMainMenuSettingInstance()->GetAudioSettingWidget()->GetMainMenuButton()->HandleButtonUnhovered();

		switch (InAudioSettingItem)
		{
			case EAudioSettingItem::MasterVolumeSlider:
			{
				MainMenuHud->GetMainMenuSettingInstance()->GetAudioSettingWidget()->GetMasterVolumeSlider()->SetKeyboardFocus();
				MainMenuHud->GetMainMenuSettingInstance()->GetAudioSettingWidget()->MasterVolumeSliderHovered();

				break;
			}
			case EAudioSettingItem::MusicVolumeSlider:
			{
				MainMenuHud->GetMainMenuSettingInstance()->GetAudioSettingWidget()->GetMusicVolumeSlider()->SetKeyboardFocus();
				MainMenuHud->GetMainMenuSettingInstance()->GetAudioSettingWidget()->MusicVolumeSliderHovered();
				break;
			}
			case EAudioSettingItem::SfxVolumeSlider:
			{
				MainMenuHud->GetMainMenuSettingInstance()->GetAudioSettingWidget()->GetSfxVolumeSlider()->SetKeyboardFocus();
				MainMenuHud->GetMainMenuSettingInstance()->GetAudioSettingWidget()->SfxVolumeSliderHovered();

				break;
			}
			case EAudioSettingItem::Return:
			{
				MainMenuHud->GetMainMenuSettingInstance()->GetAudioSettingWidget()->GetMainMenuButton()->GetButton()->SetKeyboardFocus();
				MainMenuHud->GetMainMenuSettingInstance()->GetAudioSettingWidget()->GetMainMenuButton()->HandleButtonHovered();
				break;
			}
			default:
			{
				// Should not land here
				break;
			}
		}
	}
}

void AMainMenuController::AudioSoundControl(int32 Type, float SoundValue)
{
	// Modwheel
	if (Type == 1)
	{
		if (MainMenuHud != nullptr)
		{
			// TODO: Safe the Value in the GameInstance later!
			if (MainMenuHud->GetMainMenuSettingInstance()->GetAudioSettingWidget()->GetMasterVolumeSlider()->HasKeyboardFocus())
			{
				MainMenuHud->GetMainMenuSettingInstance()->GetAudioSettingWidget()->GetMasterVolumeSlider()->SetValue(SoundValue);
			}
			if (MainMenuHud->GetMainMenuSettingInstance()->GetAudioSettingWidget()->GetMusicVolumeSlider()->HasKeyboardFocus())
			{
				MainMenuHud->GetMainMenuSettingInstance()->GetAudioSettingWidget()->GetMusicVolumeSlider()->SetValue(SoundValue);
			}
			if (MainMenuHud->GetMainMenuSettingInstance()->GetAudioSettingWidget()->GetSfxVolumeSlider()->HasKeyboardFocus())
			{
				MainMenuHud->GetMainMenuSettingInstance()->GetAudioSettingWidget()->GetSfxVolumeSlider()->SetValue(SoundValue);
			}
		}
	}
}