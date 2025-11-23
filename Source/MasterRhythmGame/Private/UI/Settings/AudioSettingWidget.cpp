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
}

void UAudioSettingWidget::OnValueChangedSfx(float Value)
{
}

void UAudioSettingWidget::ReturnMenu()
{
}

void UAudioSettingWidget::NativeConstruct()
{
	Super::NativeConstruct();

	MasterVolumeSlider->OnValueChanged.AddDynamic(this, &UAudioSettingWidget::OnValueChangedMaster);
	MusicVolumeSlider->OnValueChanged.AddDynamic(this, &UAudioSettingWidget::OnValueChangedMusic);
	SfxVolumeSlider->OnValueChanged.AddDynamic(this, &UAudioSettingWidget::OnValueChangedSfx);
}
