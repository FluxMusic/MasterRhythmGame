// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BlackscreenWidget.generated.h"

/**
 * 
 */
UCLASS()
class MASTERRHYTHMGAME_API UBlackscreenWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void FadeIn();
	
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void FadeOut();
};
