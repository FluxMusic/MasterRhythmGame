// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuWidget.generated.h"

class UButtonWidget;
class AMainMenuHUD;
/**
 * 
 */
UCLASS()
class MASTERRHYTHMGAME_API UMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION()
	void NewGameButtonClicked();

	UFUNCTION()
	void LoadGameButtonClicked();

	UFUNCTION()
	void CreditsButtonClicked();

	UFUNCTION()
	void SettingButtonClicked();

	UFUNCTION()
	void EscapeButtonClicked();

	// --- Credits Button ---
	UButtonWidget* GetCreditsButton() const { return Credits; }

	// --- Escape Button ---
	UButtonWidget* GetEscapeButton() const { return Escape; }

	// --- LoadGame Button ---
	UButtonWidget* GetLoadGameButton() const { return LoadGame; }

	// --- NewGame Button ---
	UButtonWidget* GetNewGameButton() const { return NewGame; }

	// --- Settings Button ---
	UButtonWidget* GetSettingButton() const { return Setting; }

	// --- Level Name ---
	FName GetLevelName() const { return LevelName; }

private:
	//Native Constructor
	virtual void NativeConstruct() override;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButtonWidget* Credits { nullptr };

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButtonWidget* Escape { nullptr };

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButtonWidget* LoadGame { nullptr };

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButtonWidget* NewGame { nullptr };

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButtonWidget* Setting { nullptr };

	const FName LevelName = FName(TEXT("WorldMap"));

	UPROPERTY()
	TObjectPtr<AMainMenuHUD> MainMenuHud { nullptr };
};
