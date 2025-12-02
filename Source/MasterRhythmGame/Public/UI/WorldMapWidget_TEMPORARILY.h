// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WorldMapWidget_TEMPORARILY.generated.h"

class UButton;
class AWorldMapHUD;
/**
 * 
 */
UCLASS()
class MASTERRHYTHMGAME_API UWorldMapWidget_TEMPORARILY : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION()
	void LevelOneButtonClicked();

	UFUNCTION()
	void LevelOneButtonHovered();

	UFUNCTION()
	void LevelOneButtonUnhovered();

	UFUNCTION()
	void LevelTwoButtonClicked();

	UFUNCTION()
	void LevelTwoButtonHovered();

	UFUNCTION()
	void LevelTwoButtonUnhovered();

	UFUNCTION()
	void LevelThreeButtonClicked();

	UFUNCTION()
	void LevelThreeButtonHovered();

	UFUNCTION()
	void LevelThreeButtonUnhovered();

	UFUNCTION()
	void LevelFourButtonClicked();

	UFUNCTION()
	void LevelFourButtonHovered();

	UFUNCTION()
	void LevelFourButtonUnhovered();

	UFUNCTION()
	void LevelFiveButtonClicked();

	UFUNCTION()
	void LevelFiveButtonHovered();

	UFUNCTION()
	void LevelFiveButtonUnhovered();

	UFUNCTION()
	void LevelSixButtonClicked();

	UFUNCTION()
	void LevelSixButtonHovered();

	UFUNCTION()
	void LevelSixButtonUnhovered();

	UFUNCTION()
	void MainMenuButtonClicked();

	UFUNCTION()
	void MainMenuButtonHovered();

	UFUNCTION()
	void MainMenuButtonUnhovered();


	// --- Level One Button ---
	UButton* GetLevelOneButton() const { return LevelOneButton; }

	// --- Level Two Button ---
	UButton* GetLevelTwoButton() const { return LevelTwoButton; }

	// --- Level Three Button ---
	UButton* GetLevelThreeButton() const { return LevelThreeButton; }

	// --- Level Four Button ---
	UButton* GetLevelFourButton() const { return LevelFourButton; }

	// --- Level Five Button ---
	UButton* GetLevelFiveButton() const { return LevelFiveButton; }

	// --- Level Six Button ---
	UButton* GetLevelSixButton() const { return LevelSixButton; }

	// --- Main Menu Button ---
	UButton* GetMainMenuButton() const { return MainMenuButton; }

private:
	//Native Constructor
	virtual void NativeConstruct() override;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* LevelOneButton { nullptr };

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* LevelTwoButton { nullptr };

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* LevelThreeButton { nullptr };

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* LevelFourButton { nullptr };

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* LevelFiveButton { nullptr };

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* LevelSixButton { nullptr };

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* MainMenuButton { nullptr };
	
	UPROPERTY()
	TObjectPtr<AWorldMapHUD> WorldMapHud { nullptr };

	const FName LevelName = FName(TEXT("MainMenu"));
	const FName LevelOneName = FName(TEXT("TestMap"));

};
