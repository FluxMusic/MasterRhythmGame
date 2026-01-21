// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "UI/Game/KeyboardWidget.h"
#include "MainGameWidget.generated.h"

class AGameCharacter;
class ATestEnemyActor;
class UProgressBar;
class UTextBlock;
/**
 * 
 */
UCLASS()
class MASTERRHYTHMGAME_API UMainGameWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION()
	void SetHealthPlayer(int32 InHealth);
	UFUNCTION()
	void SetMaxHealthPlayer(int32 InMaxHealth);

	UFUNCTION()
	void SetHealthEnemy1(int32 InHealth);
	UFUNCTION()
	void SetHealthEnemy2(int32 InHealth);
	UFUNCTION()
	void SetHealthEnemy3(int32 InHealth);
	UFUNCTION()
	void SetHealthEnemy4(int32 InHealth);
	UFUNCTION()
	void SetHealthEnemy5(int32 InHealth);
	UFUNCTION()

	void SetMaxHealthEnemy1(int32 InMaxHealth);
	UFUNCTION()
	void SetMaxHealthEnemy2(int32 InMaxHealth);
	UFUNCTION()
	void SetMaxHealthEnemy3(int32 InMaxHealth);
	UFUNCTION()
	void SetMaxHealthEnemy4(int32 InMaxHealth);
	UFUNCTION()
	void SetMaxHealthEnemy5(int32 InMaxHealth);

	// --- LifeBarPlayer ---
	UProgressBar* GetLifeBarPlayer() const { return LifeBarPlayer; }

	// --- LifeBarEnemyOne ---
	UProgressBar* GetLifeBarEnemy1() const { return LifeBarEnemyOne; }

	// --- LifeBarEnemyTwo ---
	UProgressBar* GetLifeBarEnemy2() const { return LifeBarEnemyTwo; }

	// --- LifeBarEnemyThree ---
	UProgressBar* GetLifeBarEnemy3() const { return LifeBarEnemyThree; }
	
	// --- LifeBarEnemyFour ---
	UProgressBar* GetLifeBarEnemy4() const { return LifeBarEnemyFour; }
	
	// --- LifeBarEnemyFive ---
	UProgressBar* GetLifeBarEnemy5() const { return LifeBarEnemyFive; }
	
	// --- CompletionPercent ---
	UTextBlock* GetCompletionPercent() const { return CompletionPercent; }

	UKeyboardWidget* GetKeyboardWidget() const { return KeyboardWidget; }

private:
	//Native Constructor
	virtual void NativeConstruct() override;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UProgressBar* LifeBarPlayer { nullptr };

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UProgressBar* LifeBarEnemyOne { nullptr };
	
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UProgressBar* LifeBarEnemyTwo { nullptr };
	
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UProgressBar* LifeBarEnemyThree { nullptr };
	
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UProgressBar* LifeBarEnemyFour { nullptr };
	
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UProgressBar* LifeBarEnemyFive { nullptr };

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* CompletionPercent { nullptr };

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UKeyboardWidget* KeyboardWidget { nullptr };

	//TODO: Kombotext

	UPROPERTY()
	TObjectPtr<ATestEnemyActor> Enemy { nullptr };

	UPROPERTY()
	int32 MaxPlayerHealth { 0 };

	UPROPERTY()
	int32 MaxEnemyHealth1 { 0 };
	UPROPERTY()
	int32 MaxEnemyHealth2 { 0 };
	UPROPERTY()
	int32 MaxEnemyHealth3 { 0 };
	UPROPERTY()
	int32 MaxEnemyHealth4 { 0 };
	UPROPERTY()
	int32 MaxEnemyHealth5 { 0 };
};
