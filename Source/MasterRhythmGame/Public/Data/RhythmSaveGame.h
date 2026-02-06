// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "../CoreTypes.h"
#include "RhythmSaveGame.generated.h"

/**
 *
 */
UCLASS(BlueprintType)
class MASTERRHYTHMGAME_API URhythmSaveGame : public USaveGame
{
	GENERATED_BODY()
public:

	UPROPERTY(VisibleAnywhere, Category = "SaveGame")
	FString SaveSlotName;

	UPROPERTY(VisibleAnywhere, Category = "SaveGame")
	uint32 UserIndex;

	// Audio Settings
	UPROPERTY(VisibleAnywhere, Category = "Settings")
	float MasterVolume;

	UPROPERTY(VisibleAnywhere, Category = "Settings")
	float MusicVolume;

	UPROPERTY(VisibleAnywhere, Category = "Settings")
	float SfxVolume;

	// Level Unlock States
	UPROPERTY(VisibleAnywhere, Category = "Progress")
	bool bIsLevelOneUnlocked;

	UPROPERTY(VisibleAnywhere, Category = "Progress")
	bool bIsLevelTwoUnlocked;

	UPROPERTY(VisibleAnywhere, Category = "Progress")
	bool bIsLevelThreeUnlocked;

	UPROPERTY(VisibleAnywhere, Category = "Progress")
	bool bIsLevelFourUnlocked;

	UPROPERTY(VisibleAnywhere, Category = "Progress")
	bool bIsLevelFiveUnlocked;

	UPROPERTY(VisibleAnywhere, Category = "Progress")
	bool bIsLevelSixUnlocked;

	// Level Scores
	UPROPERTY(VisibleAnywhere, Category = "Progress")
	int32 LevelOneScore;

	UPROPERTY(VisibleAnywhere, Category = "Progress")
	int32 LevelTwoScore;

	UPROPERTY(VisibleAnywhere, Category = "Progress")
	int32 LevelThreeScore;

	UPROPERTY(VisibleAnywhere, Category = "Progress")
	int32 LevelFourScore;

	UPROPERTY(VisibleAnywhere, Category = "Progress")
	int32 LevelFiveScore;

	UPROPERTY(VisibleAnywhere, Category = "Progress")
	int32 LevelSixScore;

	// Unlocked Instruments
	UPROPERTY(VisibleAnywhere, Category = "Progress")
	EInstrument UnlockedInstruments;

	// Current Level
	UPROPERTY(VisibleAnywhere, Category = "Progress")
	ELevels CurrentLevel;

	URhythmSaveGame();
};
