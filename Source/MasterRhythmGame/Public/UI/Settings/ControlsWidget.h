// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ControlsWidget.generated.h"

class UButtonWidget;
class AWorldMapHUD;
class AMainMenuHUD;

/**
 * 
 */
UCLASS()
class MASTERRHYTHMGAME_API	UControlsWidget : public UUserWidget
{
	GENERATED_BODY()
public:

	UFUNCTION()
	void ReturnMenu();

	UButtonWidget* GetMainMenuButton() const { return MainMenu; }

private:
	//Native Constructor
	virtual void NativeConstruct() override;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButtonWidget* MainMenu { nullptr };

	UPROPERTY()
	TObjectPtr<AMainMenuHUD> MainMenuHud { nullptr };

	UPROPERTY()
	TObjectPtr<AWorldMapHUD> WorldMapHud { nullptr };
};
