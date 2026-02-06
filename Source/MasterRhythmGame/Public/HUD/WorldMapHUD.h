// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "UI/PauseMenuWidget.h"
#include "UI/MainMenu/SaveMenuWidget.h"
#include "UI/MainMenu/LoadMenuWidget.h"
#include "UI/Settings/AudioSettingWidget.h"
#include "UI/Settings/GraphicSettingWidget.h"
#include "UI/Settings/MainMenuSettingWidget.h"
#include "WorldMapHUD.generated.h"

/**
 * 
 */
UCLASS()
class AWorldMapHUD : public AHUD
{
	GENERATED_BODY()
public:
	AWorldMapHUD();

	// --- Pause Menu Widget Class ---
	TSubclassOf<UPauseMenuWidget> GetPauseMenuWidgetClass() const { return PauseMenuWidgetClass; }
	void SetPauseMenuWidgetClass(TSubclassOf<UPauseMenuWidget> InClass) { PauseMenuWidgetClass = InClass; }

	// --- Pause Menu Instance ---
	UPauseMenuWidget* GetPauseMenuInstance() const { return PauseMenuInstance; }
	void SetPauseMenuInstance(UPauseMenuWidget* InInstance) { PauseMenuInstance = InInstance; }

	// --- Save Menu Widget Class ---
	TSubclassOf<USaveMenuWidget> GetSaveMenuWidgetClass() const { return SaveMenuWidgetClass; }
	void SetSaveMenuWidgetClass(TSubclassOf<USaveMenuWidget> InClass) { SaveMenuWidgetClass = InClass; }

	// --- Save Menu Instance ---
	USaveMenuWidget* GetSaveMenuInstance() const { return SaveMenuInstance; }
	void SetSaveMenuInstance(USaveMenuWidget* InInstance) { SaveMenuInstance = InInstance; }

	// --- Load Menu Widget Class ---
	TSubclassOf<ULoadMenuWidget> GetLoadMenuWidgetClass() const { return LoadMenuWidgetClass; }
	void SetLoadMenuWidgetClass(TSubclassOf<ULoadMenuWidget> InClass) { LoadMenuWidgetClass = InClass; }

	// --- Load Menu Instance ---
	ULoadMenuWidget* GetLoadMenuInstance() const { return LoadMenuInstance; }
	void SetLoadMenuInstance(ULoadMenuWidget* InInstance) { LoadMenuInstance = InInstance; }

	// --- Main Menu Settings Widget Class ---
	TSubclassOf<UMainMenuSettingWidget> GetMainMenuSettingWidgetClass() const { return MainMenuSettingWidgetClass; }
	void SetMainMenuSettingWidgetClass(TSubclassOf<UMainMenuSettingWidget> InClass) { MainMenuSettingWidgetClass = InClass; }

	// --- Main Menu Settings Instance ---
	UMainMenuSettingWidget* GetMainMenuSettingInstance() const { return MainMenuSettingInstance; }
	void SetMainMenuSettingInstance(UMainMenuSettingWidget* InInstance) { MainMenuSettingInstance = InInstance; }

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

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UPauseMenuWidget> PauseMenuWidgetClass;

	UPROPERTY()
	TObjectPtr<UPauseMenuWidget> PauseMenuInstance { nullptr };

	UPROPERTY(EditAnywhere)
	TSubclassOf<USaveMenuWidget> SaveMenuWidgetClass;

	UPROPERTY()
	TObjectPtr<USaveMenuWidget> SaveMenuInstance { nullptr };

	UPROPERTY(EditAnywhere)
	TSubclassOf<ULoadMenuWidget> LoadMenuWidgetClass;

	UPROPERTY()
	TObjectPtr<ULoadMenuWidget> LoadMenuInstance { nullptr };

	UPROPERTY(EditAnywhere)
	TSubclassOf<UMainMenuSettingWidget> MainMenuSettingWidgetClass;

	UPROPERTY()
	TObjectPtr<UMainMenuSettingWidget> MainMenuSettingInstance{ nullptr };

	UPROPERTY(EditAnywhere)
	TSubclassOf<UGraphicSettingWidget> GraphicSettingClass;

	UPROPERTY()
	TObjectPtr<UGraphicSettingWidget> GraphicSettingInstance{ nullptr };

	UPROPERTY(EditAnywhere)
	TSubclassOf<UAudioSettingWidget> AudioSettingClass;

	UPROPERTY()
	TObjectPtr<UAudioSettingWidget> AudioSettingInstance{ nullptr };
};
