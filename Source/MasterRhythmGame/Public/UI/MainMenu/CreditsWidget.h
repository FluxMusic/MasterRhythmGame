// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CreditsWidget.generated.h"

class AMainMenuHUD;
/**
 * 
 */
UCLASS()
class MASTERRHYTHMGAME_API UCreditsWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UFUNCTION()
	void ReturnMainMenuButtonClicked();

	UButton* GetReturnMainMenuButton() const { return ReturnMainMenu; }

private:
	//Native Constructor
	virtual void NativeConstruct() override;

	UFUNCTION()
	void MainMenuOnHovered();

	UFUNCTION()
	void MainMenuOnUnhovered();

	UPROPERTY(meta = (BindWidget))
	class UButton* ReturnMainMenu{ nullptr };

	UPROPERTY()
	TObjectPtr<AMainMenuHUD> MainMenuHud { nullptr };

};
