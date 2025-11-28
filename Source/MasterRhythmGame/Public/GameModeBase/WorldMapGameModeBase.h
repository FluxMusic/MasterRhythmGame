// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "HUD/WorldMapHUD.h"
#include "WorldMapGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class MASTERRHYTHMGAME_API AWorldMapGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:
	AWorldMapGameModeBase();

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "HUD")
	TSubclassOf<AWorldMapHUD> WorldMapHudClass;

protected:
	virtual void BeginPlay() override;
};
