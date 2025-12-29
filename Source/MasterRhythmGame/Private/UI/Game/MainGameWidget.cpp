// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Game/MainGameWidget.h"
#include "Components/ProgressBar.h"

void UMainGameWidget::SetHealthPlayer(int32 InHealth)
{
	if (GetLifeBarPlayer() != nullptr)
	{
		GetLifeBarPlayer()->SetPercent((static_cast<float>(InHealth) / 100.0f));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Penis"));
	}
}

void UMainGameWidget::SetHealthEnemy(int32 InHealth)
{
}

void UMainGameWidget::NativeConstruct()
{
	Super::NativeConstruct();
}
