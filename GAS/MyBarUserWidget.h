// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectTypes.h"
#include "Blueprint/UserWidget.h"
#include "Components/ProgressBar.h"
#include "MyBarUserWidget.generated.h"

UCLASS(BlueprintType, Blueprintable)
class GAS_API UMyBarUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void RefreshBarPercent();
	
	void OnHpChanged(const FOnAttributeChangeData& Data);
	void OnMpChanged(const FOnAttributeChangeData& Data);
	void OnMaxHpChanged(const FOnAttributeChangeData& Data);
	void OnMaxMpChanged(const FOnAttributeChangeData& Data);
	
	int Hp;
	int MaxHp;
	int Mp;
	int MaxMp;
	
private:
	UPROPERTY(meta = (BindWidget))
	UProgressBar* HpBar;
	
	UPROPERTY(meta = (BindWidget))
	UProgressBar* MpBar;
};
