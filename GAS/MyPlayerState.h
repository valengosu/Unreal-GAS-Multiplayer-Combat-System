// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "MyPlayerState.generated.h"

class UAbilitySystemComponent;
class UMyAttributeSet;
/**
 * 
 */
UCLASS()
class GAS_API AMyPlayerState : public APlayerState
{
	GENERATED_BODY()
	
public:
	AMyPlayerState();
	
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent; 
	
	UPROPERTY()
	TObjectPtr<UMyAttributeSet> MyAttributeSet;
};
