// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/MySkillSystem/GameplayAbility/MyAB_Base.h"
#include "MyAB_Death_ServerInitiated.generated.h"

UCLASS()
class GAS_API UMyAB_Death_ServerInitiated : public UMyAB_Base
{
	GENERATED_BODY()
	
protected:
	UMyAB_Death_ServerInitiated();
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, 
							const FGameplayAbilityActorInfo* ActorInfo, 
							const FGameplayAbilityActivationInfo ActivationInfo,
							const FGameplayEventData* TriggerEventData) override;
};
