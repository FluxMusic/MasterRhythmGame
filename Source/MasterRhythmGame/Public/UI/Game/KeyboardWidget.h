// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "KeyboardWidget.generated.h"

/**
 * 
 */
UCLASS()
class MASTERRHYTHMGAME_API UKeyboardWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintNativeEvent)
	void SetScaleHighlights(const TArray<int32>& ScaleDegrees);
	void SetScaleHighlights_Implementation(TArray<int32> ScaleDegrees) {}
};
