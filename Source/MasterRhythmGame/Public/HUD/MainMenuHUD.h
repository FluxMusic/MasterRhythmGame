// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "UI/MainMenu/MainMenuWidget.h"
#include "UI/MainMenu/CreditsWidget.h"
#include "MainMenuHUD.generated.h"

/**
 * 
 */
UCLASS()
class MASTERRHYTHMGAME_API AMainMenuHUD : public AHUD
{
	GENERATED_BODY()
public:
	AMainMenuHUD();

	UPROPERTY(EditAnywhere)
	TSubclassOf<UMainMenuWidget> MainMenuWidgetClass;

	UPROPERTY()
	TObjectPtr<UMainMenuWidget> MainMenuInstance { nullptr };

	UPROPERTY(EditAnywhere)
	TSubclassOf<UCreditsWidget> CreditsWidgetClass;

	UPROPERTY()
	TObjectPtr<UCreditsWidget> CreditsMenuInstance { nullptr };


protected:
	virtual void BeginPlay() override;
};