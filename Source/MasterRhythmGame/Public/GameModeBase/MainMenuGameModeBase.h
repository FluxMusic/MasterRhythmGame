// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "HUD/MainMenuHUD.h"
#include "MainMenuGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class MASTERRHYTHMGAME_API AMainMenuGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:
	AMainMenuGameModeBase();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HUD")
	TSubclassOf<AMainMenuHUD> MainMenuHudClass;

protected:
	virtual void BeginPlay() override;

};