// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuSetting.generated.h"

class AMainMenuHUD;
/**
 * 
 */
UCLASS()
class MASTERRHYTHMGAME_API UMainMenuSetting : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION()
	void GraphicSettingClicked();

	UFUNCTION()
	void AudioSettingClicked();

	UFUNCTION()
	void ReturnMainMenuClicked();

	UFUNCTION()
	void GraphicButtonHovered();

	UFUNCTION()
	void GraphicButtonUnhovered();

	UFUNCTION()
	void AudioButtonHovered();

	UFUNCTION()
	void AudioButtonUnhovered();

	UFUNCTION()
	void ReturnMainMenuButtonHovered();

	UFUNCTION()
	void ReturnMainMenuButtonUnhovered();

	// --- Graphic Button ---
	UButton* GetGraphicButton() const { return Graphic; }

	// --- Audio Button ---
	UButton* GetAudioButton() const { return Audio; }

	// --- Main Menu Button ---
	UButton* GetMainMenuButton() const { return MainMenu; }

	// --- Graphic Setting Widget ---
	UGraphicSettingWidget* GetGraphicSettingWidget() const { return WBP_GraphicSettingWidget; }

	// --- Audio Setting Widget ---
	UAudioSettingWidget* GetAudioSettingWidget() const { return WBP_AudioSettingWidget; }

private:
	//Native Constructor
	virtual void NativeConstruct() override;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* Graphic{ nullptr };

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* Audio{ nullptr };

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* MainMenu{ nullptr };

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UGraphicSettingWidget* WBP_GraphicSettingWidget{ nullptr };

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UAudioSettingWidget* WBP_AudioSettingWidget{ nullptr };

	UPROPERTY()
	TObjectPtr<AMainMenuHUD> MainMenuHud { nullptr };
};
