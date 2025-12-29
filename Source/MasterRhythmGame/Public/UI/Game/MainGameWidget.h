// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
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
	void SetHealthEnemy(int32 InHealth);

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
	
	// --- LifeBarEnemySix ---
	UProgressBar* GetLifeBarEnemy6() const { return LifeBarEnemySix; }
	
	// --- LifeBarEnemySeven ---
	UProgressBar* GetLifeBarEnemy7() const { return LifeBarEnemySeven; }
	
	// --- LifeBarEnemyEight ---
	UProgressBar* GetLifeBarEnemy8() const { return LifeBarEnemyEight; }
	
	// --- CompletionPercent ---
	UTextBlock* GetCompletionPercent() const { return CompletionPercent; }

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
	class UProgressBar* LifeBarEnemySix { nullptr };
	
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UProgressBar* LifeBarEnemySeven { nullptr };
	
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UProgressBar* LifeBarEnemyEight { nullptr };

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* CompletionPercent { nullptr };

	//TODO: Kombotext

	UPROPERTY()
	TObjectPtr<ATestEnemyActor> Enemy { nullptr };

};
