// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/Settings/GraphicSettingWidget.h"
#include "Components/Button.h"

void UGraphicSettingWidget::NativeConstruct()
{
	Super::NativeConstruct();

	MainMenu->OnClicked.AddDynamic(this, &UGraphicSettingWidget::ReturnMainMenuClicked);
	MainMenu->OnHovered.AddDynamic(this, &UGraphicSettingWidget::ReturnMainMenuHovered);
	MainMenu->OnUnhovered.AddDynamic(this, &UGraphicSettingWidget::ReturnMainMenuUnhovered);

	WindowModeDown->OnClicked.AddDynamic(this, &UGraphicSettingWidget::WindowModeDownClicked);
	WindowModeDown->OnHovered.AddDynamic(this, &UGraphicSettingWidget::WindowModeDownHovered);
	WindowModeDown->OnUnhovered.AddDynamic(this, &UGraphicSettingWidget::WindowModeDownUnhovered);

	WindowModeUp->OnClicked.AddDynamic(this, &UGraphicSettingWidget::WindowModeUpClicked);
	WindowModeUp->OnHovered.AddDynamic(this, &UGraphicSettingWidget::WindowModeUpHovered);
	WindowModeUp->OnUnhovered.AddDynamic(this, &UGraphicSettingWidget::WindowModeUpUnhovered);

	ResolutionDown->OnClicked.AddDynamic(this, &UGraphicSettingWidget::ResolutionDownClicked);
	ResolutionDown->OnHovered.AddDynamic(this, &UGraphicSettingWidget::ResolutionDownHovered);
	ResolutionDown->OnUnhovered.AddDynamic(this, &UGraphicSettingWidget::ResolutionDownUnhovered);

	ResolutionUp->OnClicked.AddDynamic(this, &UGraphicSettingWidget::ResolutionUpClicked);
	ResolutionUp->OnHovered.AddDynamic(this, &UGraphicSettingWidget::ResolutionUpHovered);
	ResolutionUp->OnUnhovered.AddDynamic(this, &UGraphicSettingWidget::ResolutionUpUnhovered);

	GraphicDown->OnClicked.AddDynamic(this, &UGraphicSettingWidget::GraphicDownClicked);
	GraphicDown->OnHovered.AddDynamic(this, &UGraphicSettingWidget::GraphicDownHovered);
	GraphicDown->OnUnhovered.AddDynamic(this, &UGraphicSettingWidget::GraphicDownUnhovered);

	GraphicUp->OnClicked.AddDynamic(this,&UGraphicSettingWidget::GraphicUpClicked);
	GraphicUp->OnHovered.AddDynamic(this,&UGraphicSettingWidget::GraphicUpHovered);
	GraphicUp->OnUnhovered.AddDynamic(this,&UGraphicSettingWidget::GraphicUpUnhovered);	

	VSyncDown->OnClicked.AddDynamic(this, &UGraphicSettingWidget::VSyncDownClicked);
	VSyncDown->OnHovered.AddDynamic(this, &UGraphicSettingWidget::VSyncDownHovered);
	VSyncDown->OnUnhovered.AddDynamic(this, &UGraphicSettingWidget::VSyncDownUnhovered);

	VSyncUp->OnClicked.AddDynamic(this, &UGraphicSettingWidget::VSyncUpClicked);
	VSyncUp->OnHovered.AddDynamic(this, &UGraphicSettingWidget::VSyncUpHovered);
	VSyncUp->OnUnhovered.AddDynamic(this, &UGraphicSettingWidget::VSyncUpUnhovered);

	Apply->OnClicked.AddDynamic(this, &UGraphicSettingWidget::ApplyClicked);
	Apply->OnHovered.AddDynamic(this, &UGraphicSettingWidget::ApplyHovered);
	Apply->OnUnhovered.AddDynamic(this, &UGraphicSettingWidget::ApplyUnhovered);

}

void UGraphicSettingWidget::ReturnMainMenuClicked()
{
}

void UGraphicSettingWidget::ReturnMainMenuHovered()
{
}

void UGraphicSettingWidget::ReturnMainMenuUnhovered()
{
}

void UGraphicSettingWidget::WindowModeDownClicked()
{
}

void UGraphicSettingWidget::WindowModeDownHovered()
{
}

void UGraphicSettingWidget::WindowModeDownUnhovered()
{
}

void UGraphicSettingWidget::WindowModeUpClicked()
{
}

void UGraphicSettingWidget::WindowModeUpHovered()
{
}

void UGraphicSettingWidget::WindowModeUpUnhovered()
{
}

void UGraphicSettingWidget::ResolutionDownClicked()
{
}

void UGraphicSettingWidget::ResolutionDownHovered()
{
}

void UGraphicSettingWidget::ResolutionDownUnhovered()
{
}

void UGraphicSettingWidget::ResolutionUpClicked()
{
}

void UGraphicSettingWidget::ResolutionUpHovered()
{
}

void UGraphicSettingWidget::ResolutionUpUnhovered()
{
}

void UGraphicSettingWidget::GraphicDownClicked()
{
}

void UGraphicSettingWidget::GraphicDownHovered()
{
}

void UGraphicSettingWidget::GraphicDownUnhovered()
{
}

void UGraphicSettingWidget::GraphicUpClicked()
{
}

void UGraphicSettingWidget::GraphicUpHovered()
{
}

void UGraphicSettingWidget::GraphicUpUnhovered()
{
}

void UGraphicSettingWidget::VSyncDownClicked()
{
}

void UGraphicSettingWidget::VSyncDownHovered()
{
}

void UGraphicSettingWidget::VSyncDownUnhovered()
{
}

void UGraphicSettingWidget::VSyncUpClicked()
{
}

void UGraphicSettingWidget::VSyncUpHovered()
{
}

void UGraphicSettingWidget::VSyncUpUnhovered()
{
}

void UGraphicSettingWidget::ApplyClicked()
{
}

void UGraphicSettingWidget::ApplyHovered()
{
}

void UGraphicSettingWidget::ApplyUnhovered()
{
}


