// Fill out your copyright notice in the Description page of Project Settings.


#include "Data/RhythmSaveGame.h"

URhythmSaveGame::URhythmSaveGame()
{
	SaveSlotName = TEXT("SaveSlot1");
	UserIndex = 0;

	// Default audio settings
	MasterVolume = 0.75f;
	MusicVolume = 0.55f;
	SfxVolume = 0.75f;

	// Default progress
	bIsLevelOneUnlocked = true;
	bIsLevelTwoUnlocked = false;
	bIsLevelThreeUnlocked = false;
	bIsLevelFourUnlocked = false;
	bIsLevelFiveUnlocked = false;
	bIsLevelSixUnlocked = false;

	// Default scores
	LevelOneScore = 0;
	LevelTwoScore = 0;
	LevelThreeScore = 0;
	LevelFourScore = 0;
	LevelFiveScore = 0;
	LevelSixScore = 0;

	// Default instruments
	UnlockedInstruments = EInstrument::Piano;

	// Default level
	CurrentLevel = ELevels::LevelOne;
}

