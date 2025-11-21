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
};
