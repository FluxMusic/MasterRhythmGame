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
	UPROPERTY(meta = (BindWidget))
	class UButton* ReturnMainMenu { nullptr };

	UFUNCTION()
	void ReturnMainMenuButtonClicked();

private:
	//Native Constructor
	virtual void NativeConstruct() override;

	UFUNCTION()
	void MainMenuOnHovered();

	UFUNCTION()
	void MainMenuOnUnhovered();

	UPROPERTY()
	TObjectPtr<AMainMenuHUD> MainMenuHud { nullptr };

};
