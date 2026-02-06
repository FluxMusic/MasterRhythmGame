// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuSettingWidget.generated.h"

class AGameHUD;
class AWorldMapHUD;
class UAudioSettingWidget;
class UGraphicSettingWidget;
class UButtonWidget;
class AMainMenuHUD;
/**
 * 
 */
UCLASS()
class MASTERRHYTHMGAME_API UMainMenuSettingWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION()
	void GraphicSettingClicked();

	UFUNCTION()
	void AudioSettingClicked();

	UFUNCTION()
	void ReturnMainMenuClicked();

	// --- Graphic Button ---
	UButtonWidget* GetGraphicButton() const { return Graphic; }

	// --- Audio Button ---
	UButtonWidget* GetAudioButton() const { return Audio; }

	// --- Main Menu Button ---
	UButtonWidget* GetMainMenuButton() const { return MainMenu; }

	// --- Graphic Setting Widget ---
	UGraphicSettingWidget* GetGraphicSettingWidget() const { return WBP_GraphicSettingWidget; }

	// --- Audio Setting Widget ---
	UAudioSettingWidget* GetAudioSettingWidget() const { return WBP_AudioSettingWidget; }

private:
	//Native Constructor
	virtual void NativeConstruct() override;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButtonWidget* Graphic{ nullptr };

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButtonWidget* Audio{ nullptr };

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButtonWidget* MainMenu{ nullptr };

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UGraphicSettingWidget* WBP_GraphicSettingWidget{ nullptr };

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UAudioSettingWidget* WBP_AudioSettingWidget{ nullptr };

	UPROPERTY()
	TObjectPtr<AMainMenuHUD> MainMenuHud { nullptr };

	UPROPERTY()
	TObjectPtr<AWorldMapHUD> WorldMapHud { nullptr };

	UPROPERTY()
	TObjectPtr<AGameHUD> GameHud { nullptr };
};
