// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "UI/PauseMenuWidget.h"
#include "UI/WorldMapWidget_TEMPORARILY.h"
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

	// --- World Map Widget Class ---
	TSubclassOf<UWorldMapWidget_TEMPORARILY> GetWorldMapWidgetClass() const { return WorldMapWidgetClass; }
	void SetWorldMapWidgetClass(TSubclassOf<UWorldMapWidget_TEMPORARILY> InClass) { WorldMapWidgetClass = InClass; }

	// --- World Map Instance ---
	UWorldMapWidget_TEMPORARILY* GetWorldMapInstance () const { return WorldMapWidgetInstance; }
	void SetWorldMapInstance(UWorldMapWidget_TEMPORARILY* InInstance) { WorldMapWidgetInstance = InInstance; }

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
	TSubclassOf<UWorldMapWidget_TEMPORARILY> WorldMapWidgetClass;

	UPROPERTY()
	TObjectPtr<UWorldMapWidget_TEMPORARILY> WorldMapWidgetInstance { nullptr };

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
