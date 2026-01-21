// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Settings/AudioSettingWidget.h"
#include "UI/ButtonWidget.h"
#include "Components/Slider.h"
#include "Controller/MainMenuController.h"
#include "Controller/WorldMapController.h"
#include "GameInstance/MyGameInstance.h"
#include "Sound/SoundSubmix.h"

void UAudioSettingWidget::OnValueChangedMaster(float Value)
{
	if (MasterSubmix != nullptr)
	{
		MasterSubmix->SetSubmixOutputVolume(this, Value);
	}

	if (auto GM = Cast<UMyGameInstance>(GetWorld()->GetGameInstance()))
	{
		GM->SetMasterVolume(Value);
	}
}

void UAudioSettingWidget::OnValueChangedMusic(float Value)
{
	if (MusicSubmix != nullptr)
	{
		MusicSubmix->SetSubmixOutputVolume(this, Value);
	}

	if (auto GM = Cast<UMyGameInstance>(GetWorld()->GetGameInstance()))
	{
		GM->SetMusicVolume(Value);
	}
}

void UAudioSettingWidget::OnValueChangedSfx(float Value)
{
	if (SfxSubmix != nullptr)
	{
		SfxSubmix->SetSubmixOutputVolume(this, Value);
	}

	if (auto GM = Cast<UMyGameInstance>(GetWorld()->GetGameInstance()))
	{
		GM->SetSFXVolume(Value);
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

	AWorldMapController* WorldMapController = Cast<AWorldMapController>(GetOwningPlayer());
	if (WorldMapController != nullptr)
	{
		WorldMapController->SetControllerState(EControllerStateWorldMap::SettingMenu);
	}
}

void UAudioSettingWidget::MasterVolumeSliderHovered()
{
	FSliderStyle HoveredStyle = MasterVolumeSlider->GetWidgetStyle();
	MasterVolumeSlider->SetWidgetStyle(HoveredStyle.SetHoveredBarImage(HoveredStyle.HoveredBarImage));
}

void UAudioSettingWidget::MusicVolumeSliderHovered()
{
	FSliderStyle HoveredStyle = MusicVolumeSlider->GetWidgetStyle();
	MusicVolumeSlider->SetWidgetStyle(HoveredStyle.SetHoveredBarImage(HoveredStyle.HoveredBarImage));
}

void UAudioSettingWidget::SfxVolumeSliderHovered()
{
	FSliderStyle HoveredStyle = SfxVolumeSlider->GetWidgetStyle();
	SfxVolumeSlider->SetWidgetStyle(HoveredStyle.SetHoveredBarImage(HoveredStyle.HoveredBarImage));
}

void UAudioSettingWidget::MasterVolumeSliderUnhovered()
{
	FSliderStyle NormalStyle = MasterVolumeSlider->GetWidgetStyle();
	MasterVolumeSlider->SetWidgetStyle(NormalStyle.SetNormalBarImage(NormalStyle.NormalBarImage));
}

void UAudioSettingWidget::MusicVolumeSliderUnhovered()
{
	FSliderStyle NormalStyle = MusicVolumeSlider->GetWidgetStyle();
	MusicVolumeSlider->SetWidgetStyle(NormalStyle.SetNormalBarImage(NormalStyle.NormalBarImage));
}

void UAudioSettingWidget::SfxVolumeSliderUnhovered()
{
	FSliderStyle NormalStyle = SfxVolumeSlider->GetWidgetStyle();
	SfxVolumeSlider->SetWidgetStyle(NormalStyle.SetNormalBarImage(NormalStyle.NormalBarImage));
}

void UAudioSettingWidget::NativeConstruct()
{
	Super::NativeConstruct();

	MasterVolumeSlider->OnValueChanged.AddDynamic(this, &UAudioSettingWidget::OnValueChangedMaster);
	MusicVolumeSlider->OnValueChanged.AddDynamic(this, &UAudioSettingWidget::OnValueChangedMusic);
	SfxVolumeSlider->OnValueChanged.AddDynamic(this, &UAudioSettingWidget::OnValueChangedSfx);

	MainMenu->OnClicked.AddDynamic(this, &UAudioSettingWidget::ReturnMenu);

	if (auto GM = Cast<UMyGameInstance>(GetWorld()->GetGameInstance()))
	{
		MasterVolumeSlider->SetValue(GM->GetMasterVolume());
		MusicVolumeSlider->SetValue(GM->GetMusicVolume());
		SfxVolumeSlider->SetValue(GM->GetSFXVolume());
	}
}