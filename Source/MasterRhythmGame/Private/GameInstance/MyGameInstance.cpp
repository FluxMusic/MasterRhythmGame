// Fill out your copyright notice in the Description page of Project Settings.


#include "GameInstance/MyGameInstance.h"
#include "Kismet/GameplayStatics.h"

bool UMyGameInstance::SaveGameToSlot(const FString& SlotName)
{
	URhythmSaveGame* SaveGameInstance = Cast<URhythmSaveGame>(UGameplayStatics::CreateSaveGameObject(URhythmSaveGame::StaticClass()));
	
	if (SaveGameInstance)
	{
		// Save audio settings
		SaveGameInstance->MasterVolume = MasterVolume;
		SaveGameInstance->MusicVolume = MusicVolume;
		SaveGameInstance->SfxVolume = SFXVolume;

		// Save level unlock states
		SaveGameInstance->bIsLevelOneUnlocked = bIsLevelOneUnlocked;
		SaveGameInstance->bIsLevelTwoUnlocked = bIsLevelTwoUnlocked;
		SaveGameInstance->bIsLevelThreeUnlocked = bIsLevelThreeUnlocked;
		SaveGameInstance->bIsLevelFourUnlocked = bIsLevelFourUnlocked;
		SaveGameInstance->bIsLevelFiveUnlocked = bIsLevelFiveUnlocked;
		SaveGameInstance->bIsLevelSixUnlocked = bIsLevelSixUnlocked;

		// Save level scores
		SaveGameInstance->LevelOneScore = LevelOneScore;
		SaveGameInstance->LevelTwoScore = LevelTwoScore;
		SaveGameInstance->LevelThreeScore = LevelThreeScore;
		SaveGameInstance->LevelFourScore = LevelFourScore;
		SaveGameInstance->LevelFiveScore = LevelFiveScore;
		SaveGameInstance->LevelSixScore = LevelSixScore;

		// Save unlocked instruments
		SaveGameInstance->UnlockedInstruments = UnlockedInstruments;

		// Save current level
		SaveGameInstance->CurrentLevel = CurrentLevel;

		// Save slot metadata
		SaveGameInstance->SaveSlotName = SlotName;
		SaveGameInstance->UserIndex = 0;

		// Perform the save
		if (UGameplayStatics::SaveGameToSlot(SaveGameInstance, SlotName, 0))
		{
			UE_LOG(LogTemp, Log, TEXT("Game saved successfully to slot: %s"), *SlotName);
			return true;
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to save game to slot: %s"), *SlotName);
			return false;
		}
	}

	UE_LOG(LogTemp, Error, TEXT("Failed to create save game object"));
	return false;
}

bool UMyGameInstance::LoadGameFromSlot(const FString& SlotName)
{
	if (UGameplayStatics::DoesSaveGameExist(SlotName, 0))
	{
		URhythmSaveGame* LoadedGame = Cast<URhythmSaveGame>(UGameplayStatics::LoadGameFromSlot(SlotName, 0));
		
		if (LoadedGame)
		{
			// Load audio settings
			MasterVolume = LoadedGame->MasterVolume;
			MusicVolume = LoadedGame->MusicVolume;
			SFXVolume = LoadedGame->SfxVolume;

			// Load level unlock states
			bIsLevelOneUnlocked = LoadedGame->bIsLevelOneUnlocked;
			bIsLevelTwoUnlocked = LoadedGame->bIsLevelTwoUnlocked;
			bIsLevelThreeUnlocked = LoadedGame->bIsLevelThreeUnlocked;
			bIsLevelFourUnlocked = LoadedGame->bIsLevelFourUnlocked;
			bIsLevelFiveUnlocked = LoadedGame->bIsLevelFiveUnlocked;
			bIsLevelSixUnlocked = LoadedGame->bIsLevelSixUnlocked;

			// Load level scores
			LevelOneScore = LoadedGame->LevelOneScore;
			LevelTwoScore = LoadedGame->LevelTwoScore;
			LevelThreeScore = LoadedGame->LevelThreeScore;
			LevelFourScore = LoadedGame->LevelFourScore;
			LevelFiveScore = LoadedGame->LevelFiveScore;
			LevelSixScore = LoadedGame->LevelSixScore;

			// Load unlocked instruments
			UnlockedInstruments = LoadedGame->UnlockedInstruments;

			// Load current level
			CurrentLevel = LoadedGame->CurrentLevel;

			// Cache the loaded save game
			RhythmSaveGame = LoadedGame;

			UE_LOG(LogTemp, Log, TEXT("Game loaded successfully from slot: %s"), *SlotName);
			OnLoadedGame();
			return true;
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to cast loaded game from slot: %s"), *SlotName);
			return false;
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Save game does not exist in slot: %s"), *SlotName);
		return false;
	}
}

bool UMyGameInstance::DoesSaveGameExist(const FString& SlotName) const
{
	return UGameplayStatics::DoesSaveGameExist(SlotName, 0);
}

int32 UMyGameInstance::GetLevelScore(ELevels Level) const
{
	switch (Level)
	{
		case ELevels::LevelOne:   return LevelOneScore;
		case ELevels::LevelTwo:   return LevelTwoScore;
		case ELevels::LevelThree: return LevelThreeScore;
		case ELevels::LevelFour:  return LevelFourScore;
		case ELevels::LevelFive:  return LevelFiveScore;
		case ELevels::LevelSix:   return LevelSixScore;
		default:
			UE_LOG(LogTemp, Warning, TEXT("Invalid level specified in GetLevelScore"));
			return 0;
	}
}

void UMyGameInstance::SetLevelScore(ELevels Level, int32 Score)
{
	switch (Level)
	{
		case ELevels::LevelOne:
			LevelOneScore = FMath::Max(LevelOneScore, Score);
			break;
		case ELevels::LevelTwo:
			LevelTwoScore = FMath::Max(LevelTwoScore, Score);
			break;
		case ELevels::LevelThree:
			LevelThreeScore = FMath::Max(LevelThreeScore, Score);
			break;
		case ELevels::LevelFour:
			LevelFourScore = FMath::Max(LevelFourScore, Score);
			break;
		case ELevels::LevelFive:
			LevelFiveScore = FMath::Max(LevelFiveScore, Score);
			break;
		case ELevels::LevelSix:
			LevelSixScore = FMath::Max(LevelSixScore, Score);
			break;
		default:
			UE_LOG(LogTemp, Warning, TEXT("Invalid level specified in SetLevelScore"));
			break;
	}
	
	UE_LOG(LogTemp, Log, TEXT("Level score updated: Level %d - Score: %d"), static_cast<int32>(Level), Score);
}

void UMyGameInstance::BeforeSaveGame()
{
}

void UMyGameInstance::OnLoadedGame()
{
    //TODO: Zeug setzen das geladen wurde @Alex mach mal
}

void UMyGameInstance::SetInstrumentUnlocked(const EInstrument& Instrument, bool bUnlocked)
{
	// We don't need to do anything if the Instrument is already added or not added
	// based on what we want to do here
	if (InstrumentFlags::HasFlag(UnlockedInstruments, Instrument) == bUnlocked)
	{
		return;
	}
	else
	{
		InstrumentFlags::SetFlag(UnlockedInstruments, Instrument);
	}
}

EInstrument UMyGameInstance::GetUnlockedInstruments()
{
	return UnlockedInstruments;
}