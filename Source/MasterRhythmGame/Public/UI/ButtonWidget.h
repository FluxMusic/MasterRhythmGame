// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Styling/SlateTypes.h"
#include "ButtonWidget.generated.h"

class UButton;
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMyButtonClicked);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMyButtonHovered);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMyButtonUnhovered);

/**
 * 
 */
UCLASS()
class MASTERRHYTHMGAME_API UButtonWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION()
	void HandleButtonClicked();

	UFUNCTION()
	void HandleButtonHovered();

	UFUNCTION()
	void HandleButtonUnhovered();

	// --- Button ---
	UButton* GetButton() const { return Button; }

	UPROPERTY(BlueprintAssignable, Category="Button")
	FOnMyButtonClicked OnClicked;

	UPROPERTY(BlueprintAssignable, Category="Button")
	FOnMyButtonHovered OnHovered;

	UPROPERTY(BlueprintAssignable, Category="Button")
	FOnMyButtonUnhovered OnUnhovered;

private:
	virtual void NativeConstruct() override;

	UPROPERTY(meta = (BindWidget))
	UButton* Button { nullptr };

	UPROPERTY(EditDefaultsOnly, Category = "Button")
	USoundBase* ClickSound { nullptr };

	// Store the original style to restore it
	FButtonStyle OriginalStyle;
};
