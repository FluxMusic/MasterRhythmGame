// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "UI/MainMenu/MainMenuWidget.h"
#include "UI/MainMenu/CreditsWidget.h"
#include "UI/Settings/AudioSettingWidget.h"
#include "UI/Settings/GraphicSettingWidget.h"
#include "UI/Settings/MainMenuSetting.h"
#include "MainMenuHUD.generated.h"

/**
 * 
 */
UCLASS()
class MASTERRHYTHMGAME_API AMainMenuHUD : public AHUD
{
	GENERATED_BODY()
public:
	AMainMenuHUD();

	UPROPERTY(EditAnywhere)
	TSubclassOf<UMainMenuWidget> MainMenuWidgetClass;

	UPROPERTY()
	TObjectPtr<UMainMenuWidget> MainMenuInstance { nullptr };

	UPROPERTY(EditAnywhere)
	TSubclassOf<UCreditsWidget> CreditsWidgetClass;

	UPROPERTY()
	TObjectPtr<UCreditsWidget> CreditsMenuInstance { nullptr };

	UPROPERTY(EditAnywhere)
	TSubclassOf<UMainMenuSetting> MainMenuSettingWidgetClass;

	UPROPERTY()
	TObjectPtr<UMainMenuSetting> MainMenuSettingInstance { nullptr };

	UPROPERTY(EditAnywhere)
	TSubclassOf<UGraphicSettingWidget> GraphicSettingClass;

	UPROPERTY()
	TObjectPtr<UGraphicSettingWidget> GraphicSettingInstance { nullptr };

	UPROPERTY(EditAnywhere)
	TSubclassOf<UAudioSettingWidget> AudioSettingClass;

	UPROPERTY()
	TObjectPtr<UAudioSettingWidget> AudioSettingInstance { nullptr };

protected:
	virtual void BeginPlay() override;
};