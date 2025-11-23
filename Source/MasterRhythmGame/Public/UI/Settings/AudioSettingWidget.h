// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "AudioSettingWidget.generated.h"

class AMainMenuHUD;
/**
 * 
 */
UCLASS()
class MASTERRHYTHMGAME_API UAudioSettingWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION()
	void OnValueChangedMaster(float Value);

	UFUNCTION()
	void OnValueChangedMusic(float Value);

	UFUNCTION()
	void OnValueChangedSfx(float Value);

	UFUNCTION()
	void ReturnMenu();

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class USlider* MasterVolumeSlider { nullptr };

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class USlider* MusicVolumeSlider { nullptr };

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class USlider* SfxVolumeSlider { nullptr };

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* MainMenu { nullptr };

	UPROPERTY(EditAnywhere)
	class USoundSubmix* MasterSubmix { nullptr };

	UPROPERTY(EditAnywhere)
	class USoundSubmix* MusicSubmix { nullptr };

	UPROPERTY(EditAnywhere)
	class USoundSubmix* SfxSubmix { nullptr };

private:
	//Native Constructor
	virtual void NativeConstruct() override;

	UPROPERTY()
	TObjectPtr<AMainMenuHUD> MainMenuHud { nullptr };
};
