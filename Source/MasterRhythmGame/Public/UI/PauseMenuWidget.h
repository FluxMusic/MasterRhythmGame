// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PauseMenuWidget.generated.h"

class AWorldMapHUD;
class UButton;
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

	UFUNCTION()
	void ResumeButtonHovered();

	UFUNCTION()
	void ResumeButtonUnhovered();

	UFUNCTION()
	void SaveGameButtonHovered();

	UFUNCTION()
	void SaveGameButtonUnhovered();

	UFUNCTION()
	void LoadGameButtonHovered();

	UFUNCTION()
	void LoadGameButtonUnhovered();

	UFUNCTION()
	void SettingsButtonHovered();

	UFUNCTION()
	void SettingsButtonUnhovered();

	UFUNCTION()
	void MainMenuButtonHovered();

	UFUNCTION()
	void MainMenuButtonUnhovered();

	UFUNCTION()
	void QuitButtonHovered();

	UFUNCTION()
	void QuitButtonUnhovered();

	// --- Resume Button ---
	UButton* GetResumeButton() const { return ResumeButton; }

	// --- Save Game Button ---
	UButton* GetSaveGameButton() const { return SaveGameButton; }

	// --- Load Game Button ---
	UButton* GetLoadGameButton() const { return LoadGameButton; }

	// --- Settings Button ---
	UButton* GetSettingsButton() const { return SettingsButton; }

	// --- Main Menu Button ---
	UButton* GetMainMenuButton() const { return MainMenuButton; }

	// --- Quit Button ---
	UButton* GetQuitButton() const { return QuitButton; }

private:
	//Native Constructor
	virtual void NativeConstruct() override;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* ResumeButton { nullptr };

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* SaveGameButton { nullptr };

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* LoadGameButton { nullptr };

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* SettingsButton { nullptr };

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* MainMenuButton { nullptr };

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* QuitButton { nullptr };

	UPROPERTY()
	TObjectPtr<AWorldMapHUD> WorldMapHud { nullptr };

	const FName LevelName = FName(TEXT("MainMenu"));
};
