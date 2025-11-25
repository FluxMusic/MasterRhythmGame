// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LoadMenuWidget.generated.h"

class UButton;
class AMainMenuHUD;
/**
 * 
 */
UCLASS()
class MASTERRHYTHMGAME_API ULoadMenuWidget : public UUserWidget
{
	GENERATED_BODY()
public:

	UFUNCTION()
	void ReturnMainMenuButtonClicked();

	UFUNCTION()
	void LoadFirstSaveButtonClicked();

	UFUNCTION()
	void LoadSecondSaveButtonClicked();

	UFUNCTION()
	void LoadThirdSaveButtonClicked();

	UFUNCTION()
	void ReturnMainMenuButtonHovered();

	UFUNCTION()
	void ReturnMainMenuButtonUnhovered();

	UFUNCTION()
	void LoadFirstSaveButtonHovered();

	UFUNCTION()
	void LoadFirstSaveButtonUnhovered();

	UFUNCTION()
	void LoadSecondSaveButtonHovered();

	UFUNCTION()
	void LoadSecondSaveButtonUnhovered();

	UFUNCTION()
	void LoadThirdSaveButtonHovered();

	UFUNCTION()
	void LoadThirdSaveButtonUnhovered();

	// --- Load First Button ---
	UButton* GetLoadFirstButton() const { return LoadFirstButton; }

	// --- Load Second Button ---
	UButton* GetLoadSecondButton() const { return LoadSecondButton; }

	// --- Load Third Button ---
	UButton* GetLoadThirdButton() const { return LoadThirdButton; }

	// --- Load MainMenu Button ---
	UButton* GetReturnMainMenuButton() const { return ReturnMainMenuButton; }

private:
	//Native Constructor
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	UButton* LoadFirstButton {nullptr}; 

	UPROPERTY(meta = (BindWidget))
	UButton* LoadSecondButton { nullptr };

	UPROPERTY(meta = (BindWidget))
	UButton* LoadThirdButton { nullptr };

	UPROPERTY(meta = (BindWidget))
	UButton* ReturnMainMenuButton { nullptr };

	UPROPERTY(EditAnywhere)
	TObjectPtr<AMainMenuHUD> MainMenuHud { nullptr };
};
