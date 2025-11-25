// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AudioSettingWidget.generated.h"

class UButton;
class USlider;
class AMainMenuHUD;
class USoundSubmix;
/**
 * 
 */
UCLASS()
class MASTERRHYTHMGAME_API UAudioSettingWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION()
	void OnValueChangedMaster(float Value);

	UFUNCTION()
	void OnValueChangedMusic(float Value);

	UFUNCTION()
	void OnValueChangedSfx(float Value);

	UFUNCTION()
	void ReturnMenu();

	UFUNCTION()
	void ReturnSettingMenuHovered();

	UFUNCTION()
	void MasterVolumeSliderHovered();

	UFUNCTION()
	void MusicVolumeSliderHovered();

	UFUNCTION()
	void SfxVolumeSliderHovered();

	UFUNCTION()
	void ReturnMenuUnhovered();

	UFUNCTION()
	void MasterVolumeSliderUnhovered();

	UFUNCTION()
	void MusicVolumeSliderUnhovered();

	UFUNCTION()
	void SfxVolumeSliderUnhovered();

	// --- Master Volume Slider ---
	USlider* GetMasterVolumeSlider() const { return MasterVolumeSlider; }

	// --- Music Volume Slider ---
	USlider* GetMusicVolumeSlider() const { return MusicVolumeSlider; }

	// --- SFX Volume Slider ---
	USlider* GetSfxVolumeSlider() const { return SfxVolumeSlider; }

	// --- Main Menu Button ---
	UButton* GetMainMenuButton() const { return MainMenu; }

	// --- Master Submix ---
	USoundSubmix* GetMasterSubmix() const { return MasterSubmix; }

	// --- Music Submix ---
	USoundSubmix* GetMusicSubmix() const { return MusicSubmix; }

	// --- SFX Submix ---
	USoundSubmix* GetSfxSubmix() const { return SfxSubmix; }

	

private:
	//Native Constructor
	virtual void NativeConstruct() override;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class USlider* MasterVolumeSlider{ nullptr };

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class USlider* MusicVolumeSlider{ nullptr };

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class USlider* SfxVolumeSlider{ nullptr };

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* MainMenu{ nullptr };

	UPROPERTY(EditAnywhere)
	class USoundSubmix* MasterSubmix{ nullptr };

	UPROPERTY(EditAnywhere)
	class USoundSubmix* MusicSubmix{ nullptr };

	UPROPERTY(EditAnywhere)
	class USoundSubmix* SfxSubmix{ nullptr };

	UPROPERTY()
	TObjectPtr<AMainMenuHUD> MainMenuHud { nullptr };
};
