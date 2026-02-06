// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/Game/MainGameWidget.h"
#include "Components/Image.h"
#include "Components/ProgressBar.h"
#include "Components/Widget.h"
#include "Math/UnrealMathUtility.h"
#include "Components/TextBlock.h"

void UMainGameWidget::SetMaxHealthPlayer(int32 InMaxHealth)
{
	// Ensure max health is at least 1 to avoid division by zero for player
	MaxPlayerHealth = FMath::Max(1, InMaxHealth);

	if (LifeBarPlayer != nullptr)
	{
		LifeBarPlayer->SetVisibility(ESlateVisibility::Visible);
		// Set the progress bar to 100%
		LifeBarPlayer->SetPercent(1.0f);
	}

	// After setting max health, update the displayed current health.
	// Assume that when max health is set, current health equals max (full health).
	SetHealthPlayer(MaxPlayerHealth);
}

void UMainGameWidget::SetHealthPlayer(int32 InHealth)
{
	if (LifeBarPlayer != nullptr)
	{
		const float Percent = (MaxPlayerHealth > 0)
			? static_cast<float>(InHealth) / static_cast<float>(MaxPlayerHealth)
			: 0.0f;

		LifeBarPlayer->SetPercent(FMath::Clamp(Percent, 0.0f, 1.0f));
	}
}

/* --- Enemy 1 --- */
void UMainGameWidget::SetMaxHealthEnemy1(int32 InMaxHealth)
{
	// allow 0 to indicate "no enemy / hidden"
	MaxEnemyHealth1 = InMaxHealth;

	if (LifeBarEnemyOne == nullptr)
	{
		return;
	}

	if (MaxEnemyHealth1 <= 0)
	{
		LifeBarEnemyOne->SetVisibility(ESlateVisibility::Hidden);
		return;
	}

	LifeBarEnemyOne->SetVisibility(ESlateVisibility::Visible);
	LifeBarEnemyOne->SetPercent(1.0f);
	SetHealthEnemy1(MaxEnemyHealth1);
}

void UMainGameWidget::SetHealthEnemy1(int32 InHealth)
{
	if (LifeBarEnemyOne == nullptr)
	{
		return;
	}

	if (MaxEnemyHealth1 <= 0)
	{
		LifeBarEnemyOne->SetVisibility(ESlateVisibility::Hidden);
		return;
	}

	LifeBarEnemyOne->SetVisibility(ESlateVisibility::Visible);
	const float Percent = static_cast<float>(InHealth) / static_cast<float>(MaxEnemyHealth1);
	LifeBarEnemyOne->SetPercent(FMath::Clamp(Percent, 0.0f, 1.0f));
}

/* --- Enemy 2 --- */
void UMainGameWidget::SetMaxHealthEnemy2(int32 InMaxHealth)
{
	MaxEnemyHealth2 = InMaxHealth;

	if (LifeBarEnemyTwo == nullptr)
	{
		return;
	}

	if (MaxEnemyHealth2 <= 0)
	{
		LifeBarEnemyTwo->SetVisibility(ESlateVisibility::Hidden);
		return;
	}

	LifeBarEnemyTwo->SetVisibility(ESlateVisibility::Visible);
	LifeBarEnemyTwo->SetPercent(1.0f);
	SetHealthEnemy2(MaxEnemyHealth2);
}

void UMainGameWidget::SetHealthEnemy2(int32 InHealth)
{
	if (LifeBarEnemyTwo == nullptr)
	{
		return;
	}

	if (MaxEnemyHealth2 <= 0)
	{
		LifeBarEnemyTwo->SetVisibility(ESlateVisibility::Hidden);
		return;
	}

	LifeBarEnemyTwo->SetVisibility(ESlateVisibility::Visible);
	const float Percent = static_cast<float>(InHealth) / static_cast<float>(MaxEnemyHealth2);
	LifeBarEnemyTwo->SetPercent(FMath::Clamp(Percent, 0.0f, 1.0f));
}

/* --- Enemy 3 --- */
void UMainGameWidget::SetMaxHealthEnemy3(int32 InMaxHealth)
{
	MaxEnemyHealth3 = InMaxHealth;

	if (LifeBarEnemyThree == nullptr)
	{
		return;
	}

	if (MaxEnemyHealth3 <= 0)
	{
		LifeBarEnemyThree->SetVisibility(ESlateVisibility::Hidden);
		return;
	}

	LifeBarEnemyThree->SetVisibility(ESlateVisibility::Visible);
	LifeBarEnemyThree->SetPercent(1.0f);
	SetHealthEnemy3(MaxEnemyHealth3);
}

void UMainGameWidget::SetHealthEnemy3(int32 InHealth)
{
	if (LifeBarEnemyThree == nullptr)
	{
		return;
	}

	if (MaxEnemyHealth3 <= 0)
	{
		LifeBarEnemyThree->SetVisibility(ESlateVisibility::Hidden);
		return;
	}

	LifeBarEnemyThree->SetVisibility(ESlateVisibility::Visible);
	const float Percent = static_cast<float>(InHealth) / static_cast<float>(MaxEnemyHealth3);
	LifeBarEnemyThree->SetPercent(FMath::Clamp(Percent, 0.0f, 1.0f));
}

/* --- Enemy 4 --- */
void UMainGameWidget::SetMaxHealthEnemy4(int32 InMaxHealth)
{
	MaxEnemyHealth4 = InMaxHealth;

	if (LifeBarEnemyFour == nullptr)
	{
		return;
	}

	if (MaxEnemyHealth4 <= 0)
	{
		LifeBarEnemyFour->SetVisibility(ESlateVisibility::Hidden);
		return;
	}

	LifeBarEnemyFour->SetVisibility(ESlateVisibility::Visible);
	LifeBarEnemyFour->SetPercent(1.0f);
	SetHealthEnemy4(MaxEnemyHealth4);
}

void UMainGameWidget::SetHealthEnemy4(int32 InHealth)
{
	if (LifeBarEnemyFour == nullptr)
	{
		return;
	}

	if (MaxEnemyHealth4 <= 0)
	{
		LifeBarEnemyFour->SetVisibility(ESlateVisibility::Hidden);
		return;
	}

	LifeBarEnemyFour->SetVisibility(ESlateVisibility::Visible);
	const float Percent = static_cast<float>(InHealth) / static_cast<float>(MaxEnemyHealth4);
	LifeBarEnemyFour->SetPercent(FMath::Clamp(Percent, 0.0f, 1.0f));
}

/* --- Enemy 5 --- */
void UMainGameWidget::SetMaxHealthEnemy5(int32 InMaxHealth)
{
	MaxEnemyHealth5 = InMaxHealth;

	if (LifeBarEnemyFive == nullptr)
	{
		return;
	}

	if (MaxEnemyHealth5 <= 0)
	{
		LifeBarEnemyFive->SetVisibility(ESlateVisibility::Hidden);
		return;
	}

	LifeBarEnemyFive->SetVisibility(ESlateVisibility::Visible);
	LifeBarEnemyFive->SetPercent(1.0f);
	SetHealthEnemy5(MaxEnemyHealth5);
}

void UMainGameWidget::SetHealthEnemy5(int32 InHealth)
{
	if (LifeBarEnemyFive == nullptr)
	{
		return;
	}

	if (MaxEnemyHealth5 <= 0)
	{
		LifeBarEnemyFive->SetVisibility(ESlateVisibility::Hidden);
		return;
	}

	LifeBarEnemyFive->SetVisibility(ESlateVisibility::Visible);
	const float Percent = static_cast<float>(InHealth) / static_cast<float>(MaxEnemyHealth5);
	LifeBarEnemyFive->SetPercent(FMath::Clamp(Percent, 0.0f, 1.0f));
}

void UMainGameWidget::SetInstrumentsImage(EInstrument Instrument)
{
	if (Instruments)
	{
		switch (Instrument)
		{
		case EInstrument::Piano:
			{
				if (PianoTexture)
				{
					Instruments->SetBrushFromTexture(PianoTexture);
				}
				break;
			}
		case EInstrument::Violin:
			{
				if (ViolinTexture)
				{
					Instruments->SetBrushFromTexture(ViolinTexture);
				}
				break;
			}
		case EInstrument::Saxophone:
			{
				if (SaxophoneTexture)
				{
					Instruments->SetBrushFromTexture(SaxophoneTexture);
				}
				break;
			}
		case EInstrument::Guitar:
			{
				if (GuitarTexture)
				{
					Instruments->SetBrushFromTexture(GuitarTexture);
				}
				break;
			}
		case EInstrument::Synth:
			{
				if (SynthTexture)
				{
					Instruments->SetBrushFromTexture(SynthTexture);
				}
				break;
			}
		
		default:
			{
				UE_LOG(LogTemp, Error, TEXT("UMainGameWidget::SetInstrumentsImage: Multiple or no Instruments selected!"));
				UE_LOG(LogTemp, Error, TEXT("Piano: %i, Violin: %i, Saxophone: %i, Guitar: %i, Synth: %i"), 
					InstrumentFlags::HasFlag(Instrument, EInstrument::Piano),
					InstrumentFlags::HasFlag(Instrument, EInstrument::Violin),
					InstrumentFlags::HasFlag(Instrument, EInstrument::Saxophone),
					InstrumentFlags::HasFlag(Instrument, EInstrument::Guitar),
					InstrumentFlags::HasFlag(Instrument, EInstrument::Synth));
				break;
			}
		}
	}
}

void UMainGameWidget::SetCompletionPercent(float InPercent)
{
	if (CompletionBar != nullptr)
	{
		CompletionBar->SetPercent(FMath::Clamp(InPercent, 0.0f, 1.0f));
	}
}

void UMainGameWidget::UpdateScore(int32 TotalScore, float ComboMultiplier, int32 ComboCount)
{
	if (ScoreText != nullptr)
	{
		ScoreText->SetText(FText::AsNumber(TotalScore));
	}

	if (ComboText != nullptr)
	{
		FString ComboString = FString::Printf(TEXT("x%.1f (%d)"), ComboMultiplier, ComboCount);
		ComboText->SetText(FText::FromString(ComboString));
	}
}

void UMainGameWidget::NativeConstruct()
{
	Super::NativeConstruct();
	
	// Initialize score display
	if (ScoreText != nullptr)
	{
		ScoreText->SetText(FText::FromString(TEXT("0")));
	}
	
	if (ComboText != nullptr)
	{
		ComboText->SetText(FText::FromString(TEXT("x0.1 (0)")));
	}
}