// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../../CoreTypes.h"
#include "Blueprint/UserWidget.h"
#include "UI/Game/KeyboardWidget.h"
#include "MainGameWidget.generated.h"

class AGameCharacter;
class ATestEnemyActor;
class UProgressBar;
class UTextBlock;
class UImage;
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

	UFUNCTION()
	void SetCompletionPercent(float InPercent);

	UFUNCTION()
	void UpdateScore(int32 TotalScore, float ComboMultiplier);

	UFUNCTION()
	void UpdateNoteAmount(int32 InNoteAmount);

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

	// --- CompletionBar ---
	UProgressBar* GetCompletionBar() const { return CompletionBar; }

	UImage* GetInstrumentsImage() const { return Instruments; }
	void SetInstrumentsImage(EInstrument Instrument);

	UKeyboardWidget* GetKeyboardWidget() const { return KeyboardWidget; }

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<class UTexture2D> PianoTexture 	  { nullptr };
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<class UTexture2D> ViolinTexture 	  { nullptr };
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<class UTexture2D> SaxophoneTexture { nullptr };
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<class UTexture2D> GuitarTexture    { nullptr };
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TObjectPtr<class UTexture2D> SynthTexture     { nullptr };


private:
	//Native Constructor
	virtual void NativeConstruct() override;

private:

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
	UImage* Instruments { nullptr };

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	UKeyboardWidget* KeyboardWidget { nullptr };

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UProgressBar* CompletionBar { nullptr };

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* ScoreText { nullptr };

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* ComboText { nullptr };

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* NoteAmount { nullptr };

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
