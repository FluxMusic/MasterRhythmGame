// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuSetting.generated.h"

class AMainMenuHUD;
/**
 * 
 */
UCLASS()
class MASTERRHYTHMGAME_API UMainMenuSetting : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION()
	void GraphicSettingClicked();

	UFUNCTION()
	void AudioSettingClicked();

	UFUNCTION()
	void ReturnMainMenuClicked();

	UFUNCTION()
	void GraphicButtonHovered();

	UFUNCTION()
	void GraphicButtonUnhovered();

	UFUNCTION()
	void AudioButtonHovered();

	UFUNCTION()
	void AudioButtonUnhovered();

	UFUNCTION()
	void ReturnMainMenuButtonHovered();

	UFUNCTION()
	void ReturnMainMenuButtonUnhovered();

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* Graphic { nullptr };

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* Audio { nullptr };

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* MainMenu { nullptr };

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UGraphicSettingWidget* WBP_GraphicSettingWidget { nullptr };

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UAudioSettingWidget* WBP_AudioSettingWidget { nullptr };

private:
	//Native Constructor
	virtual void NativeConstruct() override;

	UPROPERTY()
	TObjectPtr<AMainMenuHUD> MainMenuHud { nullptr };
};
