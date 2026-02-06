// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "UI/PauseMenuWidget.h"
#include "UI/Game/BlackscreenWidget.h"
#include "UI/Game/DeathWidget.h"
#include "UI/Game/SuccessWidget.h"
#include "UI/Game/MainGameWidget.h"
#include "UI/MainMenu/SaveMenuWidget.h"
#include "UI/MainMenu/LoadMenuWidget.h"
#include "UI/Settings/AudioSettingWidget.h"
#include "UI/Settings/GraphicSettingWidget.h"
#include "UI/Settings/MainMenuSettingWidget.h"
#include "GameHUD.generated.h"

/**
 * 
 */
UCLASS()
class MASTERRHYTHMGAME_API AGameHUD : public AHUD
{
	GENERATED_BODY()
public:
	AGameHUD();

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

	// --- Main Game Class ---
	TSubclassOf<UMainGameWidget> GetMainGameClass() const { return MainGameClass; }
	void SetMainGameClass(TSubclassOf<UMainGameWidget> InClass) { MainGameClass = InClass; }

	// --- Main Game Instance ---
	UMainGameWidget* GetMainGameInstance() const { return MainGameInstance; }
	void SetMainGameInstance(UMainGameWidget* InInstance) { MainGameInstance = InInstance; }

	// --- Death Widget Class ---
	TSubclassOf<UDeathWidget> GetDeathWidgetClass() const { return DeathWidgetClass; }
	void SetDeathWidgetClass(TSubclassOf<UDeathWidget> InClass) { DeathWidgetClass = InClass; }

	// --- Death Widget Instance ---
	UDeathWidget* GetDeathWidgetInstance() const { return DeathWidgetInstance; }
	void SetDeathWidgetInstance(UDeathWidget* InInstance) { DeathWidgetInstance = InInstance; }
	
	// --- Success Widget Class ---
	TSubclassOf<USuccessWidget> GetSuccessWidgetClass() const { return SuccessWidgetClass; }
	void SetSuccessWidgetClass(TSubclassOf<USuccessWidget> InClass) { SuccessWidgetClass = InClass; }

	// --- Success Widget Instance ---
	USuccessWidget* GetSuccessWidgetInstance() const { return SuccessWidgetInstance; }
	void SetSuccessWidgetInstance(USuccessWidget* InInstance) { SuccessWidgetInstance = InInstance; }

	// --- Blackscreen Class ---
	TSubclassOf<UBlackscreenWidget> GetBlackscreenClass() const { return BlackscreenClass; }
	void SetBlackscreenClass(TSubclassOf<UBlackscreenWidget> InClass) { BlackscreenClass = InClass; }

	// --- Blackscreen Instance ---
	UBlackscreenWidget* GetBlackscreenInstance() const { return BlackscreenInstance; }
	void SetBlackscreenInstance(UBlackscreenWidget* InInstance) { BlackscreenInstance = InInstance; }

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UPauseMenuWidget> PauseMenuWidgetClass { nullptr };

	UPROPERTY()
	TObjectPtr<UPauseMenuWidget> PauseMenuInstance { nullptr };

	UPROPERTY(EditAnywhere)
	TSubclassOf<USaveMenuWidget> SaveMenuWidgetClass { nullptr };

	UPROPERTY()
	TObjectPtr<USaveMenuWidget> SaveMenuInstance { nullptr };

	UPROPERTY(EditAnywhere)
	TSubclassOf<ULoadMenuWidget> LoadMenuWidgetClass { nullptr };

	UPROPERTY()
	TObjectPtr<ULoadMenuWidget> LoadMenuInstance { nullptr };

	UPROPERTY(EditAnywhere)
	TSubclassOf<UMainMenuSettingWidget> MainMenuSettingWidgetClass { nullptr };

	UPROPERTY()
	TObjectPtr<UMainMenuSettingWidget> MainMenuSettingInstance { nullptr };

	UPROPERTY(EditAnywhere)
	TSubclassOf<UGraphicSettingWidget> GraphicSettingClass { nullptr };

	UPROPERTY()
	TObjectPtr<UGraphicSettingWidget> GraphicSettingInstance { nullptr };

	UPROPERTY(EditAnywhere)
	TSubclassOf<UAudioSettingWidget> AudioSettingClass { nullptr };

	UPROPERTY()
	TObjectPtr<UAudioSettingWidget> AudioSettingInstance { nullptr };

	UPROPERTY(EditAnywhere)
	TSubclassOf<UMainGameWidget> MainGameClass { nullptr };

	UPROPERTY()
	TObjectPtr<UMainGameWidget> MainGameInstance { nullptr };

	UPROPERTY(EditAnywhere)
	TSubclassOf<UDeathWidget> DeathWidgetClass { nullptr };

	UPROPERTY()
	TObjectPtr<UDeathWidget> DeathWidgetInstance { nullptr };
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<USuccessWidget> SuccessWidgetClass { nullptr };

	UPROPERTY()
	TObjectPtr<USuccessWidget> SuccessWidgetInstance { nullptr };

	UPROPERTY(EditAnywhere)
	TSubclassOf<UBlackscreenWidget> BlackscreenClass { nullptr };

	UPROPERTY()
	TObjectPtr<UBlackscreenWidget> BlackscreenInstance { nullptr };
};
