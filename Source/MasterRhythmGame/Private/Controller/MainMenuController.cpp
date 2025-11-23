// Fill out your copyright notice in the Description page of Project Settings.		


#include "Controller/MainMenuController.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputSubsystemInterface.h"
#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"
#include "Components/Button.h"
#include "MIDIDevice/Public/MIDIDeviceInputController.h"
#include "MIDIDevice/Public/MIDIDeviceManager.h"

// Sets default values
AMainMenuController::AMainMenuController()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	InitInputAction();

	bShowMouseCursor = true;
}

//Called when the game starts or when spawned
void AMainMenuController::BeginPlay()
{
	Super::BeginPlay();

	InitMidi();

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		// Add mapping context
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}

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

//Initialize the Mapping Context and the Input Actions
void AMainMenuController::InitInputAction()
{
	// Initialize everything
	DefaultMappingContext = LoadObject<UInputMappingContext>(nullptr, TEXT("/Game/Input/IMC_Character.IMC_Character"));

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
	ENote NoteEnum;
	bool bMapped = true;

	switch (Semitone)
	{
		case 0:  NoteEnum = ENote::C; break;
		case 2:  NoteEnum = ENote::D; break;
		case 4:  NoteEnum = ENote::E; break;
		case 5:  NoteEnum = ENote::F; break;
		case 7:  NoteEnum = ENote::G; break;
		case 9:  NoteEnum = ENote::A; break;
		case 11: NoteEnum = ENote::B; break;
		default:
			bMapped = false;
			break;
	}

	if (!bMapped)
	{
		// Ignore notes that are not part of the defined C-D-E-F-G-A-B set.
		return;
	}

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
	//UE_LOG(LogTemp, Log, TEXT("Channel: %d"),  Channel);
	//UE_LOG(LogTemp, Log, TEXT("Type: %d"),     Type);
	//UE_LOG(LogTemp, Log, TEXT("Value: %d"),    Value);
}

void AMainMenuController::MainMenuControl(ENote Note)
{
	switch (Note)
	{
		case ENote::C:
		{
			MainMenuIndex++;
			MainMenuIndex = FMath::Clamp(MainMenuIndex, 0, 4);
			UE_LOG(LogTemp, Log, TEXT("MainMenuIndex: %d"), MainMenuIndex);

			// Switch
			break;
		}
		case ENote::D:
		{
			MainMenuIndex--;
			MainMenuIndex = FMath::Clamp(MainMenuIndex, 0, 4);
			UE_LOG(LogTemp, Log, TEXT("MainMenuIndex: %d"), MainMenuIndex);

			// Switch 
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
				MainMenuHud->GetMainMenuInstance()->LoadGameUnhovered();
				MainMenuHud->GetMainMenuInstance()->LoadGameButtonClicked();
				break;
			}
			if (MainMenuHud->GetMainMenuInstance()->GetSettingButton()->HasKeyboardFocus())
			{
				MainMenuIndex = 0;
				MainMenuHud->GetMainMenuInstance()->SettingUnhovered();
				MainMenuHud->GetMainMenuInstance()->SettingButtonClicked();
				break;
			}
			if (MainMenuHud->GetMainMenuInstance()->GetCreditsButton()->HasKeyboardFocus())
			{
				MainMenuIndex = 0;
				MainMenuHud->GetMainMenuInstance()->CreditUnhovered();
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
					MainMenuHud->GetMainMenuSettingInstance()->GetGraphicSettingWidget()->ReturnMainMenuUnhovered();
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

			//Switch
			break;
		}
		case ENote::D:
		{
			LoadMenuIndex--;
			LoadMenuIndex = FMath::Clamp(LoadMenuIndex, 0, 4);
			UE_LOG(LogTemp, Log, TEXT("LoadMenuIndex: %d"), LoadMenuIndex);

			//Switch
			break;
		}
		default:
		{
			//Should not land here
			break;
		}
	}
}

void AMainMenuController::MainMenuSwitchButton(EMainMenuItem InMainMenuItem)
{
	if (MainMenuHud != nullptr)
	{
		switch (InMainMenuItem)
		{
			case EMainMenuItem::NewGame:
			{
				MainMenuHud->GetMainMenuInstance()->GetNewGameButton()->SetKeyboardFocus();
				MainMenuHud->GetMainMenuInstance()->NewGameHovered();
				break;
			}
			case EMainMenuItem::LoadGame:
			{
				MainMenuHud->GetMainMenuInstance()->GetLoadGameButton()->SetKeyboardFocus();
				MainMenuHud->GetMainMenuInstance()->LoadGameHovered();
				break;
			}
			case EMainMenuItem::Setting:
			{
				MainMenuHud->GetMainMenuInstance()->GetSettingButton()->SetKeyboardFocus();
				MainMenuHud->GetMainMenuInstance()->SettingHovered();
				break;
			}
			case EMainMenuItem::Credits:
			{
				MainMenuHud->GetMainMenuInstance()->GetCreditsButton()->SetKeyboardFocus();
				MainMenuHud->GetMainMenuInstance()->CreditHovered();
				break;
			}
			case EMainMenuItem::Escape:
			{
				MainMenuHud->GetMainMenuInstance()->GetEscapeButton()->SetKeyboardFocus();
				MainMenuHud->GetMainMenuInstance()->EscapeHovered();
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
		switch (InMainSettingItem)
		{
			case EMainSettingItem::Graphic:
			{
				MainMenuHud->GetMainMenuSettingInstance()->GetGraphicButton()->SetKeyboardFocus();
				MainMenuHud->GetMainMenuSettingInstance()->GraphicButtonHovered();
				break;
			}
			case EMainSettingItem::Audio:
			{
				MainMenuHud->GetMainMenuSettingInstance()->GetAudioButton()->SetKeyboardFocus();
				MainMenuHud->GetMainMenuSettingInstance()->AudioButtonHovered();
				break;
			}
			case EMainSettingItem::MainMenu:
			{
				MainMenuHud->GetMainMenuSettingInstance()->GetMainMenuButton()->SetKeyboardFocus();
				MainMenuHud->GetMainMenuSettingInstance()->ReturnMainMenuButtonHovered();
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
		switch (InGraphicSettingItem)
		{
			case EGraphicSettingItem::WindowModeDown:
			{
				MainMenuHud->GetMainMenuSettingInstance()->GetGraphicSettingWidget()->GetWindowModeDownButton()->SetKeyboardFocus();
				MainMenuHud->GetMainMenuSettingInstance()->GetGraphicSettingWidget()->WindowModeDownHovered();
				break;
			}
			case EGraphicSettingItem::WindowModeUp:
			{
				MainMenuHud->GetMainMenuSettingInstance()->GetGraphicSettingWidget()->GetWindowModeUpButton()->SetKeyboardFocus();
				MainMenuHud->GetMainMenuSettingInstance()->GetGraphicSettingWidget()->WindowModeUpHovered();
				break;
			}
			case EGraphicSettingItem::ResolutionDown:
			{
				MainMenuHud->GetMainMenuSettingInstance()->GetGraphicSettingWidget()->GetResolutionDownButton()->SetKeyboardFocus();
				MainMenuHud->GetMainMenuSettingInstance()->GetGraphicSettingWidget()->ResolutionDownHovered();
				break;
			}
			case EGraphicSettingItem::ResolutionUp:
			{
				MainMenuHud->GetMainMenuSettingInstance()->GetGraphicSettingWidget()->GetResolutionUpButton()->SetKeyboardFocus();
				MainMenuHud->GetMainMenuSettingInstance()->GetGraphicSettingWidget()->ResolutionUpHovered();
				break;
			}
			case EGraphicSettingItem::GraphicDown:
			{
				MainMenuHud->GetMainMenuSettingInstance()->GetGraphicSettingWidget()->GetGraphicDownButton()->SetKeyboardFocus();
				MainMenuHud->GetMainMenuSettingInstance()->GetGraphicSettingWidget()->GraphicDownHovered();
				break;
			}
			case EGraphicSettingItem::GraphicUp:
			{
				MainMenuHud->GetMainMenuSettingInstance()->GetGraphicSettingWidget()->GetGraphicUpButton()->SetKeyboardFocus();
				MainMenuHud->GetMainMenuSettingInstance()->GetGraphicSettingWidget()->GraphicUpHovered();
				break;
			}
			case EGraphicSettingItem::VSyncDown:
			{
				MainMenuHud->GetMainMenuSettingInstance()->GetGraphicSettingWidget()->GetVSyncDownButton()->SetKeyboardFocus();
				MainMenuHud->GetMainMenuSettingInstance()->GetGraphicSettingWidget()->VSyncDownHovered();
				break;
			}
			case EGraphicSettingItem::VSyncUp:
			{
				MainMenuHud->GetMainMenuSettingInstance()->GetGraphicSettingWidget()->GetVSyncUpButton()->SetKeyboardFocus();
				MainMenuHud->GetMainMenuSettingInstance()->GetGraphicSettingWidget()->VSyncUpHovered();
				break;
			}
			case EGraphicSettingItem::Apply:
			{
				MainMenuHud->GetMainMenuSettingInstance()->GetGraphicSettingWidget()->GetApplyButton()->SetKeyboardFocus();
				MainMenuHud->GetMainMenuSettingInstance()->GetGraphicSettingWidget()->ApplyHovered();
				break;
			}
			case EGraphicSettingItem::Return:
			{
				MainMenuHud->GetMainMenuSettingInstance()->GetGraphicSettingWidget()->GetMainMenuButton()->SetKeyboardFocus();
				MainMenuHud->GetMainMenuSettingInstance()->GetGraphicSettingWidget()->ReturnMainMenuHovered();
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