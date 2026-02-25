// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/TextBlock.h"
#include "UserScoreWidget.generated.h"

/**
 * Widget that displays the score for a specific level
 * Rotates continuously and faces the camera
 */
UCLASS()
class MASTERRHYTHMGAME_API UUserScoreWidget : public UUserWidget
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (BindWidget), meta = (AllowPrivateAccess = true))
	UTextBlock* HighScore { nullptr };
};
