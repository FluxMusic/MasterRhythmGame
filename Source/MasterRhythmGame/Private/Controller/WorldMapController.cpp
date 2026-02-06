// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/WorldMapController.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "MIDIDeviceManager.h"
#include "Character/WorldMapPlayerCharacter.h"
#include "Components/Button.h"
#include "Components/Slider.h"
#include "Controller/MainMenuController.h"
#include "GameInstance/MyGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "UI/ButtonWidget.h"
#include "WorldMap/LevelNode.h"

AWorldMapController::AWorldMapController()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bShowMouseCursor = false;
}

void AWorldMapController::BeginPlay()
{
	Super::BeginPlay();

	InitMidi();

	if (PlayerCharacterClass)
	{
		// Try to find an existing actor of the specified Blueprint class
		AActor* Found = UGameplayStatics::GetActorOfClass(GetWorld(), PlayerCharacterClass);
		PlayerCharacter = Cast<AWorldMapPlayerCharacter>(Found);
	}

	WorldMapHUD = Cast<AWorldMapHUD>(GetHUD());

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}
}

void AWorldMapController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(MoveForwardAction, ETriggerEvent::Started, this, &AWorldMapController::HandleMoveForward);
		EnhancedInputComponent->BindAction(MoveBackwardAction, ETriggerEvent::Started, this, &AWorldMapController::HandleMoveBackward);
		EnhancedInputComponent->BindAction(MoveLeftAction, ETriggerEvent::Started, this, &AWorldMapController::HandleMoveLeft);
		EnhancedInputComponent->BindAction(MoveRightAction, ETriggerEvent::Started, this, &AWorldMapController::HandleMoveRight);
		EnhancedInputComponent->BindAction(PauseAction, ETriggerEvent::Started, this, &AWorldMapController::HandleMoveForward);
		EnhancedInputComponent->BindAction(OpenLevelAction, ETriggerEvent::Started, this, &AWorldMapController::HandleOpenLevel);
		EnhancedInputComponent->BindAction(PauseAction, ETriggerEvent::Started, this, &AWorldMapController::HandlePause);
	}
}

void AWorldMapController::InitMidi()
{
	DisplayMidiDevicesIDs();

	CreateMidiController();
}

void AWorldMapController::DisplayMidiDevicesIDs()
{
	TArray<FFoundMIDIDevice> InputDevices;

	UMIDIDeviceManager::FindMIDIDevices(InputDevices);

	for (auto InputDevice : InputDevices)
	{
		UE_LOG(LogTemp, Log, TEXT("ID: %d"), InputDevice.DeviceID);
		UE_LOG(LogTemp, Log, TEXT("ID: %s"), *InputDevice.DeviceName);
	}
}

void AWorldMapController::CreateMidiController()
{
	MidiController = UMIDIDeviceManager::CreateMIDIDeviceInputController(1, 1024);

	if (MidiController != nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("Successfully connected with MIDI Controller."));
		MidiController->OnMIDINoteOn.AddDynamic(this, &AWorldMapController::HandleNoteOn);
		MidiController->OnMIDINoteOff.AddDynamic(this, &AWorldMapController::HandleNoteOff);
		MidiController->OnMIDIPitchBend.AddDynamic(this, &AWorldMapController::HandlePitchBend);
		MidiController->OnMIDIAftertouch.AddDynamic(this, &AWorldMapController::HandleAftertouch);
		MidiController->OnMIDIControlChange.AddDynamic(this, &AWorldMapController::HandleControlChange);
	}
	else
	{
		// Should not happen
		UE_LOG(LogTemp, Log, TEXT("Failed to connect with MIDI Controller."));
	}
}

void AWorldMapController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void AWorldMapController::HandleNoteOn(UMIDIDeviceInputController* MIDIDeviceController, int32 Timestamp, int32 Channel,
	int32 Note, int32 Velocity)
{
	// Convert incoming MIDI note number to semitone class (0-11) and map to ENote.
	const int32 Semitone = Note % 12;

	ENote NoteEnum = static_cast<ENote>(Semitone);

	switch (GetControllerState())
	{
		case EControllerStateWorldMap::WorldMap:
		{
			WorldMapControl(NoteEnum);
			break;
		}
		case EControllerStateWorldMap::PauseMenu:
		{
			PauseMenuControl(NoteEnum);
			break;
		}
		case EControllerStateWorldMap::SettingMenu:
		{
			SettingMenuControl(NoteEnum);
			break;
		}
		case EControllerStateWorldMap::AudioMenu:
		{
			AudioMenuControl(NoteEnum);
			break;
		}
		case EControllerStateWorldMap::GraphicsMenu:
		{
			GraphicMenuControl(NoteEnum);
			break;
		}
	}
}

void AWorldMapController::HandleNoteOff(UMIDIDeviceInputController* MIDIDeviceController, int32 Timestamp,
	int32 Channel, int32 Note, int32 Velocity)
{
	//UE_LOG(LogTemp, Log, TEXT("Channel: %d"),  Channel);
	//UE_LOG(LogTemp, Log, TEXT("Note: %d"),     Note);
	//UE_LOG(LogTemp, Log, TEXT("Velocity: %d"), Velocity);
}

void AWorldMapController::HandlePitchBend(UMIDIDeviceInputController* MIDIDeviceController, int32 Timestamp,
	int32 Channel, int32 Pitch)
{
	//UE_LOG(LogTemp, Log, TEXT("Channel: %d"),  Channel);
	//UE_LOG(LogTemp, Log, TEXT("Pitch: %d"),    Pitch);
}

void AWorldMapController::HandleAftertouch(UMIDIDeviceInputController* MIDIDeviceController, int32 Timestamp,
	int32 Channel, int32 Note, int32 Amount)
{
	//UE_LOG(LogTemp, Log, TEXT("Channel: %d"),  Channel);
	//UE_LOG(LogTemp, Log, TEXT("Note: %d"),     Note);
	//UE_LOG(LogTemp, Log, TEXT("Amount: %d"),   Amount);
	//UE_LOG(LogTemp, Log, TEXT("Amount: %d"),   Amount);
}

void AWorldMapController::HandleControlChange(UMIDIDeviceInputController* MIDIDeviceController, int32 Timestamp,
	int32 Channel, int32 Type, int32 Value)
{
	float NormalizedValue = static_cast<float>(Value) / 127.0f;
	NormalizedValue = FMath::Clamp(NormalizedValue, 0.0f, 1.0f);

	AudioSoundControl(Type, NormalizedValue);
}

void AWorldMapController::HandleMoveForward()
{
	if (PlayerCharacter->GetIsMoving())
	{
		return;
	}

	if (PlayerCharacter->GetLevelNodeRef()->GetSplineForward().Spline != nullptr && PlayerCharacter->GetLevelNodeRef()->GetSplineForward().bIsUnlocked)
	{
		PlayerCharacter->SetSplineRef(PlayerCharacter->GetLevelNodeRef()->GetSplineForward().Spline);
		PlayerCharacter->Move(PlayerCharacter->GetLevelNodeRef()->GetSplineForward().DirectionWorldMap);
	}
}

void AWorldMapController::HandleMoveBackward()
{
	if (PlayerCharacter->GetIsMoving())
	{
		return;
	}

	if (PlayerCharacter->GetLevelNodeRef()->GetSplineBackward().Spline != nullptr)
	{
		PlayerCharacter->SetSplineRef(PlayerCharacter->GetLevelNodeRef()->GetSplineBackward().Spline);
		PlayerCharacter->Move(PlayerCharacter->GetLevelNodeRef()->GetSplineBackward().DirectionWorldMap);
	}
}

void AWorldMapController::HandleMoveLeft()
{
	if (PlayerCharacter->GetIsMoving())
	{
		return;
	}

	if (PlayerCharacter->GetLevelNodeRef()->GetSplineLeft().Spline != nullptr && PlayerCharacter->GetLevelNodeRef()->GetSplineLeft().bIsUnlocked)
	{
		PlayerCharacter->SetSplineRef(PlayerCharacter->GetLevelNodeRef()->GetSplineLeft().Spline);
		PlayerCharacter->Move(PlayerCharacter->GetLevelNodeRef()->GetSplineLeft().DirectionWorldMap);
	}
}

void AWorldMapController::HandleMoveRight()
{
	if (PlayerCharacter->GetIsMoving())
	{
		return;
	}

	if (PlayerCharacter->GetLevelNodeRef()->GetSplineRight().Spline != nullptr && PlayerCharacter->GetLevelNodeRef()->GetSplineRight().bIsUnlocked)
	{
		PlayerCharacter->SetSplineRef(PlayerCharacter->GetLevelNodeRef()->GetSplineRight().Spline);
		PlayerCharacter->Move(PlayerCharacter->GetLevelNodeRef()->GetSplineRight().DirectionWorldMap);
	}
}

void AWorldMapController::HandlePause()
{
	SetControllerState(EControllerStateWorldMap::PauseMenu);

	// Toggle pause state
	bool bCurrentlyPaused = UGameplayStatics::IsGamePaused(GetWorld());
	UGameplayStatics::SetGamePaused(GetWorld(), !bCurrentlyPaused);

	if (bCurrentlyPaused == false)
	{
		if (WorldMapHUD != nullptr)
		{
			WorldMapHUD->GetPauseMenuInstance()->SetVisibility(ESlateVisibility::Visible);
		}

		// Show mouse cursor and switch to GameAndUI input so widgets receive focus
		bShowMouseCursor = true;
		FInputModeGameAndUI InputMode;
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		SetInputMode(InputMode);
	}
}

void AWorldMapController::HandleOpenLevel()
{
	UGameplayStatics::OpenLevelBySoftObjectPtr(this, PlayerCharacter->GetLevelNodeRef()->GetLevelToLoad());
}

void AWorldMapController::WorldMapControl(ENote Note)
{
	if (WorldMapHUD != nullptr)
	{
		if (PlayerCharacter == nullptr)
		{
			return;
		}

		if (PlayerCharacter->GetIsMoving())
		{
			return;
		}

		switch (Note)
		{
			// Left
			case ENote::E:
			{
				if (PlayerCharacter->GetLevelNodeRef()->GetSplineLeft().Spline != nullptr && PlayerCharacter->GetLevelNodeRef()->GetSplineLeft().bIsUnlocked)
				{
					PlayerCharacter->SetSplineRef(PlayerCharacter->GetLevelNodeRef()->GetSplineLeft().Spline);
					PlayerCharacter->Move(PlayerCharacter->GetLevelNodeRef()->GetSplineLeft().DirectionWorldMap);
					break;
				}
				break;
			}
			// Backward
			case ENote::F:
			{
				if (PlayerCharacter->GetLevelNodeRef()->GetSplineBackward().Spline != nullptr)
				{
					PlayerCharacter->SetSplineRef(PlayerCharacter->GetLevelNodeRef()->GetSplineBackward().Spline);
					PlayerCharacter->Move(PlayerCharacter->GetLevelNodeRef()->GetSplineBackward().DirectionWorldMap);
					break;
				}
				break;
			}
			// Right
			case ENote::G:
			{
				if (PlayerCharacter->GetLevelNodeRef()->GetSplineRight().Spline != nullptr && PlayerCharacter->GetLevelNodeRef()->GetSplineRight().bIsUnlocked)
				{
					PlayerCharacter->SetSplineRef(PlayerCharacter->GetLevelNodeRef()->GetSplineRight().Spline);
					 PlayerCharacter->Move(PlayerCharacter->GetLevelNodeRef()->GetSplineRight().DirectionWorldMap);
					break;
				}
				break;
			}
			// Forward
			case ENote::FSharp:
			{
				if (PlayerCharacter->GetLevelNodeRef()->GetSplineForward().Spline != nullptr && PlayerCharacter->GetLevelNodeRef()->GetSplineForward().bIsUnlocked)
				{
					PlayerCharacter->SetSplineRef(PlayerCharacter->GetLevelNodeRef()->GetSplineForward().Spline);
					PlayerCharacter->Move(PlayerCharacter->GetLevelNodeRef()->GetSplineForward().DirectionWorldMap);
					break;
				}
				break;
			}
			case ENote::CSharp:
			{
				SetControllerState(EControllerStateWorldMap::PauseMenu);
				if (WorldMapHUD != nullptr)
				{
					WorldMapHUD->GetPauseMenuInstance()->SetVisibility(ESlateVisibility::Visible);
					// TODO: Change UI first 
					//WorldMapHUD->GetWorldMapInstance()->SetVisibility(ESlateVisibility::Hidden);
				}
				break;
			}
			case ENote::B:
			{
				if (UMyGameInstance* GI = UMyGameInstance::Get())
				{
					GI->SetCurrentLevel(PlayerCharacter->GetLevelNodeRef()->GetLevelNodeLevel());
				}
				
				UGameplayStatics::OpenLevelBySoftObjectPtr(this, PlayerCharacter->GetLevelNodeRef()->GetLevelToLoad());
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

void AWorldMapController::PauseMenuControl(ENote Note)
{
	if (WorldMapHUD != nullptr)
	{
		switch (Note)
		{
			case ENote::C:
			{
				PauseMenuIndex++;
				PauseMenuIndex = FMath::Clamp(PauseMenuIndex, 0, 5);
				UE_LOG(LogTemp, Log, TEXT("PauseMenuIndex: %d"), PauseMenuIndex);
				break;
			}
			case ENote::D:
			{
				PauseMenuIndex--;
				PauseMenuIndex = FMath::Clamp(PauseMenuIndex, 0, 5);
				UE_LOG(LogTemp, Log, TEXT("PauseMenuIndex: %d"), PauseMenuIndex); break;
			}
			case ENote::B:
			{
				if (WorldMapHUD->GetPauseMenuInstance()->GetResumeButton()->HasKeyboardFocus())
				{
					PauseMenuIndex = 0;
					WorldMapHUD->GetPauseMenuInstance()->ResumeButtonClicked();
					break;
				}
				if (WorldMapHUD->GetPauseMenuInstance()->GetSaveGameButton()->HasKeyboardFocus())
				{
					PauseMenuIndex = 0;
					WorldMapHUD->GetPauseMenuInstance()->SaveGameButtonClicked();
					break;
				}
				if (WorldMapHUD->GetPauseMenuInstance()->GetLoadGameButton()->HasKeyboardFocus())
				{
					PauseMenuIndex = 0;
					WorldMapHUD->GetPauseMenuInstance()->LoadGameButtonClicked();
					break;
				}
				if (WorldMapHUD->GetPauseMenuInstance()->GetSettingsButton()->HasKeyboardFocus())
				{
					PauseMenuIndex = 0;
					WorldMapHUD->GetPauseMenuInstance()->SettingsButtonClicked();
					break;
				}
				if (WorldMapHUD->GetPauseMenuInstance()->GetMainMenuButton()->HasKeyboardFocus())
				{
					PauseMenuIndex = 0;
					WorldMapHUD->GetPauseMenuInstance()->MainMenuButtonClicked();
					break;
				}
				if (WorldMapHUD->GetPauseMenuInstance()->GetQuitButton()->HasKeyboardFocus())
				{
					PauseMenuIndex = 0;
					WorldMapHUD->GetPauseMenuInstance()->QuitButtonClicked();
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
	PauseMenuSwitchButton(static_cast<EPauseMenuItem>(PauseMenuIndex));
}

void AWorldMapController::MainSettingSwitchButton(EMainSettingItem InMainSettingItem)
{
	if (WorldMapHUD != nullptr)
	{
		switch (InMainSettingItem)
		{
			case EMainSettingItem::Graphic:
			{
				WorldMapHUD->GetMainMenuSettingInstance()->GetGraphicButton()->SetKeyboardFocus();
				WorldMapHUD->GetMainMenuSettingInstance()->GetGraphicButton()->HandleButtonHovered();
				break;
			}
			case EMainSettingItem::Audio:
			{
				WorldMapHUD->GetMainMenuSettingInstance()->GetAudioButton()->SetKeyboardFocus();
				WorldMapHUD->GetMainMenuSettingInstance()->GetAudioButton()->HandleButtonHovered();
				break;
			}
			case EMainSettingItem::MainMenu:
			{
				WorldMapHUD->GetMainMenuSettingInstance()->GetMainMenuButton()->SetKeyboardFocus();
				WorldMapHUD->GetMainMenuSettingInstance()->GetMainMenuButton()->HandleButtonHovered();
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

void AWorldMapController::GraphicMenuSwitchButton(EGraphicSettingItem InGraphicSettingItem)
{
	if (WorldMapHUD != nullptr)
	{
		switch (InGraphicSettingItem)
		{
			case EGraphicSettingItem::WindowModeDown:
			{
				WorldMapHUD->GetMainMenuSettingInstance()->GetGraphicSettingWidget()->GetWindowModeDownButton()->SetKeyboardFocus();
				WorldMapHUD->GetMainMenuSettingInstance()->GetGraphicSettingWidget()->GetWindowModeDownButton()->HandleButtonHovered();
				break;
			}
			case EGraphicSettingItem::WindowModeUp:
			{
				WorldMapHUD->GetMainMenuSettingInstance()->GetGraphicSettingWidget()->GetWindowModeUpButton()->SetKeyboardFocus();
				WorldMapHUD->GetMainMenuSettingInstance()->GetGraphicSettingWidget()->GetWindowModeUpButton()->HandleButtonHovered();
				break;
			}
			case EGraphicSettingItem::ResolutionDown:
			{
				WorldMapHUD->GetMainMenuSettingInstance()->GetGraphicSettingWidget()->GetResolutionDownButton()->SetKeyboardFocus();
				WorldMapHUD->GetMainMenuSettingInstance()->GetGraphicSettingWidget()->GetResolutionDownButton()->HandleButtonHovered();
				break;
			}
			case EGraphicSettingItem::ResolutionUp:
			{
				WorldMapHUD->GetMainMenuSettingInstance()->GetGraphicSettingWidget()->GetResolutionUpButton()->SetKeyboardFocus();
				WorldMapHUD->GetMainMenuSettingInstance()->GetGraphicSettingWidget()->GetResolutionUpButton()->HandleButtonHovered();
				break;
			}
			case EGraphicSettingItem::GraphicDown:
			{
				WorldMapHUD->GetMainMenuSettingInstance()->GetGraphicSettingWidget()->GetGraphicDownButton()->SetKeyboardFocus();
				WorldMapHUD->GetMainMenuSettingInstance()->GetGraphicSettingWidget()->GetGraphicDownButton()->HandleButtonHovered();
				break;
			}
			case EGraphicSettingItem::GraphicUp:
			{
				WorldMapHUD->GetMainMenuSettingInstance()->GetGraphicSettingWidget()->GetGraphicUpButton()->SetKeyboardFocus();
				WorldMapHUD->GetMainMenuSettingInstance()->GetGraphicSettingWidget()->GetGraphicUpButton()->HandleButtonHovered();
				break;
			}
			case EGraphicSettingItem::VSyncDown:
			{
				WorldMapHUD->GetMainMenuSettingInstance()->GetGraphicSettingWidget()->GetVSyncDownButton()->SetKeyboardFocus();
				WorldMapHUD->GetMainMenuSettingInstance()->GetGraphicSettingWidget()->GetVSyncDownButton()->HandleButtonHovered();
				break;
			}
			case EGraphicSettingItem::VSyncUp:
			{
				WorldMapHUD->GetMainMenuSettingInstance()->GetGraphicSettingWidget()->GetVSyncUpButton()->SetKeyboardFocus();
				WorldMapHUD->GetMainMenuSettingInstance()->GetGraphicSettingWidget()->GetVSyncUpButton()->HandleButtonHovered();
				break;
			}
			case EGraphicSettingItem::Apply:
			{
				WorldMapHUD->GetMainMenuSettingInstance()->GetGraphicSettingWidget()->GetApplyButton()->SetKeyboardFocus();
				WorldMapHUD->GetMainMenuSettingInstance()->GetGraphicSettingWidget()->GetApplyButton()->HandleButtonHovered();
				break;
			}
			case EGraphicSettingItem::Return:
			{
				WorldMapHUD->GetMainMenuSettingInstance()->GetGraphicSettingWidget()->GetMainMenuButton()->SetKeyboardFocus();
				WorldMapHUD->GetMainMenuSettingInstance()->GetGraphicSettingWidget()->GetMainMenuButton()->HandleButtonHovered();
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

void AWorldMapController::AudioMenuControl(ENote Note)
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
			if (WorldMapHUD != nullptr)
			{
				if (WorldMapHUD->GetMainMenuSettingInstance()->GetAudioSettingWidget()->GetMasterVolumeSlider()->HasKeyboardFocus())
				{
					AudioMenuIndex = 0;
					break;
				}
				if (WorldMapHUD->GetMainMenuSettingInstance()->GetAudioSettingWidget()->GetMusicVolumeSlider()->HasKeyboardFocus())
				{
					AudioMenuIndex = 0;
					break;
				}
				if (WorldMapHUD->GetMainMenuSettingInstance()->GetAudioSettingWidget()->GetSfxVolumeSlider()->HasKeyboardFocus())
				{
					AudioMenuIndex = 0;
					break;
				}
				if (WorldMapHUD->GetMainMenuSettingInstance()->GetAudioSettingWidget()->GetMainMenuButton()->GetButton()->HasKeyboardFocus())
				{
					AudioMenuIndex = 0;
					WorldMapHUD->GetMainMenuSettingInstance()->GetAudioSettingWidget()->ReturnMenu();
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

void AWorldMapController::SettingMenuControl(ENote Note)
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
			if (WorldMapHUD != nullptr)
			{
				if (WorldMapHUD->GetMainMenuSettingInstance()->GetGraphicButton()->HasKeyboardFocus())
				{
					SetControllerState(EControllerStateWorldMap::GraphicsMenu);
					SettingMenuIndex = 0;
					GraphicMenuIndex = 0;
					WorldMapHUD->GetMainMenuSettingInstance()->GraphicSettingClicked();
					break;
				}
				if (WorldMapHUD->GetMainMenuSettingInstance()->GetAudioButton()->HasKeyboardFocus())
				{
					SetControllerState(EControllerStateWorldMap::AudioMenu);
					SettingMenuIndex = 0;
					AudioMenuIndex = 0;
					WorldMapHUD->GetMainMenuSettingInstance()->AudioSettingClicked();
					break;
				}
				if (WorldMapHUD->GetMainMenuSettingInstance()->GetMainMenuButton()->HasKeyboardFocus())
				{
					SetControllerState(EControllerStateWorldMap::PauseMenu);
					SettingMenuIndex = 0;
					PauseMenuIndex = 0;
					WorldMapHUD->GetMainMenuSettingInstance()->ReturnMainMenuClicked();
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
	MainSettingSwitchButton(static_cast<EMainSettingItem>(SettingMenuIndex));
}

void AWorldMapController::GraphicMenuControl(ENote Note)
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
			if (WorldMapHUD != nullptr)
			{
				if (WorldMapHUD->GetMainMenuSettingInstance()->GetGraphicSettingWidget()->GetWindowModeDownButton()->HasKeyboardFocus())
				{
					WorldMapHUD->GetMainMenuSettingInstance()->GetGraphicSettingWidget()->WindowModeDownClicked();
					break;
				}
				if (WorldMapHUD->GetMainMenuSettingInstance()->GetGraphicSettingWidget()->GetWindowModeUpButton()->HasKeyboardFocus())
				{
					WorldMapHUD->GetMainMenuSettingInstance()->GetGraphicSettingWidget()->WindowModeUpClicked();
					break;
				}
				if (WorldMapHUD->GetMainMenuSettingInstance()->GetGraphicSettingWidget()->GetResolutionDownButton()->HasKeyboardFocus())
				{
					WorldMapHUD->GetMainMenuSettingInstance()->GetGraphicSettingWidget()->ResolutionDownClicked();
					break;
				}
				if (WorldMapHUD->GetMainMenuSettingInstance()->GetGraphicSettingWidget()->GetResolutionUpButton()->HasKeyboardFocus())
				{
					WorldMapHUD->GetMainMenuSettingInstance()->GetGraphicSettingWidget()->ResolutionUpClicked();
					break;
				}
				if (WorldMapHUD->GetMainMenuSettingInstance()->GetGraphicSettingWidget()->GetGraphicDownButton()->HasKeyboardFocus())
				{
					WorldMapHUD->GetMainMenuSettingInstance()->GetGraphicSettingWidget()->GraphicDownClicked();
					break;
				}
				if (WorldMapHUD->GetMainMenuSettingInstance()->GetGraphicSettingWidget()->GetGraphicUpButton()->HasKeyboardFocus())
				{
					WorldMapHUD->GetMainMenuSettingInstance()->GetGraphicSettingWidget()->GraphicUpClicked();
					break;
				}
				if (WorldMapHUD->GetMainMenuSettingInstance()->GetGraphicSettingWidget()->GetVSyncDownButton()->HasKeyboardFocus())
				{
					WorldMapHUD->GetMainMenuSettingInstance()->GetGraphicSettingWidget()->VSyncDownClicked();
					break;
				}
				if (WorldMapHUD->GetMainMenuSettingInstance()->GetGraphicSettingWidget()->GetVSyncUpButton()->HasKeyboardFocus())
				{
					WorldMapHUD->GetMainMenuSettingInstance()->GetGraphicSettingWidget()->VSyncUpClicked();
					break;
				}
				if (WorldMapHUD->GetMainMenuSettingInstance()->GetGraphicSettingWidget()->GetApplyButton()->HasKeyboardFocus())
				{
					WorldMapHUD->GetMainMenuSettingInstance()->GetGraphicSettingWidget()->ApplyClicked();
					break;
				}
				if (WorldMapHUD->GetMainMenuSettingInstance()->GetGraphicSettingWidget()->GetMainMenuButton()->HasKeyboardFocus())
				{
					GraphicMenuIndex = 0;
					WorldMapHUD->GetMainMenuSettingInstance()->GetGraphicSettingWidget()->GetMainMenuButton()->HandleButtonUnhovered();
					WorldMapHUD->GetMainMenuSettingInstance()->GetGraphicSettingWidget()->ReturnMainMenuClicked();
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

void AWorldMapController::SwitchAudioMenuButton(EAudioSettingItem InAudioSettingItem)
{
	if (WorldMapHUD != nullptr)
	{
		switch (InAudioSettingItem)
		{
			case EAudioSettingItem::MasterVolumeSlider:
			{
				WorldMapHUD->GetMainMenuSettingInstance()->GetAudioSettingWidget()->GetMasterVolumeSlider()->SetKeyboardFocus();
				WorldMapHUD->GetMainMenuSettingInstance()->GetAudioSettingWidget()->MasterVolumeSliderHovered();

				break;
			}
			case EAudioSettingItem::MusicVolumeSlider:
			{
				WorldMapHUD->GetMainMenuSettingInstance()->GetAudioSettingWidget()->GetMusicVolumeSlider()->SetKeyboardFocus();
				WorldMapHUD->GetMainMenuSettingInstance()->GetAudioSettingWidget()->MusicVolumeSliderHovered();
				break;
			}
			case EAudioSettingItem::SfxVolumeSlider:
			{
				WorldMapHUD->GetMainMenuSettingInstance()->GetAudioSettingWidget()->GetSfxVolumeSlider()->SetKeyboardFocus();
				WorldMapHUD->GetMainMenuSettingInstance()->GetAudioSettingWidget()->SfxVolumeSliderHovered();

				break;
			}
			case EAudioSettingItem::Return:
			{
				WorldMapHUD->GetMainMenuSettingInstance()->GetAudioSettingWidget()->GetMainMenuButton()->GetButton()->SetKeyboardFocus();
				WorldMapHUD->GetMainMenuSettingInstance()->GetAudioSettingWidget()->GetMainMenuButton()->HandleButtonHovered();
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

void AWorldMapController::AudioSoundControl(int32 Type, float SoundValue)
{
	// Modwheel
	if (Type == 1)
	{
		if (WorldMapHUD != nullptr)
		{
			// TODO: Safe the Value in the GameInstance later!
			if (WorldMapHUD->GetMainMenuSettingInstance()->GetAudioSettingWidget()->GetMasterVolumeSlider()->HasKeyboardFocus())
			{
				WorldMapHUD->GetMainMenuSettingInstance()->GetAudioSettingWidget()->GetMasterVolumeSlider()->SetValue(SoundValue);
			}
			if (WorldMapHUD->GetMainMenuSettingInstance()->GetAudioSettingWidget()->GetMusicVolumeSlider()->HasKeyboardFocus())
			{
				WorldMapHUD->GetMainMenuSettingInstance()->GetAudioSettingWidget()->GetMusicVolumeSlider()->SetValue(SoundValue);
			}
			if (WorldMapHUD->GetMainMenuSettingInstance()->GetAudioSettingWidget()->GetSfxVolumeSlider()->HasKeyboardFocus())
			{
				WorldMapHUD->GetMainMenuSettingInstance()->GetAudioSettingWidget()->GetSfxVolumeSlider()->SetValue(SoundValue);
			}
		}
	}
}

void AWorldMapController::PauseMenuSwitchButton(EPauseMenuItem InPauseMenuItem)
{
	if (WorldMapHUD != nullptr)
	{
		switch (InPauseMenuItem)
		{
			case EPauseMenuItem::Resume:
			{
				WorldMapHUD->GetPauseMenuInstance()->GetResumeButton()->SetKeyboardFocus();
				WorldMapHUD->GetPauseMenuInstance()->GetResumeButton()->HandleButtonHovered();
				break;
			}
			case EPauseMenuItem::Save:
			{
				WorldMapHUD->GetPauseMenuInstance()->GetSaveGameButton()->SetKeyboardFocus();
				WorldMapHUD->GetPauseMenuInstance()->GetSaveGameButton()->HandleButtonHovered();
				break;
			}
			case EPauseMenuItem::Load:
			{
				WorldMapHUD->GetPauseMenuInstance()->GetLoadGameButton()->SetKeyboardFocus();
				WorldMapHUD->GetPauseMenuInstance()->GetLoadGameButton()->HandleButtonHovered();
				break;
			}
			case EPauseMenuItem::Settings:
			{
				WorldMapHUD->GetPauseMenuInstance()->GetSettingsButton()->SetKeyboardFocus();
				WorldMapHUD->GetPauseMenuInstance()->GetSettingsButton()->HandleButtonHovered();
				break;
			}
			case EPauseMenuItem::MainMenu:
			{
				WorldMapHUD->GetPauseMenuInstance()->GetMainMenuButton()->SetKeyboardFocus();
				WorldMapHUD->GetPauseMenuInstance()->GetMainMenuButton()->HandleButtonHovered();
				break;
			}
			case EPauseMenuItem::Quit:
			{
				WorldMapHUD->GetPauseMenuInstance()->GetQuitButton()->SetKeyboardFocus();
				WorldMapHUD->GetPauseMenuInstance()->GetQuitButton()->HandleButtonHovered();
				break;
			}
		}
	}
}