// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "UI/PauseMenuWidget.h"
#include "WorldMapHUD.generated.h"

/**
 * 
 */
UCLASS()
class AWorldMapHUD : public AHUD
{
	GENERATED_BODY()
public:
	AWorldMapHUD();

	// --- Pause Menu Widget Class ---
	TSubclassOf<UPauseMenuWidget> GetPauseMenuWidgetClass() const { return PauseMenuWidgetClass; }
	void SetPauseMenuWidgetClass(TSubclassOf<UPauseMenuWidget> InClass) { PauseMenuWidgetClass = InClass; }

	// --- Pause Menu Instance ---
	UPauseMenuWidget* GetPauseMenuInstance() const { return PauseMenuInstance; }
	void SetPauseMenuInstance(UPauseMenuWidget* InInstance) { PauseMenuInstance = InInstance; }

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<UPauseMenuWidget> PauseMenuWidgetClass;

	UPROPERTY()
	TObjectPtr<UPauseMenuWidget> PauseMenuInstance { nullptr };
};
