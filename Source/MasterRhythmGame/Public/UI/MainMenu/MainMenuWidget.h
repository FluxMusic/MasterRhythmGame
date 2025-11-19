// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class MASTERRHYTHMGAME_API UMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION()
	void NewGameHovered();

	UFUNCTION()
	void NewGameUnhovered();

	UFUNCTION()
	void LoadGameHovered();

	UFUNCTION()
	void LoadGameUnhovered();

	UFUNCTION()
	void CreditHovered();

	UFUNCTION()
	void CreditUnhovered();

	UFUNCTION()
	void SettingHovered();

	UFUNCTION()
	void SettingUnhovered();

	UFUNCTION()
	void EscapeHovered();

	UFUNCTION()
	void EscapeUnhovered();

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* Credits{ nullptr };

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* Escape { nullptr };

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* LoadGame { nullptr };
	
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* NewGame { nullptr };
	
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* Setting { nullptr };

private:
	//Native Constructor
	virtual void NativeConstruct() override;

	UFUNCTION()
	void NewGameButtonClicked();

	UFUNCTION()
	void LoadGameButtonClicked();

	UFUNCTION()
	void CreditsButtonClicked();

	UFUNCTION()
	void SettingButtonClicked();

	UFUNCTION()
	void EscapeButtonClicked();

	const FName LevelName = FName(TEXT(""));
};
