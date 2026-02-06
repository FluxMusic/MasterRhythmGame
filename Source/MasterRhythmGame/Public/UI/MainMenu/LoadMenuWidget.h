// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LoadMenuWidget.generated.h"

class UButtonWidget;
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

	// --- Load First Button ---
	UButtonWidget* GetLoadFirstButton() const { return LoadFirstButton; }

	// --- Load Second Button ---
	UButtonWidget* GetLoadSecondButton() const { return LoadSecondButton; }

	// --- Load Third Button ---
	UButtonWidget* GetLoadThirdButton() const { return LoadThirdButton; }

	// --- Load MainMenu Button ---
	UButtonWidget* GetReturnMainMenuButton() const { return ReturnMainMenuButton; }

	// Update visibility of save slot buttons based on save game existence
	UFUNCTION(BlueprintCallable)
	void UpdateSaveSlotVisibility();

protected:
	virtual void NativeOnInitialized() override;
	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

public:
	virtual void SetVisibility(ESlateVisibility InVisibility) override;

private:
	//Native Constructor
	virtual void NativeConstruct() override;

	// Helper function to load a specific save slot
	void LoadSaveSlot(const FString& SlotName);

	UPROPERTY(meta = (BindWidget))
	UButtonWidget* LoadFirstButton {nullptr}; 

	UPROPERTY(meta = (BindWidget))
	UButtonWidget* LoadSecondButton { nullptr };

	UPROPERTY(meta = (BindWidget))
	UButtonWidget* LoadThirdButton { nullptr };

	UPROPERTY(meta = (BindWidget))
	UButtonWidget* ReturnMainMenuButton { nullptr };

	UPROPERTY(EditAnywhere)
	TObjectPtr<AMainMenuHUD> MainMenuHud { nullptr };
};
