// Fill out your copyright notice in the Description page of Project Settings.


#include "MyHUDWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

void UMyHUDWidget::UpdateHealth(float CurrentHealth, float MaxHealth, float DamageTaken)
{
    if (HealthBar)
    {
        float Ratio = CurrentHealth / MaxHealth;
        HealthBar->SetPercent(Ratio);

        // HP 퍼센트에 따라 색 변경 (초록 → 노랑 → 빨강)
        FLinearColor BarColor = FLinearColor::LerpUsingHSV(
            FLinearColor::Red, FLinearColor::Green, Ratio);
        HealthBar->SetFillColorAndOpacity(BarColor);
    }

    if (HealthText)
    {
        FString HPString = FString::Printf(TEXT("HP: %.0f / %.0f"), CurrentHealth, MaxHealth);
        HealthText->SetText(FText::FromString(HPString));
    }
}