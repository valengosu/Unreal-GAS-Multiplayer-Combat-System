// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "MyAB_Base.h"
#include "MyABMelee_Base.generated.h"

/**
 * 
 */
UCLASS()
class GAS_API UMyABMelee_Base : public UMyAB_Base
{
	GENERATED_BODY()
	
protected:
	UMyABMelee_Base();
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, 
								const FGameplayAbilityActorInfo* ActorInfo, 
								const FGameplayAbilityActivationInfo ActivationInfo,
								const FGameplayEventData* TriggerEventData) override;
	
	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
										const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	
private:
	void CreateHitBoxActor(AActor* Owner, FGameplayTag SkillTag);
	void DestoryHitBoxActor(AActor* Owner);
};
