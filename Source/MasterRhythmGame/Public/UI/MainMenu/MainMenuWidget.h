// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuWidget.generated.h"

class UButton;
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

	UFUNCTION()
	void NewGameHovered();

	UFUNCTION()
	void NewGameUnhovered();

	UFUNCTION()
	void LoadGameHovered();

	UFUNCTION()
	void LoadGameUnhovered();

	UFUNCTION()
	void CreditHovered();

	UFUNCTION()
	void CreditUnhovered();

	UFUNCTION()
	void SettingHovered();

	UFUNCTION()
	void SettingUnhovered();

	UFUNCTION()
	void EscapeHovered();

	UFUNCTION()
	void EscapeUnhovered();

	// --- Credits Button ---
	UButton* GetCreditsButton() const { return Credits; }

	// --- Escape Button ---
	UButton* GetEscapeButton() const { return Escape; }

	// --- LoadGame Button ---
	UButton* GetLoadGameButton() const { return LoadGame; }

	// --- NewGame Button ---
	UButton* GetNewGameButton() const { return NewGame; }

	// --- Settings Button ---
	UButton* GetSettingButton() const { return Setting; }

	// --- Level Name ---
	FName GetLevelName() const { return LevelName; }

private:
	//Native Constructor
	virtual void NativeConstruct() override;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* Credits{ nullptr };

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* Escape{ nullptr };

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* LoadGame{ nullptr };

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* NewGame{ nullptr };

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* Setting{ nullptr };

	//TODO: Change Name later to WorldMap back
	const FName LevelName = FName(TEXT("TestMap"));

	UPROPERTY()
	TObjectPtr<AMainMenuHUD> MainMenuHud { nullptr };
};
