// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DeathWidget.generated.h"

class UButton;
/**
 * 
 */
UCLASS()
class MASTERRHYTHMGAME_API UDeathWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION()
	void RespawnButtonClicked();

	UFUNCTION()
	void RespawnButtonHovered();

	UFUNCTION()
	void RespawnButtonUnhovered();

	UFUNCTION()
	void MainMenuButtonClicked();

	UFUNCTION()
	void MainMenuButtonHovered();

	UFUNCTION()
	void MainMenuButtonUnhovered();

	UFUNCTION()
	void WorldMapButtonClicked();

	UFUNCTION()
	void WorldMapButtonHovered();

	UFUNCTION()
	void WorldMapButtonUnhovered();

	// --- Respawn Button ---
	UButton* GetRespawnButton() const { return Respawn; }

	// --- WorldMap Button ---
	UButton* GetWorldMapButton() const { return WorldMap; }

	// --- MainMenu Button ---
	UButton* GetMainMenuButton() const { return MainMenu; }

private:
	//Native Constructor
	virtual void NativeConstruct() override;

	UPROPERTY(EditAnywhere, meta = (BindWidget = true))
	UButton* Respawn { nullptr };

	UPROPERTY(EditAnywhere, meta = (BindWidget = true))
	UButton* WorldMap { nullptr };

	UPROPERTY(EditAnywhere, meta = (BindWidget = true))
	UButton* MainMenu { nullptr };

	const FName LevelName = FName(TEXT("WorldMap"));

	const FName MainMenuName = FName(TEXT("MainMenu"));
};
