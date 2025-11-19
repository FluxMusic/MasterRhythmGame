// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "UI/MainMenu/MainMenuWidget.h"
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


protected:
	virtual void BeginPlay() override;

private:
	const FString ObjectPath = FString(TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/Blueprints/UI/MainMenu/WBP_MainMenu.WBP_MainMenu_C'"));
};
