// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GraphicSettingWidget.generated.h"

/**
 * 
 */
UCLASS()
class MASTERRHYTHMGAME_API UGraphicSettingWidget : public UUserWidget
{
	GENERATED_BODY()
public:

	UFUNCTION()
	void ReturnMainMenuClicked();

	UFUNCTION()
	void ReturnMainMenuHovered();

	UFUNCTION()
	void ReturnMainMenuUnhovered();

	UFUNCTION()
	void WindowModeDownClicked();

	UFUNCTION()
	void WindowModeDownHovered();

	UFUNCTION()
	void WindowModeDownUnhovered();

	UFUNCTION()
	void WindowModeUpClicked();

	UFUNCTION()
	void WindowModeUpHovered();

	UFUNCTION()
	void WindowModeUpUnhovered();

	UFUNCTION()
	void ResolutionDownClicked();

	UFUNCTION()
	void ResolutionDownHovered();

	UFUNCTION()
	void ResolutionDownUnhovered();

	UFUNCTION()
	void ResolutionUpClicked();

	UFUNCTION()
	void ResolutionUpHovered();

	UFUNCTION()
	void ResolutionUpUnhovered();

	UFUNCTION()
	void GraphicDownClicked();

	UFUNCTION()
	void GraphicDownHovered();

	UFUNCTION()
	void GraphicDownUnhovered();

	UFUNCTION()
	void GraphicUpClicked();

	UFUNCTION()
	void GraphicUpHovered();

	UFUNCTION()
	void GraphicUpUnhovered();

	UFUNCTION()
	void VSyncDownClicked();

	UFUNCTION()
	void VSyncDownHovered();

	UFUNCTION()
	void VSyncDownUnhovered();

	UFUNCTION()
	void VSyncUpClicked();

	UFUNCTION()
	void VSyncUpHovered();

	UFUNCTION()
	void VSyncUpUnhovered();

	UFUNCTION()
	void ApplyClicked();

	UFUNCTION()
	void ApplyHovered();

	UFUNCTION()
	void ApplyUnhovered();

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* MainMenu { nullptr };

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* WindowModeDown { nullptr };

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* WindowModeUp { nullptr };

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* ResolutionDown { nullptr };

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* ResolutionUp { nullptr };

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* GraphicDown { nullptr };

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* GraphicUp { nullptr };

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* VSyncDown { nullptr };

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* VSyncUp { nullptr };

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* Apply { nullptr };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<EWindowMode::Type> WindowMode = EWindowMode::Fullscreen;

private:
	//Native Constructor
	virtual void NativeConstruct() override;
};
