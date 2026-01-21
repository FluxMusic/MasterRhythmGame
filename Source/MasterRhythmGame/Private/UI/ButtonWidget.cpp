// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/ButtonWidget.h"
#include "Components/Button.h"

void UButtonWidget::HandleButtonClicked()
{
	OnClicked.Broadcast();
}

void UButtonWidget::HandleButtonHovered()
{
	FButtonStyle NewStyle = OriginalStyle;
	NewStyle.Normal = OriginalStyle.Hovered;
	Button->SetStyle(NewStyle);
	
	OnHovered.Broadcast();
}

void UButtonWidget::HandleButtonUnhovered()
{
	Button->SetStyle(OriginalStyle);
	
	OnUnhovered.Broadcast();
}

void UButtonWidget::NativeConstruct()
{
	Super::NativeConstruct();

	OriginalStyle = Button->GetStyle();

	Button->OnClicked.AddDynamic(this, &UButtonWidget::HandleButtonClicked);
	Button->OnHovered.AddDynamic(this, &UButtonWidget::HandleButtonHovered);
	Button->OnUnhovered.AddDynamic(this, &UButtonWidget::HandleButtonUnhovered);
}