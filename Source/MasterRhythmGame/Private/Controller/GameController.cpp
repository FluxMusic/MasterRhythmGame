// Fill out your copyright notice in the Description page of Project Settings.


#include "Controller/GameController.h"
#include "EnhancedInputSubsystems.h"
#include "MIDIDeviceManager.h"
#include "Character/GameCharacter.h"
#include "Components/Button.h"
#include "Components/CapsuleComponent.h"
#include "Components/Slider.h"
#include "Components/SplineComponent.h"
#include "Controller/MainMenuController.h"
#include "Controller/WorldMapController.h"
#include "HUD/GameHUD.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "EnhancedInputComponent.h"
#include "Actor/NodeActor.h"
#include "Actor/SplineActor.h"
#include "Components/TimelineComponent.h"
#include "Manager/AudioManagerSubsystem.h"
#include "Enemy/TestEnemyActor.h"

AGameController::AGameController()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AGameController::BeginPlay()
{
	Super::BeginPlay();

	InitMidi();

	GameHud = Cast<AGameHUD>(GetHUD());
	
	if (GameCharacterBPClass)
	{
		// Try to find an existing actor of the specified Blueprint class
		AActor* Found = UGameplayStatics::GetActorOfClass(GetWorld(), GameCharacterBPClass);
		GameCharacter = Cast<AGameCharacter>(Found);
	}

	// Ensure we have a valid enemy class to search for. If none has been set, default to the concrete actor class.
	if (EnemyClass == nullptr)
	{
		EnemyClass = ATestEnemyActor::StaticClass();
	}
	else if (EnemyClass != nullptr)
	{
		// Find an existing actor of the specified enemy class in the world and cache it.
		AActor* FoundEnemy = UGameplayStatics::GetActorOfClass(GetWorld(), EnemyClass);
		Enemy = Cast<ATestEnemyActor>(FoundEnemy);
	}

	// Ensure Spline is set at runtime (find the first ASplineActor in the world if not already assigned).
	if (SplineClass == nullptr)
	{
		SplineClass = ASplineActor::StaticClass();
	}
	else if (SplineClass != nullptr)
	{
		AActor* FoundSpline = UGameplayStatics::GetActorOfClass(GetWorld(), SplineClass);
		Spline = Cast<ASplineActor>(FoundSpline);
	}

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}
}

void AGameController::SetupInputComponent()
{
	Super::SetupInputComponent();

	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent))
	{
		EnhancedInputComponent->BindAction(C1Action,      ETriggerEvent::Started, this, &AGameController::HandleC1Attack);
		EnhancedInputComponent->BindAction(CSharp1Action, ETriggerEvent::Started, this, &AGameController::HandleCSharp1Attack);
		EnhancedInputComponent->BindAction(D1Action,      ETriggerEvent::Started, this, &AGameController::HandleD1Attack);
		EnhancedInputComponent->BindAction(DSharp1Action, ETriggerEvent::Started, this, &AGameController::HandleDSharp1Attack);
		EnhancedInputComponent->BindAction(E1Action,      ETriggerEvent::Started, this, &AGameController::HandleE1Attack);
		EnhancedInputComponent->BindAction(F1Action,      ETriggerEvent::Started, this, &AGameController::HandleF1Attack);
		EnhancedInputComponent->BindAction(FSharp1Action, ETriggerEvent::Started, this, &AGameController::HandleFSharp1Attack);
		EnhancedInputComponent->BindAction(G1Action,      ETriggerEvent::Started, this, &AGameController::HandleG1Attack);
		EnhancedInputComponent->BindAction(GSharp1Action, ETriggerEvent::Started, this, &AGameController::HandleGSharp1Attack);
		EnhancedInputComponent->BindAction(A1Action,      ETriggerEvent::Started, this, &AGameController::HandleA1Attack);
		EnhancedInputComponent->BindAction(ASharp1Action, ETriggerEvent::Started, this, &AGameController::HandleASharp1Attack);
		EnhancedInputComponent->BindAction(B1Action,      ETriggerEvent::Started, this, &AGameController::HandleB1Attack);
		EnhancedInputComponent->BindAction(C2Action,      ETriggerEvent::Started, this, &AGameController::HandleC2Attack);
		EnhancedInputComponent->BindAction(CSharp2Action, ETriggerEvent::Started, this, &AGameController::HandleCSharp2Attack);
		EnhancedInputComponent->BindAction(D2Action,      ETriggerEvent::Started, this, &AGameController::HandleD2Attack);
		EnhancedInputComponent->BindAction(DSharp2Action, ETriggerEvent::Started, this, &AGameController::HandleDSharp2Attack);
		EnhancedInputComponent->BindAction(E2Action,      ETriggerEvent::Started, this, &AGameController::HandleE2Attack);
		EnhancedInputComponent->BindAction(F2Action,      ETriggerEvent::Started, this, &AGameController::HandleF2Attack);
		EnhancedInputComponent->BindAction(FSharp2Action, ETriggerEvent::Started, this, &AGameController::HandleFSharp2Attack);
		EnhancedInputComponent->BindAction(G2Action,      ETriggerEvent::Started, this, &AGameController::HandleG2Attack);
		
		//Trigger Release of note when releasing key
		EnhancedInputComponent->BindAction(C1Action,      ETriggerEvent::Started, this, &AGameController::HandleNoteRelease);
		EnhancedInputComponent->BindAction(CSharp1Action, ETriggerEvent::Started, this, &AGameController::HandleNoteRelease);
		EnhancedInputComponent->BindAction(D1Action,      ETriggerEvent::Started, this, &AGameController::HandleNoteRelease);
		EnhancedInputComponent->BindAction(DSharp1Action, ETriggerEvent::Started, this, &AGameController::HandleNoteRelease);
		EnhancedInputComponent->BindAction(E1Action,      ETriggerEvent::Started, this, &AGameController::HandleNoteRelease);
		EnhancedInputComponent->BindAction(F1Action,      ETriggerEvent::Started, this, &AGameController::HandleNoteRelease);
		EnhancedInputComponent->BindAction(FSharp1Action, ETriggerEvent::Started, this, &AGameController::HandleNoteRelease);
		EnhancedInputComponent->BindAction(G1Action,      ETriggerEvent::Started, this, &AGameController::HandleNoteRelease);
		EnhancedInputComponent->BindAction(GSharp1Action, ETriggerEvent::Started, this, &AGameController::HandleNoteRelease);
		EnhancedInputComponent->BindAction(A1Action,      ETriggerEvent::Started, this, &AGameController::HandleNoteRelease);
		EnhancedInputComponent->BindAction(ASharp1Action, ETriggerEvent::Started, this, &AGameController::HandleNoteRelease);
		EnhancedInputComponent->BindAction(B1Action,      ETriggerEvent::Started, this, &AGameController::HandleNoteRelease);
		EnhancedInputComponent->BindAction(C2Action,      ETriggerEvent::Started, this, &AGameController::HandleNoteRelease);
		EnhancedInputComponent->BindAction(CSharp2Action, ETriggerEvent::Started, this, &AGameController::HandleNoteRelease);
		EnhancedInputComponent->BindAction(D2Action,      ETriggerEvent::Started, this, &AGameController::HandleNoteRelease);
		EnhancedInputComponent->BindAction(DSharp2Action, ETriggerEvent::Started, this, &AGameController::HandleNoteRelease);
		EnhancedInputComponent->BindAction(E2Action,      ETriggerEvent::Started, this, &AGameController::HandleNoteRelease);
		EnhancedInputComponent->BindAction(F2Action,      ETriggerEvent::Started, this, &AGameController::HandleNoteRelease);
		EnhancedInputComponent->BindAction(FSharp2Action, ETriggerEvent::Started, this, &AGameController::HandleNoteRelease);
		EnhancedInputComponent->BindAction(G2Action,      ETriggerEvent::Started, this, &AGameController::HandleNoteRelease);

		EnhancedInputComponent->BindAction(PauseAction, ETriggerEvent::Started, this, &AGameController::HandlePause);
	}
}

void AGameController::InitMidi()
{
	DisplayMidiDevicesIDs();

	CreateMidiController();
}

void AGameController::DisplayMidiDevicesIDs()
{
	TArray<FFoundMIDIDevice> InputDevices;

	UMIDIDeviceManager::FindMIDIDevices(InputDevices);

	for (auto InputDevice : InputDevices)
	{
		UE_LOG(LogTemp, Log, TEXT("ID: %d"), InputDevice.DeviceID);
		UE_LOG(LogTemp, Log, TEXT("ID: %s"), *InputDevice.DeviceName);
	}
}

void AGameController::CreateMidiController()
{
	MidiController = UMIDIDeviceManager::CreateMIDIDeviceInputController(1, 1024);

	if (MidiController != nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("Successfully connected with MIDI Controller."));
		MidiController->OnMIDINoteOn.AddDynamic(this, &AGameController::HandleNoteOn);
		MidiController->OnMIDINoteOff.AddDynamic(this, &AGameController::HandleNoteOff);
		MidiController->OnMIDIPitchBend.AddDynamic(this, &AGameController::HandlePitchBend);
		MidiController->OnMIDIAftertouch.AddDynamic(this, &AGameController::HandleAftertouch);
		MidiController->OnMIDIControlChange.AddDynamic(this, &AGameController::HandleControlChange);
	}
	else
	{
		// Should not happen
		UE_LOG(LogTemp, Log, TEXT("Failed to connect with MIDI Controller."));
	}
}

void AGameController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

void AGameController::HandleNoteOn(UMIDIDeviceInputController* MIDIDeviceController, int32 Timestamp, int32 Channel,
	int32 Note, int32 Velocity)
{
	//Test for Input Latency
	UAudioManagerSubsystem* AudioManager = GetWorld()->GetSubsystem<UAudioManagerSubsystem>();
	if (AudioManager && AudioManager->GetQuartzSubsystem())
	{
		double LatencyBetweenThreads = AudioManager->GetLatencyBetweenThreads();

		// double Latency = (FPlatformTime::Seconds() - AudioManager->GetBeatSeconds()) * 1000.0;
		double Latency = (FPlatformTime::Seconds() - AudioManager->GetMetaSoundOutputTimeSeconds()) * 1000.0;

		UE_LOG(LogTemp, Warning, TEXT("Audio Thread to Game Thread Latency: %f ms"), LatencyBetweenThreads);
		UE_LOG(LogTemp, Warning, TEXT("Input Latency: %f ms"), Latency);
		UE_LOG(LogTemp, Warning, TEXT("Full Latency: %f ms"), Latency + LatencyBetweenThreads);
	}


	NotePlayed = Note;

	// Convert incoming MIDI note number to semitone class (0-11) and map to ENote.
	const int32 Semitone = Note % 12;

	ENote NoteEnum = static_cast<ENote>(Semitone);

	switch (GetControllerState())
	{
		case EControllerStateGame::Game:
		{
			//Remap incoming MIDI Note to allow different musical scales
			//Maps the incoming scale onto the enharmonic scale with C as root note
			//Meaning: If the scale is A minor and the player presses A, it comes out as C etc.
			const int32 RemappedSemitone = (Note - static_cast<int32>(RootNote)) % 12;
			NoteEnum = static_cast<ENote>(RemappedSemitone);
			GameControl(NoteEnum);
			break;
		}
		case EControllerStateGame::PauseMenu:
		{
			PauseMenuControl(NoteEnum);
			break;
		}
		case EControllerStateGame::AudioMenu:
		{
			AudioMenuControl(NoteEnum);
			break;
		}
		case EControllerStateGame::GraphicsMenu:
		{
			GraphicMenuControl(NoteEnum);
			break;
		}
		case EControllerStateGame::SettingMenu:
		{
			SettingMenuControl(NoteEnum);
			break;
		}
		case EControllerStateGame::DeathMenu:
		{
			DeathMenuControl(NoteEnum);
			break;
		}
		default:
		{
			// Should not land here
			break;
		}
	}
}

void AGameController::HandleNoteOff(UMIDIDeviceInputController* MIDIDeviceController, int32 Timestamp, int32 Channel, int32 Note, int32 Velocity)
{
	if (GameCharacter)
	{
		GameCharacter->StopNote();
	}
	
}

void AGameController::HandlePitchBend(UMIDIDeviceInputController* MIDIDeviceController, int32 Timestamp, int32 Channel, int32 Pitch)
{
	//UE_LOG(LogTemp, Log, TEXT("Channel: %d"),  Channel);
	//UE_LOG(LogTemp, Log, TEXT("Pitch: %d"),    Pitch);
}

void AGameController::HandleAftertouch(UMIDIDeviceInputController* MIDIDeviceController, int32 Timestamp, int32 Channel, int32 Note, int32 Amount)
{
	//UE_LOG(LogTemp, Log, TEXT("Channel: %d"),  Channel);
	//UE_LOG(LogTemp, Log, TEXT("Note: %d"),     Note);
	//UE_LOG(LogTemp, Log, TEXT("Amount: %d"),   Amount);
}

void AGameController::HandleControlChange(UMIDIDeviceInputController* MIDIDeviceController, int32 Timestamp, int32 Channel, int32 Type, int32 Value)
{
	float NormalizedValue = static_cast<float>(Value) / 127.0f;
	NormalizedValue = FMath::Clamp(NormalizedValue, 0.0f, 1.0f);

	AudioSoundControl(Type, NormalizedValue);
}

void AGameController::HandleC1Attack()
{
	NotePlayed = static_cast<int32>(CNoteOctave);
	HandleKeyboardAttack(NotePlayed);
}

void AGameController::HandleCSharp1Attack()
{
	NotePlayed = static_cast<int32>(CNoteOctave) + 1;
	HandleKeyboardAttack(NotePlayed);
}

void AGameController::HandleD1Attack()
{
	NotePlayed = static_cast<int32>(CNoteOctave) + 2;
	HandleKeyboardAttack(NotePlayed);
}

void AGameController::HandleDSharp1Attack()
{
	NotePlayed = static_cast<int32>(CNoteOctave) + 3;
	HandleKeyboardAttack(NotePlayed);
}

void AGameController::HandleE1Attack()
{
	NotePlayed = static_cast<int32>(CNoteOctave) + 4;
	HandleKeyboardAttack(NotePlayed);
}

void AGameController::HandleF1Attack()
{
	NotePlayed = static_cast<int32>(CNoteOctave) + 5;
	HandleKeyboardAttack(NotePlayed);
}

void AGameController::HandleFSharp1Attack()
{
	NotePlayed = static_cast<int32>(CNoteOctave) + 6;
	HandleKeyboardAttack(NotePlayed);
}

void AGameController::HandleG1Attack()
{
	NotePlayed = static_cast<int32>(CNoteOctave) + 7;
	HandleKeyboardAttack(NotePlayed);
}

void AGameController::HandleGSharp1Attack()
{
	NotePlayed = static_cast<int32>(CNoteOctave) + 8;
	HandleKeyboardAttack(NotePlayed);
}

void AGameController::HandleA1Attack()
{
	NotePlayed = static_cast<int32>(CNoteOctave) + 9;
	HandleKeyboardAttack(NotePlayed);
}

void AGameController::HandleASharp1Attack()
{
	NotePlayed = static_cast<int32>(CNoteOctave) + 10;
	HandleKeyboardAttack(NotePlayed);
}

void AGameController::HandleB1Attack()
{
	NotePlayed = static_cast<int32>(CNoteOctave) + 11;
	HandleKeyboardAttack(NotePlayed);
}

void AGameController::HandleC2Attack()
{
	NotePlayed = static_cast<int32>(CNoteOctave) + 12;
	HandleKeyboardAttack(NotePlayed);
}

void AGameController::HandleCSharp2Attack()
{
	NotePlayed = static_cast<int32>(CNoteOctave) + 13;
	HandleKeyboardAttack(NotePlayed);
}

void AGameController::HandleD2Attack()
{
	NotePlayed = static_cast<int32>(CNoteOctave) + 14;
	HandleKeyboardAttack(NotePlayed);
}

void AGameController::HandleDSharp2Attack()
{
	NotePlayed = static_cast<int32>(CNoteOctave) + 15;
	HandleKeyboardAttack(NotePlayed);
}

void AGameController::HandleE2Attack()
{
	NotePlayed = static_cast<int32>(CNoteOctave) + 16;
	HandleKeyboardAttack(NotePlayed);
}

void AGameController::HandleF2Attack()
{
	NotePlayed = static_cast<int32>(CNoteOctave) + 17;
	HandleKeyboardAttack(NotePlayed);
}

void AGameController::HandleFSharp2Attack()
{
	NotePlayed = static_cast<int32>(CNoteOctave) + 18;
	HandleKeyboardAttack(NotePlayed);
}

void AGameController::HandleG2Attack()
{
	NotePlayed = static_cast<int32>(CNoteOctave) + 19;
	HandleKeyboardAttack(NotePlayed);
}

void AGameController::HandleKeyboardAttack(int32 MidiNote)
{
	//Remap Midi Note (See HandleMidiNoteOn)
	const int32 RemappedSemitone = (MidiNote - static_cast<int32>(RootNote)) % 12;
	ENote NoteEnum = static_cast<ENote>(RemappedSemitone);

	//Handle Movement and Attacks
	MovePlayer(NoteEnum);
	HandleAttack(NoteEnum);
}

void AGameController::HandleAttack(ENote Note)
{
	UAudioManagerSubsystem* AudioManager = GetWorld()->GetSubsystem<UAudioManagerSubsystem>();
	if (AudioManager != nullptr)
	{
		if (AudioManager->GetPlayerCanAttack() && GameCharacter->GetDefended() > 0)
		{
			auto Defended = GameCharacter->GetDefended() - 1;
			UE_LOG(LogTemp, Warning, TEXT("Defended Left: %d"), Defended);
			GameCharacter->SetDefended(Defended);

			//Play Player Attack Anim
			GameCharacter->PlayAttackAnimation();
			
			if (GameCharacter->GetSplineRef()->IsVisible())
			{
				auto ActorLocation = GameCharacter->GetActorLocation();
				auto SceneLocation = GameCharacter->GetSceneComponent()->GetRelativeLocation();
				auto SpawnLocationPlayer = ActorLocation + SceneLocation;
				auto NoteActor = GetWorld()->SpawnActor<ANodeActor>(NodeActor, SpawnLocationPlayer, GameCharacter->GetActorRotation());
				//Play some Feedback Sound
				GameCharacter->PlayNote(NotePlayed);

				if (NoteActor != nullptr && Enemy != nullptr)
				{
					NoteActor->GetTimeline()->SetPlayRate(.25f);
					NoteActor->SetSplineRef(GameCharacter->GetSplineRef());
					NoteActor->MoveRight();
				}
			}
			else
			{
				//TODO: Add some feedback when the Player plays a note that is off key
			}
		}
	}
}

void AGameController::HandleNoteRelease()
{
	if (GameCharacter)
	{
		GameCharacter->StopNote();
	}
	
}

void AGameController::HandlePause()
{
	SetControllerState(EControllerStateGame::PauseMenu);

	// Toggle pause state
	bool bCurrentlyPaused = UGameplayStatics::IsGamePaused(GetWorld());
	UGameplayStatics::SetGamePaused(GetWorld(), !bCurrentlyPaused);

	if (bCurrentlyPaused == false)
	{
		if (GameHud != nullptr)
		{
			GameHud->GetPauseMenuInstance()->SetVisibility(ESlateVisibility::Visible);
		}

		// Show mouse cursor and switch to GameAndUI input so widgets receive focus
		bShowMouseCursor = true;
		FInputModeGameAndUI InputMode;
		InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		SetInputMode(InputMode);
	}
}

void AGameController::MovePlayer(ENote Note)
{
	if (GameCharacter && GameCharacter->GetCapsuleComponent() && Spline)
	{
		//Sets the Characters Spline Ref based on the Midi Note played
		USplineComponent* SplineRef = Spline->GetSplines(static_cast<int32>(Note));

		GameCharacter->SetSplineRef(SplineRef);

		FVector SplineWorldLocation = SplineRef->GetLocationAtSplinePoint(0, ESplineCoordinateSpace::World);

		GameCharacter->GetCapsuleComponent()->SetWorldLocation(SplineWorldLocation);
		
		//Collision stuff
		GameCharacter->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		
		GetWorld()->GetTimerManager().ClearTimer(CollisionTimerHandle);

		float TimerDelay = FrameToSeconds();
		GetWorld()->GetTimerManager().SetTimer(CollisionTimerHandle, this, &AGameController::DisablePlayerCollision, TimerDelay, false);
	}
}

void AGameController::GameControl(ENote Note)
{
	if (GameHud != nullptr)
	{
		MovePlayer(Note);
		HandleAttack(Note);
	}
}

void AGameController::PauseMenuControl(ENote Note)
{
	if (GameHud != nullptr)
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
			if (GameHud->GetPauseMenuInstance()->GetResumeButton()->HasKeyboardFocus())
			{
				PauseMenuIndex = 0;
				GameHud->GetPauseMenuInstance()->ResumeButtonClicked();
				break;
			}
			if (GameHud->GetPauseMenuInstance()->GetSaveGameButton()->HasKeyboardFocus())
			{
				PauseMenuIndex = 0;
				GameHud->GetPauseMenuInstance()->SaveGameButtonClicked();
				break;
			}
			if (GameHud->GetPauseMenuInstance()->GetLoadGameButton()->HasKeyboardFocus())
			{
				PauseMenuIndex = 0;
				GameHud->GetPauseMenuInstance()->LoadGameButtonClicked();
				break;
			}
			if (GameHud->GetPauseMenuInstance()->GetSettingsButton()->HasKeyboardFocus())
			{
				PauseMenuIndex = 0;
				GameHud->GetPauseMenuInstance()->SettingsButtonClicked();
				break;
			}
			if (GameHud->GetPauseMenuInstance()->GetMainMenuButton()->HasKeyboardFocus())
			{
				PauseMenuIndex = 0;
				GameHud->GetPauseMenuInstance()->MainMenuButtonClicked();
				break;
			}
			if (GameHud->GetPauseMenuInstance()->GetQuitButton()->HasKeyboardFocus())
			{
				PauseMenuIndex = 0;
				GameHud->GetPauseMenuInstance()->QuitButtonClicked();
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

void AGameController::SettingMenuControl(ENote Note)
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
		if (GameHud != nullptr)
		{
			if (GameHud->GetMainMenuSettingInstance()->GetGraphicButton()->HasKeyboardFocus())
			{
				SetControllerState(EControllerStateGame::GraphicsMenu);
				SettingMenuIndex = 0;
				GraphicMenuIndex = 0;
				GameHud->GetMainMenuSettingInstance()->GraphicSettingClicked();
				break;
			}
			if (GameHud->GetMainMenuSettingInstance()->GetAudioButton()->HasKeyboardFocus())
			{
				SetControllerState(EControllerStateGame::AudioMenu);
				SettingMenuIndex = 0;
				AudioMenuIndex = 0;
				GameHud->GetMainMenuSettingInstance()->AudioSettingClicked();
				break;
			}
			if (GameHud->GetMainMenuSettingInstance()->GetMainMenuButton()->HasKeyboardFocus())
			{
				SetControllerState(EControllerStateGame::PauseMenu);
				SettingMenuIndex = 0;
				GameHud->GetMainMenuSettingInstance()->ReturnMainMenuClicked();
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

void AGameController::GraphicMenuControl(ENote Note)
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
		if (GameHud != nullptr)
		{
			if (GameHud->GetMainMenuSettingInstance()->GetGraphicSettingWidget()->GetWindowModeDownButton()->HasKeyboardFocus())
			{
				GameHud->GetMainMenuSettingInstance()->GetGraphicSettingWidget()->WindowModeDownClicked();
				break;
			}
			if (GameHud->GetMainMenuSettingInstance()->GetGraphicSettingWidget()->GetWindowModeUpButton()->HasKeyboardFocus())
			{
				GameHud->GetMainMenuSettingInstance()->GetGraphicSettingWidget()->WindowModeUpClicked();
				break;
			}
			if (GameHud->GetMainMenuSettingInstance()->GetGraphicSettingWidget()->GetResolutionDownButton()->HasKeyboardFocus())
			{
				GameHud->GetMainMenuSettingInstance()->GetGraphicSettingWidget()->ResolutionDownClicked();
				break;
			}
			if (GameHud->GetMainMenuSettingInstance()->GetGraphicSettingWidget()->GetResolutionUpButton()->HasKeyboardFocus())
			{
				GameHud->GetMainMenuSettingInstance()->GetGraphicSettingWidget()->ResolutionUpClicked();
				break;
			}
			if (GameHud->GetMainMenuSettingInstance()->GetGraphicSettingWidget()->GetGraphicDownButton()->HasKeyboardFocus())
			{
				GameHud->GetMainMenuSettingInstance()->GetGraphicSettingWidget()->GraphicDownClicked();
				break;
			}
			if (GameHud->GetMainMenuSettingInstance()->GetGraphicSettingWidget()->GetGraphicUpButton()->HasKeyboardFocus())
			{
				GameHud->GetMainMenuSettingInstance()->GetGraphicSettingWidget()->GraphicUpClicked();
				break;
			}
			if (GameHud->GetMainMenuSettingInstance()->GetGraphicSettingWidget()->GetVSyncDownButton()->HasKeyboardFocus())
			{
				GameHud->GetMainMenuSettingInstance()->GetGraphicSettingWidget()->VSyncDownClicked();
				break;
			}
			if (GameHud->GetMainMenuSettingInstance()->GetGraphicSettingWidget()->GetVSyncUpButton()->HasKeyboardFocus())
			{
				GameHud->GetMainMenuSettingInstance()->GetGraphicSettingWidget()->VSyncUpClicked();
				break;
			}
			if (GameHud->GetMainMenuSettingInstance()->GetGraphicSettingWidget()->GetApplyButton()->HasKeyboardFocus())
			{
				GameHud->GetMainMenuSettingInstance()->GetGraphicSettingWidget()->ApplyClicked();
				break;
			}
			if (GameHud->GetMainMenuSettingInstance()->GetGraphicSettingWidget()->GetMainMenuButton()->HasKeyboardFocus())
			{
				GraphicMenuIndex = 0;
				GameHud->GetMainMenuSettingInstance()->GetGraphicSettingWidget()->ReturnMainMenuUnhovered();
				GameHud->GetMainMenuSettingInstance()->GetGraphicSettingWidget()->ReturnMainMenuClicked();
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

void AGameController::AudioMenuControl(ENote Note)
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
			if (GameHud != nullptr)
			{
				if (GameHud->GetMainMenuSettingInstance()->GetAudioSettingWidget()->GetMasterVolumeSlider()->HasKeyboardFocus())
				{
					AudioMenuIndex = 0;
					break;
				}
				if (GameHud->GetMainMenuSettingInstance()->GetAudioSettingWidget()->GetMusicVolumeSlider()->HasKeyboardFocus())
				{
					AudioMenuIndex = 0;
					break;
				}
				if (GameHud->GetMainMenuSettingInstance()->GetAudioSettingWidget()->GetSfxVolumeSlider()->HasKeyboardFocus())
				{
					AudioMenuIndex = 0;
					break;
				}
				if (GameHud->GetMainMenuSettingInstance()->GetAudioSettingWidget()->GetMainMenuButton()->HasKeyboardFocus())
				{
					AudioMenuIndex = 0;
					GameHud->GetMainMenuSettingInstance()->GetAudioSettingWidget()->ReturnMenu();
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

void AGameController::GraphicMenuSwitchButton(EGraphicSettingItem InGraphicSettingItem)
{
	if (GameHud != nullptr)
	{
		switch (InGraphicSettingItem)
		{
			case EGraphicSettingItem::WindowModeDown:
			{
				GameHud->GetMainMenuSettingInstance()->GetGraphicSettingWidget()->GetWindowModeDownButton()->SetKeyboardFocus();
				GameHud->GetMainMenuSettingInstance()->GetGraphicSettingWidget()->WindowModeDownHovered();
				break;
			}
			case EGraphicSettingItem::WindowModeUp:
			{
				GameHud->GetMainMenuSettingInstance()->GetGraphicSettingWidget()->GetWindowModeUpButton()->SetKeyboardFocus();
				GameHud->GetMainMenuSettingInstance()->GetGraphicSettingWidget()->WindowModeUpHovered();
				break;
			}
			case EGraphicSettingItem::ResolutionDown:
			{
				GameHud->GetMainMenuSettingInstance()->GetGraphicSettingWidget()->GetResolutionDownButton()->SetKeyboardFocus();
				GameHud->GetMainMenuSettingInstance()->GetGraphicSettingWidget()->ResolutionDownHovered();
				break;
			}
			case EGraphicSettingItem::ResolutionUp:
			{
				GameHud->GetMainMenuSettingInstance()->GetGraphicSettingWidget()->GetResolutionUpButton()->SetKeyboardFocus();
				GameHud->GetMainMenuSettingInstance()->GetGraphicSettingWidget()->ResolutionUpHovered();
				break;
			}
			case EGraphicSettingItem::GraphicDown:
			{
				GameHud->GetMainMenuSettingInstance()->GetGraphicSettingWidget()->GetGraphicDownButton()->SetKeyboardFocus();
				GameHud->GetMainMenuSettingInstance()->GetGraphicSettingWidget()->GraphicDownHovered();
				break;
			}
			case EGraphicSettingItem::GraphicUp:
			{
				GameHud->GetMainMenuSettingInstance()->GetGraphicSettingWidget()->GetGraphicUpButton()->SetKeyboardFocus();
				GameHud->GetMainMenuSettingInstance()->GetGraphicSettingWidget()->GraphicUpHovered();
				break;
			}
			case EGraphicSettingItem::VSyncDown:
			{
				GameHud->GetMainMenuSettingInstance()->GetGraphicSettingWidget()->GetVSyncDownButton()->SetKeyboardFocus();
				GameHud->GetMainMenuSettingInstance()->GetGraphicSettingWidget()->VSyncDownHovered();
				break;
			}
			case EGraphicSettingItem::VSyncUp:
			{
				GameHud->GetMainMenuSettingInstance()->GetGraphicSettingWidget()->GetVSyncUpButton()->SetKeyboardFocus();
				GameHud->GetMainMenuSettingInstance()->GetGraphicSettingWidget()->VSyncUpHovered();
				break;
			}
			case EGraphicSettingItem::Apply:
			{
				GameHud->GetMainMenuSettingInstance()->GetGraphicSettingWidget()->GetApplyButton()->SetKeyboardFocus();
				GameHud->GetMainMenuSettingInstance()->GetGraphicSettingWidget()->ApplyHovered();
				break;
			}
			case EGraphicSettingItem::Return:
			{
				GameHud->GetMainMenuSettingInstance()->GetGraphicSettingWidget()->GetMainMenuButton()->SetKeyboardFocus();
				GameHud->GetMainMenuSettingInstance()->GetGraphicSettingWidget()->ReturnSettingMenuHovered();
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

void AGameController::SwitchAudioMenuButton(EAudioSettingItem InAudioSettingItem)
{
	if (GameHud != nullptr)
	{
		switch (InAudioSettingItem)
		{
			case EAudioSettingItem::MasterVolumeSlider:
			{
				GameHud->GetMainMenuSettingInstance()->GetAudioSettingWidget()->GetMasterVolumeSlider()->SetKeyboardFocus();
				GameHud->GetMainMenuSettingInstance()->GetAudioSettingWidget()->MasterVolumeSliderHovered();

				break;
			}
			case EAudioSettingItem::MusicVolumeSlider:
			{
				GameHud->GetMainMenuSettingInstance()->GetAudioSettingWidget()->GetMusicVolumeSlider()->SetKeyboardFocus();
				GameHud->GetMainMenuSettingInstance()->GetAudioSettingWidget()->MusicVolumeSliderHovered();
				break;
			}
			case EAudioSettingItem::SfxVolumeSlider:
			{
				GameHud->GetMainMenuSettingInstance()->GetAudioSettingWidget()->GetSfxVolumeSlider()->SetKeyboardFocus();
				GameHud->GetMainMenuSettingInstance()->GetAudioSettingWidget()->SfxVolumeSliderHovered();

				break;
			}
			case EAudioSettingItem::Return:
			{
				GameHud->GetMainMenuSettingInstance()->GetAudioSettingWidget()->GetMainMenuButton()->SetKeyboardFocus();
				GameHud->GetMainMenuSettingInstance()->GetAudioSettingWidget()->ReturnSettingMenuHovered();
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

void AGameController::AudioSoundControl(int32 Type, float SoundValue)
{
	// Modwheel
	if (Type == 1)
	{
		if (GameHud != nullptr)
		{
			// TODO: Safe the Value in the GameInstance later!
			if (GameHud->GetMainMenuSettingInstance()->GetAudioSettingWidget()->GetMasterVolumeSlider()->HasKeyboardFocus())
			{
				GameHud->GetMainMenuSettingInstance()->GetAudioSettingWidget()->GetMasterVolumeSlider()->SetValue(SoundValue);
			}
			if (GameHud->GetMainMenuSettingInstance()->GetAudioSettingWidget()->GetMusicVolumeSlider()->HasKeyboardFocus())
			{
				GameHud->GetMainMenuSettingInstance()->GetAudioSettingWidget()->GetMusicVolumeSlider()->SetValue(SoundValue);
			}
			if (GameHud->GetMainMenuSettingInstance()->GetAudioSettingWidget()->GetSfxVolumeSlider()->HasKeyboardFocus())
			{
				GameHud->GetMainMenuSettingInstance()->GetAudioSettingWidget()->GetSfxVolumeSlider()->SetValue(SoundValue);
			}
		}
	}
}

void AGameController::PauseMenuSwitchButton(EPauseMenuItem InPauseMenuItem)
{
	if (GameHud != nullptr)
	{
		switch (InPauseMenuItem)
		{
			case EPauseMenuItem::Resume:
			{
				GameHud->GetPauseMenuInstance()->GetResumeButton()->SetKeyboardFocus();
				GameHud->GetPauseMenuInstance()->ResumeButtonHovered();
				break;
			}
			case EPauseMenuItem::Save:
			{
				GameHud->GetPauseMenuInstance()->GetSaveGameButton()->SetKeyboardFocus();
				GameHud->GetPauseMenuInstance()->SaveGameButtonHovered();
				break;
			}
			case EPauseMenuItem::Load:
			{
				GameHud->GetPauseMenuInstance()->GetLoadGameButton()->SetKeyboardFocus();
				GameHud->GetPauseMenuInstance()->LoadGameButtonHovered();
				break;
			}
			case EPauseMenuItem::Settings:
			{
				GameHud->GetPauseMenuInstance()->GetSettingsButton()->SetKeyboardFocus();
				GameHud->GetPauseMenuInstance()->SettingsButtonHovered();
				break;
			}
			case EPauseMenuItem::MainMenu:
			{
				GameHud->GetPauseMenuInstance()->GetMainMenuButton()->SetKeyboardFocus();
				GameHud->GetPauseMenuInstance()->MainMenuButtonHovered();
				break;
			}
			case EPauseMenuItem::Quit:
			{
				GameHud->GetPauseMenuInstance()->GetQuitButton()->SetKeyboardFocus();
				GameHud->GetPauseMenuInstance()->QuitButtonHovered();
				break;
			}
		}
	}
}

void AGameController::MainSettingSwitchButton(EMainSettingItem InMainSettingItem)
{
	if (GameHud != nullptr)
	{
		switch (InMainSettingItem)
		{
			case EMainSettingItem::Graphic:
			{
				GameHud->GetMainMenuSettingInstance()->GetGraphicButton()->SetKeyboardFocus();
				GameHud->GetMainMenuSettingInstance()->GraphicButtonHovered();
				break;
			}
			case EMainSettingItem::Audio:
			{
				GameHud->GetMainMenuSettingInstance()->GetAudioButton()->SetKeyboardFocus();
				GameHud->GetMainMenuSettingInstance()->AudioButtonHovered();
				break;
			}
			case EMainSettingItem::MainMenu:
			{
				GameHud->GetMainMenuSettingInstance()->GetMainMenuButton()->SetKeyboardFocus();
				GameHud->GetMainMenuSettingInstance()->ReturnMainMenuButtonHovered();
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

void AGameController::DeathMenuControl(ENote Note)
{
	switch (Note)
	{
		case ENote::C:
		{
			DeathMenuIndex++;
			DeathMenuIndex = FMath::Clamp(DeathMenuIndex, 0, 2);
			UE_LOG(LogTemp, Log, TEXT("DeathMenuIndex: %d"), DeathMenuIndex);
			break;
		}
		case ENote::D:
		{
			DeathMenuIndex--;
			DeathMenuIndex = FMath::Clamp(DeathMenuIndex, 0, 2);
			UE_LOG(LogTemp, Log, TEXT("DeathMenuIndex: %d"), DeathMenuIndex);
			break;
		}
		case ENote::B:
		{
			if (GameHud != nullptr)
			{
				if (GameHud->GetDeathWidgetInstance()->GetRespawnButton()->HasKeyboardFocus())
				{
					DeathMenuIndex = 0;
					GameHud->GetDeathWidgetInstance()->RespawnButtonClicked();
					break;
				}
				if (GameHud->GetDeathWidgetInstance()->GetMainMenuButton()->HasKeyboardFocus())
				{
					DeathMenuIndex = 0;
					GameHud->GetDeathWidgetInstance()->MainMenuButtonClicked();
					break;
				}
				if (GameHud->GetDeathWidgetInstance()->GetWorldMapButton()->HasKeyboardFocus())
				{
					DeathMenuIndex = 0;
					GameHud->GetDeathWidgetInstance()->WorldMapButtonClicked();
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
	DeathMenuSwitchButton(static_cast<EDeathState>(DeathMenuIndex));
}

void AGameController::DeathMenuSwitchButton(EDeathState InDeathState)
{
	if (GameHud != nullptr)
	{
		switch (InDeathState)
		{
			case EDeathState::Respawn:
			{
				GameHud->GetDeathWidgetInstance()->GetRespawnButton()->SetKeyboardFocus();
				GameHud->GetDeathWidgetInstance()->RespawnButtonHovered();
				break;
			}
			case EDeathState::MainMenu:
			{
				GameHud->GetDeathWidgetInstance()->GetMainMenuButton()->SetKeyboardFocus();
				GameHud->GetDeathWidgetInstance()->MainMenuButtonHovered();
				break;
			}
			case EDeathState::WorldMap:
			{
				GameHud->GetDeathWidgetInstance()->GetWorldMapButton()->SetKeyboardFocus();
				GameHud->GetDeathWidgetInstance()->WorldMapButtonHovered();
			}
			default:
			{
				// Should not land here
				break;
			}
		}
	}
}

float AGameController::FrameToSeconds()
{
	float Seconds = 1.f;
	if (auto* World = GetWorld())
	{
		float FrameTime = World->GetDeltaSeconds();
		Seconds = FrameTime * FramesToEnableCollision;
	}
	return Seconds;
}

void AGameController::DisablePlayerCollision()
{
	if (GameCharacter && GameCharacter->GetCapsuleComponent())
	{
		GameCharacter->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	
}
	
