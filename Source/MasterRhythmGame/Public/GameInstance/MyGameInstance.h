// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
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

private:

	float MasterVolume { 0.75f };
	float MusicVolume { 0.55f };
	float SFXVolume { 0.75f };
};
