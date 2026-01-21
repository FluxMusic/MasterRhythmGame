// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AudioSettingWidget.generated.h"

class AWorldMapHUD;
class UButtonWidget;
class USlider;
class AMainMenuHUD;
class USoundSubmix;
/**
 * 
 */
UCLASS()
class MASTERRHYTHMGAME_API	UAudioSettingWidget : public UUserWidget
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
	void MasterVolumeSliderHovered();

	UFUNCTION()
	void MusicVolumeSliderHovered();

	UFUNCTION()
	void SfxVolumeSliderHovered();

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
	UButtonWidget* GetMainMenuButton() const { return MainMenu; }

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
	USlider* MasterVolumeSlider{ nullptr };

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	USlider* MusicVolumeSlider{ nullptr };

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	USlider* SfxVolumeSlider{ nullptr };

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButtonWidget* MainMenu{ nullptr };

	UPROPERTY(EditAnywhere)
	USoundSubmix* MasterSubmix{ nullptr };

	UPROPERTY(EditAnywhere)
	USoundSubmix* MusicSubmix{ nullptr };

	UPROPERTY(EditAnywhere)
	USoundSubmix* SfxSubmix{ nullptr };

	UPROPERTY()
	TObjectPtr<AMainMenuHUD> MainMenuHud { nullptr };

	UPROPERTY()
	TObjectPtr<AWorldMapHUD> WorldMapHud { nullptr };
};
