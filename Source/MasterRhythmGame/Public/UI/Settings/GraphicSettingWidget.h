// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GraphicSettingWidget.generated.h"

class UButton;
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
	void ReturnSettingMenuHovered();

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
	UButton* GetMainMenuButton() const { return MainMenu; }

	// --- Window Mode Down Button ---
	UButton* GetWindowModeDownButton() const { return WindowModeDown; }

	// --- Window Mode Up Button ---
	UButton* GetWindowModeUpButton() const { return WindowModeUp; }

	// --- Resolution Down Button ---
	UButton* GetResolutionDownButton() const { return ResolutionDown; }

	// --- Resolution Up Button ---
	UButton* GetResolutionUpButton() const { return ResolutionUp; }

	// --- Graphics Down Button ---
	UButton* GetGraphicDownButton() const { return GraphicDown; }

	// --- Graphics Up Button ---
	UButton* GetGraphicUpButton() const { return GraphicUp; }

	// --- VSync Down Button ---
	UButton* GetVSyncDownButton() const { return VSyncDown; }

	// --- VSync Up Button ---
	UButton* GetVSyncUpButton() const { return VSyncUp; }

	// --- Apply Button ---
	UButton* GetApplyButton() const { return Apply; }

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
	class UButton* MainMenu{ nullptr };

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* WindowModeDown{ nullptr };

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* WindowModeUp{ nullptr };

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* ResolutionDown{ nullptr };

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* ResolutionUp{ nullptr };

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* GraphicDown{ nullptr };

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* GraphicUp{ nullptr };

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* VSyncDown{ nullptr };

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* VSyncUp{ nullptr };

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* Apply{ nullptr };

	UPROPERTY()
	TObjectPtr<AMainMenuHUD> MainMenuHud { nullptr };
};
