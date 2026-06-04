// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MyHUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL_PRACTICE_API UMyHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

    UPROPERTY(meta = (BindWidget))
    class UProgressBar* HealthBar;

    UPROPERTY(meta = (BindWidget))
    class UTextBlock* HealthText;

    UFUNCTION(BlueprintCallable)
    void UpdateHealth(float CurrentHealth, float MaxHealth, float DamageTaken);
};

