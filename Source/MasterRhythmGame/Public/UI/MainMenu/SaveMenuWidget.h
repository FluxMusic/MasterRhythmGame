// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SaveMenuWidget.generated.h"

class UButtonWidget;
class AMainMenuHUD;
class AGameHUD;
class AWorldMapHUD;
/**
 * 
 */
UCLASS()
class MASTERRHYTHMGAME_API USaveMenuWidget : public UUserWidget
{
	GENERATED_BODY()
public:

	UFUNCTION()
	void ReturnMenuButtonClicked();

	UFUNCTION()
	void SaveFirstSlotButtonClicked();

	UFUNCTION()
	void SaveSecondSlotButtonClicked();

	UFUNCTION()
	void SaveThirdSlotButtonClicked();

	// --- Save First Button ---
	UButtonWidget* GetSaveFirstButton() const { return SaveFirstButton; }

	// --- Save Second Button ---
	UButtonWidget* GetSaveSecondButton() const { return SaveSecondButton; }

	// --- Save Third Button ---
	UButtonWidget* GetSaveThirdButton() const { return SaveThirdButton; }

	// --- Return Menu Button ---
	UButtonWidget* GetReturnMenuButton() const { return ReturnMenuButton; }

private:
	//Native Constructor
	virtual void NativeConstruct() override;

	// Helper function to save to a specific save slot
	void SaveToSlot(const FString& SlotName);

	UPROPERTY(meta = (BindWidget))
	UButtonWidget* SaveFirstButton {nullptr}; 

	UPROPERTY(meta = (BindWidget))
	UButtonWidget* SaveSecondButton { nullptr };

	UPROPERTY(meta = (BindWidget))
	UButtonWidget* SaveThirdButton { nullptr };

	UPROPERTY(meta = (BindWidget))
	UButtonWidget* ReturnMenuButton { nullptr };

	UPROPERTY(EditAnywhere)
	TObjectPtr<AMainMenuHUD> MainMenuHud { nullptr };

	UPROPERTY()
	TObjectPtr<AGameHUD> GameHUD { nullptr };

	UPROPERTY()
	TObjectPtr<AWorldMapHUD> WorldMapHud { nullptr };
};
