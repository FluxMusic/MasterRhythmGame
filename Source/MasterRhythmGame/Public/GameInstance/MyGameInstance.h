// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../CoreTypes.h"
#include "Engine/GameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "Data/RhythmSaveGame.h"
#include "MyGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class MASTERRHYTHMGAME_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	// Audio Settings
	float GetMasterVolume() const { return MasterVolume; }
	void SetMasterVolume(float Volume) { MasterVolume = Volume; }

	float GetMusicVolume() const { return MusicVolume; }
	void SetMusicVolume(float Volume) { MusicVolume = Volume; }

	float GetSFXVolume() const { return SFXVolume; }
	void SetSFXVolume(float Volume) { SFXVolume = Volume; }

	// Save/Load System
	UFUNCTION(BlueprintCallable)
	bool SaveGameToSlot(const FString& SlotName = TEXT("SaveSlot1"));

	UFUNCTION(BlueprintCallable)
	bool LoadGameFromSlot(const FString& SlotName = TEXT("SaveSlot1"));

	UFUNCTION(BlueprintCallable)
	bool DoesSaveGameExist(const FString& SlotName = TEXT("SaveSlot1")) const;

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void SaveGame();

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void LoadGame();

	UFUNCTION(BlueprintCallable)
	void BeforeSaveGame();

	UFUNCTION(BlueprintCallable)
	void OnLoadedGame();

	// Level Management
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void LoadLevel(FName LevelName);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void UnloadLevel(FName LevelName);

	// Level Unlock States
	UFUNCTION(BlueprintCallable)
	bool GetLevelOneUnlocked() const { return bIsLevelOneUnlocked; }
	UFUNCTION(BlueprintCallable)
	void SetLevelOneUnlocked(bool bUnlocked) { bIsLevelOneUnlocked = bUnlocked; }

	UFUNCTION(BlueprintCallable)
	bool GetLevelTwoUnlocked() const { return bIsLevelTwoUnlocked; }
	UFUNCTION(BlueprintCallable)
	void SetLevelTwoUnlocked(bool bUnlocked) { bIsLevelTwoUnlocked = bUnlocked; }

	UFUNCTION(BlueprintCallable)
	bool GetLevelThreeUnlocked() const { return bIsLevelThreeUnlocked; }
	UFUNCTION(BlueprintCallable)
	void SetLevelThreeUnlocked(bool bUnlocked) { bIsLevelThreeUnlocked = bUnlocked; }

	UFUNCTION(BlueprintCallable)
	bool GetLevelFourUnlocked() const { return bIsLevelFourUnlocked; }
	UFUNCTION(BlueprintCallable)
	void SetLevelFourUnlocked(bool bUnlocked) { bIsLevelFourUnlocked = bUnlocked; }

	UFUNCTION(BlueprintCallable)
	bool GetLevelFiveUnlocked() const { return bIsLevelFiveUnlocked; }
	UFUNCTION(BlueprintCallable)
	void SetLevelFiveUnlocked(bool bUnlocked) { bIsLevelFiveUnlocked = bUnlocked; }

	UFUNCTION(BlueprintCallable)
	bool GetLevelSixUnlocked() const { return bIsLevelSixUnlocked; }
	UFUNCTION(BlueprintCallable)
	void SetLevelSixUnlocked(bool bUnlocked) { bIsLevelSixUnlocked = bUnlocked; }

	// Level Scores
	UFUNCTION(BlueprintCallable)
	int32 GetLevelScore(ELevels Level) const;
	
	UFUNCTION(BlueprintCallable)
	void SetLevelScore(ELevels Level, int32 Score);

	// Instrument Management
	UFUNCTION(BlueprintCallable)
	void SetInstrumentUnlocked(const EInstrument& Instrument, bool bUnlocked);
	
	UFUNCTION(BlueprintCallable)
	EInstrument GetUnlockedInstruments();

	// Current Level
	UFUNCTION(BlueprintCallable)
	void SetCurrentLevel(ELevels Level) { CurrentLevel = Level; }
	UFUNCTION(BlueprintCallable)
	ELevels GetCurrentLevel() { return CurrentLevel; }

	static inline UMyGameInstance* Get() { return Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GWorld)); }

private:
	// Audio Settings
	float MasterVolume { 0.75f };
	float MusicVolume { 0.55f };
	float SFXVolume { 0.75f };

	// Level Unlock States
	bool bIsLevelOneUnlocked   { true };
	bool bIsLevelTwoUnlocked   { false };
	bool bIsLevelThreeUnlocked { false };
	bool bIsLevelFourUnlocked  { false };
	bool bIsLevelFiveUnlocked  { false };
	bool bIsLevelSixUnlocked   { false };

	// Level Scores
	int32 LevelOneScore   { 0 };
	int32 LevelTwoScore   { 0 };
	int32 LevelThreeScore { 0 };
	int32 LevelFourScore  { 0 };
	int32 LevelFiveScore  { 0 };
	int32 LevelSixScore   { 0 };

	// Instrument Management
	UPROPERTY()
	EInstrument UnlockedInstruments { EInstrument::Piano };

	// Current Level
	UPROPERTY()
	ELevels CurrentLevel { ELevels::LevelOne };

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = true))
	TObjectPtr<URhythmSaveGame> RhythmSaveGame { nullptr };
};
