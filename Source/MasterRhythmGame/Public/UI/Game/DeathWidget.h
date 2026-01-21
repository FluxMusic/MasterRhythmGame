// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DeathWidget.generated.h"

class UButtonWidget;
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
	void MainMenuButtonClicked();

	UFUNCTION()
	void WorldMapButtonClicked();

	// --- Respawn Button ---
	UButtonWidget* GetRespawnButton() const { return Respawn; }

	// --- WorldMap Button ---
	UButtonWidget* GetWorldMapButton() const { return WorldMap; }

	// --- MainMenu Button ---
	UButtonWidget* GetMainMenuButton() const { return MainMenu; }

private:
	//Native Constructor
	virtual void NativeConstruct() override;

	UPROPERTY(EditAnywhere, meta = (BindWidget = true))
	UButtonWidget* Respawn { nullptr };

	UPROPERTY(EditAnywhere, meta = (BindWidget = true))
	UButtonWidget* WorldMap { nullptr };

	UPROPERTY(EditAnywhere, meta = (BindWidget = true))
	UButtonWidget* MainMenu { nullptr };

	const FName LevelName = FName(TEXT("WorldMap"));

	const FName MainMenuName = FName(TEXT("MainMenu"));
};
