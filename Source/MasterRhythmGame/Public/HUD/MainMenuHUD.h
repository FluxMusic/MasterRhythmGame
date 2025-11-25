// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "UI/MainMenu/MainMenuWidget.h"
#include "UI/MainMenu/CreditsWidget.h"
#include "UI/MainMenu/LoadMenuWidget.h"
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

	// --- Main Menu Widget Class ---
	TSubclassOf<UMainMenuWidget> GetMainMenuWidgetClass() const { return MainMenuWidgetClass; }
	void SetMainMenuWidgetClass(TSubclassOf<UMainMenuWidget> InClass) { MainMenuWidgetClass = InClass; }

	// --- Main Menu Instance ---
	UMainMenuWidget* GetMainMenuInstance() const { return MainMenuInstance; }
	void SetMainMenuInstance(UMainMenuWidget* InInstance) { MainMenuInstance = InInstance; }
	
	// --- Credits Widget Class ---
	TSubclassOf<UCreditsWidget> GetCreditsWidgetClass() const { return CreditsWidgetClass; }
	void SetCreditsWidgetClass(TSubclassOf<UCreditsWidget> InClass) { CreditsWidgetClass = InClass; }

	// --- Credits Widget Instance ---
	UCreditsWidget* GetCreditsMenuInstance() const { return CreditsMenuInstance; }
	void SetCreditsMenuInstance(UCreditsWidget* InInstance) { CreditsMenuInstance = InInstance; }

	// --- Main Menu Settings Widget Class ---
	TSubclassOf<UMainMenuSetting> GetMainMenuSettingWidgetClass() const { return MainMenuSettingWidgetClass; }
	void SetMainMenuSettingWidgetClass(TSubclassOf<UMainMenuSetting> InClass) { MainMenuSettingWidgetClass = InClass; }

	// --- Main Menu Settings Instance ---
	UMainMenuSetting* GetMainMenuSettingInstance() const { return MainMenuSettingInstance; }
	void SetMainMenuSettingInstance(UMainMenuSetting* InInstance) { MainMenuSettingInstance = InInstance; }

	// --- Graphic Setting Class ---
	TSubclassOf<UGraphicSettingWidget> GetGraphicSettingClass() const { return GraphicSettingClass; }
	void SetGraphicSettingClass(TSubclassOf<UGraphicSettingWidget> InClass) { GraphicSettingClass = InClass; }

	// --- Graphic Setting Instance ---
	UGraphicSettingWidget* GetGraphicSettingInstance() const { return GraphicSettingInstance; }
	void SetGraphicSettingInstance(UGraphicSettingWidget* InInstance) { GraphicSettingInstance = InInstance; }

	// --- Audio Setting Class ---
	TSubclassOf<UAudioSettingWidget> GetAudioSettingClass() const { return AudioSettingClass; }
	void SetAudioSettingClass(TSubclassOf<UAudioSettingWidget> InClass) { AudioSettingClass = InClass; }

	// --- Audio Setting Instance ---
	UAudioSettingWidget* GetAudioSettingInstance() const { return AudioSettingInstance; }
	void SetAudioSettingInstance(UAudioSettingWidget* InInstance) { AudioSettingInstance = InInstance; }

	// --- Load Menu Class ---
	TSubclassOf<ULoadMenuWidget> GetLoadMenuClass() const { return LoadMenuClass; }
	void SetLoadMenuClass(TSubclassOf<ULoadMenuWidget> InClass) { LoadMenuClass = InClass; }

	// --- Load Menu Instance ---
	ULoadMenuWidget* GetLoadMenuInstance() const { return LoadMenuInstance; }
	void SetLoadMenuInstance(ULoadMenuWidget* InInstance) { LoadMenuInstance = InInstance; }

protected:
	virtual void BeginPlay() override;

private:
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

	UPROPERTY(EditAnywhere)
	TSubclassOf<ULoadMenuWidget> LoadMenuClass;

	UPROPERTY()
	TObjectPtr<ULoadMenuWidget> LoadMenuInstance { nullptr };
};