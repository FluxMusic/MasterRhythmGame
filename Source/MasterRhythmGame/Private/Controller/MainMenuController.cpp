// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/MainMenuController.h"

#include <rapidjson/reader.h>

#include "EnhancedInputSubsystems.h"
#include "EnhancedInputSubsystemInterface.h"
#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"
#include "PropertyAccess.h"
#include "MIDIDevice/Public/MIDIDeviceInputController.h"
#include "MIDIDevice/Public/MIDIDeviceManager.h"

// Sets default values
AMainMenuController::AMainMenuController()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	InitInputAction();

	//MusicComponent = CreateDefaultSubobject<UAudioComponent>(*Music_Name);
}

//Called when the game starts or when spawned
void AMainMenuController::BeginPlay()
{
	Super::BeginPlay();

	InitMidi();

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		// add mapping context
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
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
	UE_LOG(LogTemp, Log, TEXT("Channel: %d"),  Channel);
	UE_LOG(LogTemp, Log, TEXT("Note: %d"),     Note);
	UE_LOG(LogTemp, Log, TEXT("Velocity: %d"), Velocity);

	Note = Note % 12;

	switch(ControllerState)
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
	UE_LOG(LogTemp, Log, TEXT("Channel: %d"),  Channel);
	UE_LOG(LogTemp, Log, TEXT("Note: %d"),     Note);
	UE_LOG(LogTemp, Log, TEXT("Velocity: %d"), Velocity);
}

void AMainMenuController::HandlePitchBend(UMIDIDeviceInputController* MIDIDeviceController, int32 Timestamp, int32 Channel, int32 Pitch)
{
	UE_LOG(LogTemp, Log, TEXT("Channel: %d"),  Channel);
	UE_LOG(LogTemp, Log, TEXT("Pitch: %d"),    Pitch);
}

void AMainMenuController::HandleAftertouch(UMIDIDeviceInputController* MIDIDeviceController, int32 Timestamp, int32 Channel, int32 Note, int32 Amount)
{
	UE_LOG(LogTemp, Log, TEXT("Channel: %d"),  Channel);
	UE_LOG(LogTemp, Log, TEXT("Note: %d"),     Note);
	UE_LOG(LogTemp, Log, TEXT("Amount: %d"),   Amount);
}

void AMainMenuController::HandleControlChange(UMIDIDeviceInputController* MIDIDeviceController, int32 Timestamp, int32 Channel, int32 Type, int32 Value)
{
	UE_LOG(LogTemp, Log, TEXT("Channel: %d"),  Channel);
	UE_LOG(LogTemp, Log, TEXT("Type: %d"),     Type);
	UE_LOG(LogTemp, Log, TEXT("Value: %d"),    Value);
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
			//TODO: Cast to MainMenuHUD -> Check which one of the buttons got focus then go into it
			break;
			}
		default:
			{
			// Should not land here
			break;
			}
	}
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
				//TODO: Cast to MainMenuHUD 
				break;
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

				//Switch
				break;
			}
		case 2:
			{
				SettingMenuIndex--;
				SettingMenuIndex = FMath::Clamp(SettingMenuIndex, 0, 2);
				UE_LOG(LogTemp, Log, TEXT("SettingMenuIndex: %d"), SettingMenuIndex);

				//Switch
				break;
			}
		case 11:
			{
				//TODO: Cast to MainMenuHUD
				break;
			}
		default:
			{
				// Should not land here
				break;
			}
	}
}

void AMainMenuController::CreditMenuControl(int32 Note)
{
	switch (Note)
	{
		case 0:
			{
				//TODO: Cast to MainMenuHUD
			}
		case 11:
			{
				//TODO: Cast to MainMenuHUD
			}
		default:
			{
				// Should not land here
				break;
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

			//Switch
			break;
		}
		case 2:
		{
			GraphicMenuIndex--;
			GraphicMenuIndex = FMath::Clamp(GraphicMenuIndex, 0, 9);
			UE_LOG(LogTemp, Log, TEXT("GraphicMenuIndex: %d"), GraphicMenuIndex);

			//Switch
			break;
		}
		case 11:
			{
				//TODO: Cast to MenuHUD
				break;
			}
		default:
		{
			//Should not land here
			break;
		}
	}
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
