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
	//UE_LOG(LogTemp, Log, TEXT("Channel: %d"),  Channel);
	//UE_LOG(LogTemp, Log, TEXT("Note: %d"),     Note);
	//UE_LOG(LogTemp, Log, TEXT("Velocity: %d"), Velocity);

	Note = Note % 12;

	switch(GetControllerState())
	{
	case EControllerState::MainMenu:
		{
			MainMenuControl(Note);
			break;
		}
	case EControllerState::AudioMenu:
		{
			AudioMenuControl(Note);
			break;
		}
	case EControllerState::CreditsMenu:
		{
			CreditMenuControl(Note);
			break;
		}
	case EControllerState::GraphicsMenu:
		{
			GraphicMenuControl(Note);
			break;
		}
	case EControllerState::LoadMenu:
		{
			LoadMenuControl(Note);
			break;
		}
	case EControllerState::SettingMenu:
		{
			SettingMenuControl(Note);
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

void AMainMenuController::MainMenuControl(int32 Note)
{
	switch(Note)
	{
		case 0:
		{
			MainMenuIndex++;
			MainMenuIndex = FMath::Clamp(MainMenuIndex, 0, 4);
			UE_LOG(LogTemp, Log, TEXT("MainMenuIndex: %d"), MainMenuIndex);

			// Switch
			break;
		}
		case 2:
		{
			MainMenuIndex--;
			MainMenuIndex = FMath::Clamp(MainMenuIndex, 0, 4);
			UE_LOG(LogTemp, Log, TEXT("MainMenuIndex: %d"), MainMenuIndex);

			// Switch 
			break;
		}
		case 11:
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
	MainMenuSwitchButton(MainMenuIndex);
}

void AMainMenuController::AudioMenuControl(int32 Note)
{
	switch (Note)
	{
		case 0:
		{
		AudioMenuIndex++;
		AudioMenuIndex = FMath::Clamp(AudioMenuIndex, 0, 3);
		UE_LOG(LogTemp, Log, TEXT("AudioMenuIndex: %d"), AudioMenuIndex);

		//Switch
		break;

		}
		case 2:
		{
		AudioMenuIndex--;
		AudioMenuIndex = FMath::Clamp(AudioMenuIndex, 0, 3);
		UE_LOG(LogTemp, Log, TEXT("AudioMenuIndex: %d"), AudioMenuIndex);

		//Switch
		break;
		}
		case 11:
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

void AMainMenuController::SettingMenuControl(int32 Note)
{
	switch (Note)
	{
		case 0:
			{
				SettingMenuIndex++;
				SettingMenuIndex = FMath::Clamp(SettingMenuIndex, 0, 2);
				UE_LOG(LogTemp, Log, TEXT("SettingMenuIndex: %d"), SettingMenuIndex);
				break;
			}
		case 2:
			{
				SettingMenuIndex--;
				SettingMenuIndex = FMath::Clamp(SettingMenuIndex, 0, 2);
				UE_LOG(LogTemp, Log, TEXT("SettingMenuIndex: %d"), SettingMenuIndex);
				break;
			}
		case 11:
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
	MainSettingSwitchButton(SettingMenuIndex);
}

void AMainMenuController::CreditMenuControl(int32 Note)
{
	if (MainMenuHud != nullptr)
	{
		MainMenuHud->GetCreditsMenuInstance()->GetReturnMainMenuButton()->SetKeyboardFocus();
		switch (Note)
		{
			case 11:
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

void AMainMenuController::GraphicMenuControl(int32 Note)
{
	switch (Note)
	{
		case 0:
		{
			GraphicMenuIndex++;
			GraphicMenuIndex = FMath::Clamp(GraphicMenuIndex, 0, 9);
			UE_LOG(LogTemp, Log, TEXT("GraphicMenuIndex: %d"), GraphicMenuIndex);
			break;
		}
		case 2:
		{
			GraphicMenuIndex--;
			GraphicMenuIndex = FMath::Clamp(GraphicMenuIndex, 0, 9);
			UE_LOG(LogTemp, Log, TEXT("GraphicMenuIndex: %d"), GraphicMenuIndex);
			break;
		}
		case 11:
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
	GraphicMenuSwitchButton(GraphicMenuIndex);
}

void AMainMenuController::LoadMenuControl(int32 Note)
{
	switch (Note)
	{
		case 0:
			{
			LoadMenuIndex++;
			LoadMenuIndex = FMath::Clamp(LoadMenuIndex, 0, 4);
			UE_LOG(LogTemp, Log, TEXT("LoadMenuIndex: %d"), LoadMenuIndex);

			//Switch
			break;
			}
		case 2:
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

void AMainMenuController::MainMenuSwitchButton(int32 InMainMenuIndex)
{
	if (MainMenuHud != nullptr)
	{
		switch (InMainMenuIndex)
		{
			// New Game
			case 0:
			{
				MainMenuHud->GetMainMenuInstance()->GetNewGameButton()->SetKeyboardFocus();
				MainMenuHud->GetMainMenuInstance()->NewGameHovered();
				break;
			}
			// Load Game
			case 1:
			{
				MainMenuHud->GetMainMenuInstance()->GetLoadGameButton()->SetKeyboardFocus();
				MainMenuHud->GetMainMenuInstance()->LoadGameHovered();
				break;
			}
			// Setting
			case 2:
			{
				MainMenuHud->GetMainMenuInstance()->GetSettingButton()->SetKeyboardFocus();
				MainMenuHud->GetMainMenuInstance()->SettingHovered();
				break;
			}
			// Credit
			case 3:
			{
				MainMenuHud->GetMainMenuInstance()->GetCreditsButton()->SetKeyboardFocus();
				MainMenuHud->GetMainMenuInstance()->CreditHovered();
				break;
			}
			// Escape
			case 4:
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

void AMainMenuController::MainSettingSwitchButton(int32 InMainSettingIndex)
{
	if (MainMenuHud != nullptr)
	{
		switch (InMainSettingIndex)
		{
			case 0:
			{
				MainMenuHud->GetMainMenuSettingInstance()->GetGraphicButton()->SetKeyboardFocus();
				MainMenuHud->GetMainMenuSettingInstance()->GraphicButtonHovered();
				break;
			}
			case 1:
			{
				MainMenuHud->GetMainMenuSettingInstance()->GetAudioButton()->SetKeyboardFocus();
				MainMenuHud->GetMainMenuSettingInstance()->AudioButtonHovered();
				break;
			}
			case 2:
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

void AMainMenuController::GraphicMenuSwitchButton(int32 InGraphicSettingIndex)
{
	if (MainMenuHud != nullptr)
	{
		switch (InGraphicSettingIndex)
		{
			// Window Mode Down
			case 0:
			{
				MainMenuHud->GetMainMenuSettingInstance()->GetGraphicSettingWidget()->GetWindowModeDownButton()->SetKeyboardFocus();
				MainMenuHud->GetMainMenuSettingInstance()->GetGraphicSettingWidget()->WindowModeDownHovered();
				break;
			}
			// Window Mode Up
			case 1:
			{
				MainMenuHud->GetMainMenuSettingInstance()->GetGraphicSettingWidget()->GetWindowModeUpButton()->SetKeyboardFocus();
				MainMenuHud->GetMainMenuSettingInstance()->GetGraphicSettingWidget()->WindowModeUpHovered();
				break;
			}
			// Resolution Mode Down
			case 2:
			{
				MainMenuHud->GetMainMenuSettingInstance()->GetGraphicSettingWidget()->GetResolutionDownButton()->SetKeyboardFocus();
				MainMenuHud->GetMainMenuSettingInstance()->GetGraphicSettingWidget()->ResolutionDownHovered();
				break;
			}
			// Resolution Mode Up
			case 3:
			{
				MainMenuHud->GetMainMenuSettingInstance()->GetGraphicSettingWidget()->GetResolutionUpButton()->SetKeyboardFocus();
				MainMenuHud->GetMainMenuSettingInstance()->GetGraphicSettingWidget()->ResolutionUpHovered();
				break;
			}
			// Graphic Mode Down
			case 4:
			{
				MainMenuHud->GetMainMenuSettingInstance()->GetGraphicSettingWidget()->GetGraphicDownButton()->SetKeyboardFocus();
				MainMenuHud->GetMainMenuSettingInstance()->GetGraphicSettingWidget()->GraphicDownHovered();
				break;
			}
			// Graphic Mode Up
			case 5:
			{
				MainMenuHud->GetMainMenuSettingInstance()->GetGraphicSettingWidget()->GetGraphicUpButton()->SetKeyboardFocus();
				MainMenuHud->GetMainMenuSettingInstance()->GetGraphicSettingWidget()->GraphicUpHovered();
				break;
			}
			// VSync Mode Down
			case 6:
			{
				MainMenuHud->GetMainMenuSettingInstance()->GetGraphicSettingWidget()->GetVSyncDownButton()->SetKeyboardFocus();
				MainMenuHud->GetMainMenuSettingInstance()->GetGraphicSettingWidget()->VSyncDownHovered();
				break;
			}
			// VSync Mode Up
			case 7:
			{
				MainMenuHud->GetMainMenuSettingInstance()->GetGraphicSettingWidget()->GetVSyncUpButton()->SetKeyboardFocus();
				MainMenuHud->GetMainMenuSettingInstance()->GetGraphicSettingWidget()->VSyncUpHovered();
				break;
			}
			// Apply Button
			case 8:
			{
				MainMenuHud->GetMainMenuSettingInstance()->GetGraphicSettingWidget()->GetApplyButton()->SetKeyboardFocus();
				MainMenuHud->GetMainMenuSettingInstance()->GetGraphicSettingWidget()->ApplyHovered();
				break;
			}
			// Return Button
			case 9:
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
