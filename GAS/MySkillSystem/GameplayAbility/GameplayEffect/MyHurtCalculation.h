// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "MyHurtCalculation.generated.h"

/**
 * 
 */
UCLASS()
class GAS_API UMyHurtCalculation : public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()
	
public:
	UMyHurtCalculation();
	virtual void Execute_Implementation(const FGameplayEffectCustomExecutionParameters& Params, FGameplayEffectCustomExecutionOutput& Out) const override;
};
