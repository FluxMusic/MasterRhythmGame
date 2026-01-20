// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GraphicSettingWidget.generated.h"

class AWorldMapHUD;
class UButtonWidget;
class AMainMenuHUD;
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
	void WindowModeDownClicked();

	UFUNCTION()
	void WindowModeUpClicked();

	UFUNCTION()
	void ResolutionDownClicked();

	UFUNCTION()
	void ResolutionUpClicked();

	UFUNCTION()
	void GraphicDownClicked();

	UFUNCTION()
	void GraphicUpClicked();

	UFUNCTION()
	void VSyncDownClicked();

	UFUNCTION()
	void VSyncUpClicked();

	UFUNCTION()
	void ApplyClicked();

	// --- Window Mode ---
	TEnumAsByte<EWindowMode::Type> GetWindowMode() const { return WindowMode; }
	void SetWindowMode(TEnumAsByte<EWindowMode::Type> InMode) { WindowMode = InMode; }

	// --- Resolution Index ---
	int32 GetResolutionIndex() const { return ResolutionIndex; }
	void SetResolutionIndex(int32 InIndex) { ResolutionIndex = InIndex; }

	// --- Resolution ---
	FIntPoint GetResolution() const { return Resolution; }
	void SetResolution(const FIntPoint& InResolution) { Resolution = InResolution; }

	// --- Graphics Index ---
	int32 GetGraphicsIndex() const { return GraphicsIndex; }
	void SetGraphicsIndex(int32 InGraphicsIndex) { GraphicsIndex = InGraphicsIndex; }

	// --- VSync ---
	bool GetVSync() const { return bVSync; }
	void SetVSync(bool bInVSync) { bVSync = bInVSync; }

	// --- Main Menu Button ---
	UButtonWidget* GetMainMenuButton() const { return MainMenu; }

	// --- Window Mode Down Button ---
	UButtonWidget* GetWindowModeDownButton() const { return WindowModeDown; }

	// --- Window Mode Up Button ---
	UButtonWidget* GetWindowModeUpButton() const { return WindowModeUp; }

	// --- Resolution Down Button ---
	UButtonWidget* GetResolutionDownButton() const { return ResolutionDown; }

	// --- Resolution Up Button ---
	UButtonWidget* GetResolutionUpButton() const { return ResolutionUp; }

	// --- Graphics Down Button ---
	UButtonWidget* GetGraphicDownButton() const { return GraphicDown; }

	// --- Graphics Up Button ---
	UButtonWidget* GetGraphicUpButton() const { return GraphicUp; }

	// --- VSync Down Button ---
	UButtonWidget* GetVSyncDownButton() const { return VSyncDown; }

	// --- VSync Up Button ---
	UButtonWidget* GetVSyncUpButton() const { return VSyncUp; }

	// --- Apply Button ---
	UButtonWidget* GetApplyButton() const { return Apply; }

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TEnumAsByte<EWindowMode::Type> WindowMode = EWindowMode::Fullscreen;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ResolutionIndex{ 0 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FIntPoint Resolution{ 1920, 1080 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 GraphicsIndex{ 0 };

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bVSync{ true };

private:
	//Native Constructor
	virtual void NativeConstruct() override;

	void SwitchResolution(int32 InResolutionIndex);

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButtonWidget* MainMenu{ nullptr };

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButtonWidget* WindowModeDown{ nullptr };

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButtonWidget* WindowModeUp{ nullptr };

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButtonWidget* ResolutionDown{ nullptr };

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButtonWidget* ResolutionUp{ nullptr };

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButtonWidget* GraphicDown{ nullptr };

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButtonWidget* GraphicUp{ nullptr };

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButtonWidget* VSyncDown{ nullptr };

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButtonWidget* VSyncUp{ nullptr };

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UButtonWidget* Apply{ nullptr };

	UPROPERTY()
	TObjectPtr<AMainMenuHUD> MainMenuHud { nullptr };

	UPROPERTY()
	TObjectPtr<AWorldMapHUD> WorldMapHud { nullptr };
};
