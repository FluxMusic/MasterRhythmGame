// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Settings/AudioSettingWidget.h"
#include "HUD/MainMenuHUD.h"
#include "Components/Button.h"
#include "Components/Slider.h"
#include "Controller/MainMenuController.h"
#include "Sound/SoundSubmix.h"

void UAudioSettingWidget::OnValueChangedMaster(float Value)
{
	if (MasterSubmix != nullptr)
	{
		MasterSubmix->SetSubmixOutputVolume(this, Value);
	}
}

void UAudioSettingWidget::OnValueChangedMusic(float Value)
{
	if (MusicSubmix != nullptr)
	{
		MusicSubmix->SetSubmixOutputVolume(this, Value);
	}
}

void UAudioSettingWidget::OnValueChangedSfx(float Value)
{
	if (SfxSubmix != nullptr)
	{
		SfxSubmix->SetSubmixOutputVolume(this, Value);
	}
}

void UAudioSettingWidget::ReturnMenu()
{
	SetVisibility(ESlateVisibility::Hidden);
	AMainMenuController* PlayerController = Cast<AMainMenuController>(GetOwningPlayer());
	if (PlayerController != nullptr)
	{
		PlayerController->SetControllerState(EControllerState::SettingMenu);
	}
	ReturnMenuUnhovered();
}

void UAudioSettingWidget::ReturnSettingMenuHovered()
{
	MainMenu->SetBackgroundColor(FLinearColor::Green);

	MasterVolumeSlider->SetSliderBarColor(FLinearColor::White);
	MusicVolumeSlider->SetSliderBarColor(FLinearColor::White);
	SfxVolumeSlider->SetSliderBarColor(FLinearColor::White);
}

void UAudioSettingWidget::MasterVolumeSliderHovered()
{
	MasterVolumeSlider->SetSliderBarColor(FLinearColor::Green);

	MusicVolumeSlider->SetSliderBarColor(FLinearColor::White);
	SfxVolumeSlider->SetSliderBarColor(FLinearColor::White);
	MainMenu->SetBackgroundColor(FLinearColor::White);
}

void UAudioSettingWidget::MusicVolumeSliderHovered()
{
	MusicVolumeSlider->SetSliderBarColor(FLinearColor::Green);

	MasterVolumeSlider->SetSliderBarColor(FLinearColor::White);
	SfxVolumeSlider->SetSliderBarColor(FLinearColor::White);
	MainMenu->SetBackgroundColor(FLinearColor::White);
}

void UAudioSettingWidget::SfxVolumeSliderHovered()
{
	SfxVolumeSlider->SetSliderBarColor(FLinearColor::Green);

	MasterVolumeSlider->SetSliderBarColor(FLinearColor::White);
	MusicVolumeSlider->SetSliderBarColor(FLinearColor::White);
	MainMenu->SetBackgroundColor(FLinearColor::White);
}

void UAudioSettingWidget::ReturnMenuUnhovered()
{
	MainMenu->SetBackgroundColor(FLinearColor::White);
}

void UAudioSettingWidget::MasterVolumeSliderUnhovered()
{
	MasterVolumeSlider->SetSliderBarColor(FLinearColor::White);
}

void UAudioSettingWidget::MusicVolumeSliderUnhovered()
{
	MusicVolumeSlider->SetSliderBarColor(FLinearColor::White);
}

void UAudioSettingWidget::SfxVolumeSliderUnhovered()
{
	SfxVolumeSlider->SetSliderBarColor(FLinearColor::White);
}

void UAudioSettingWidget::NativeConstruct()
{
	Super::NativeConstruct();

	MasterVolumeSlider->OnValueChanged.AddDynamic(this, &UAudioSettingWidget::OnValueChangedMaster);
	MusicVolumeSlider->OnValueChanged.AddDynamic(this, &UAudioSettingWidget::OnValueChangedMusic);
	SfxVolumeSlider->OnValueChanged.AddDynamic(this, &UAudioSettingWidget::OnValueChangedSfx);

	MainMenu->OnClicked.AddDynamic(this, &UAudioSettingWidget::ReturnMenu);

	MasterVolumeSlider->SetSliderBarColor(FLinearColor::Green);
}