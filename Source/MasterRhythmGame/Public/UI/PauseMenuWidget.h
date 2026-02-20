// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PauseMenuWidget.generated.h"

class AGameHUD;
class AWorldMapHUD;
class UButtonWidget;
/**
 * 
 */
UCLASS()
class MASTERRHYTHMGAME_API UPauseMenuWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION()
	void ResumeButtonClicked();

	UFUNCTION()
	void SaveGameButtonClicked();

	UFUNCTION()
	void LoadGameButtonClicked();

	UFUNCTION()
	void SettingsButtonClicked();

	UFUNCTION()
	void MainMenuButtonClicked();

	UFUNCTION()
	void QuitButtonClicked();

	// --- Resume Button ---
	UButtonWidget* GetResumeButton() const { return ResumeButton; }

	// --- Save Game Button ---
	UButtonWidget* GetSaveGameButton() const { return SaveGameButton; }

	// --- Load Game Button ---
	UButtonWidget* GetLoadGameButton() const { return LoadGameButton; }

	// --- Settings Button ---
	UButtonWidget* GetSettingsButton() const { return SettingsButton; }

	// --- Main Menu Button ---
	UButtonWidget* GetMainMenuButton() const { return MainMenuButton; }

	// --- Quit Button ---
	UButtonWidget* GetQuitButton() const { return QuitButton; }

private:
	//Native Constructor
	virtual void NativeConstruct() override;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButtonWidget* ResumeButton { nullptr };

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButtonWidget* SaveGameButton { nullptr };

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButtonWidget* LoadGameButton { nullptr };

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButtonWidget* SettingsButton { nullptr };

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButtonWidget* MainMenuButton { nullptr };

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButtonWidget* QuitButton { nullptr };

	UPROPERTY()
	TObjectPtr<AWorldMapHUD> WorldMapHud { nullptr };

	UPROPERTY()
	TObjectPtr<AGameHUD> GameHUD { nullptr };

	const FName LevelName = FName(TEXT("MAP_MainMenu"));
};
