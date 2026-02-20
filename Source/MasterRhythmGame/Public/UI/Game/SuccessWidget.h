// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SuccessWidget.generated.h"

class UButtonWidget;
/**
 * 
 */
UCLASS()
class MASTERRHYTHMGAME_API USuccessWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION()
	void RetryButtonClicked();

	UFUNCTION()
	void MainMenuButtonClicked();

	UFUNCTION()
	void WorldMapButtonClicked();

	// --- Retry Button ---
	UButtonWidget* GetRetryButton() const { return Retry; }

	// --- WorldMap Button ---
	UButtonWidget* GetWorldMapButton() const { return WorldMap; }

	// --- MainMenu Button ---
	UButtonWidget* GetMainMenuButton() const { return MainMenu; }

private:
	//Native Constructor
	virtual void NativeConstruct() override;

	UPROPERTY(EditAnywhere, meta = (BindWidget = true))
	UButtonWidget* Retry { nullptr };

	UPROPERTY(EditAnywhere, meta = (BindWidget = true))
	UButtonWidget* WorldMap { nullptr };

	UPROPERTY(EditAnywhere, meta = (BindWidget = true))
	UButtonWidget* MainMenu { nullptr };

	const FName LevelName = FName(TEXT("MAP_WorldMap"));

	const FName MainMenuName = FName(TEXT("MAP_MainMenu"));
};
