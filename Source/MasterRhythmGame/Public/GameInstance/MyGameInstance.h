// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../CoreTypes.h"
#include "Engine/GameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "MyGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class MASTERRHYTHMGAME_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()
public:
	float GetMasterVolume() const { return MasterVolume; }
	void SetMasterVolume(float Volume) { MasterVolume = Volume; }

	float GetMusicVolume() const { return MusicVolume; }
	void SetMusicVolume(float Volume) { MusicVolume = Volume; }

	float GetSFXVolume() const { return SFXVolume; }
	void SetSFXVolume(float Volume) { SFXVolume = Volume; }

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void LoadLevel(FName LevelName);

	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void UnloadLevel(FName LevelName);

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

	UFUNCTION(BlueprintCallable)
	void SetInstrumentUnlocked(const EInstrument& Instrument, bool bUnlocked);
	
	UFUNCTION(BlueprintCallable)
	EInstrument GetUnlockedInstruments();

	static inline UMyGameInstance* Get() { return Cast<UMyGameInstance>(UGameplayStatics::GetGameInstance(GWorld)); }

private:

	float MasterVolume { 0.75f };
	float MusicVolume { 0.55f };
	float SFXVolume { 0.75f };

	bool bIsLevelOneUnlocked   { true };
	bool bIsLevelTwoUnlocked   { false };
	bool bIsLevelThreeUnlocked { false };
	bool bIsLevelFourUnlocked  { false };
	bool bIsLevelFiveUnlocked  { false };
	bool bIsLevelSixUnlocked   { false };

	//Testing Purposes
	//TODO: Set Instrument to piano (always unlocked)
	UPROPERTY()
	EInstrument UnlockedInstruments { EInstrument::All };
};
