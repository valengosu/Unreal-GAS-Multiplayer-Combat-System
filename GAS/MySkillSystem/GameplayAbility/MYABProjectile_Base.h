// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GAS/MySkillSystem/GameplayAbility/MyAB_Base.h"
#include "../HitDetection/MyProjectileActor.h"
#include "MYABProjectile_Base.generated.h"

UCLASS()
class GAS_API UMYABProjectile_Base : public UMyAB_Base
{
	GENERATED_BODY()
	
protected:
	UMYABProjectile_Base();
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, 
								const FGameplayAbilityActorInfo* ActorInfo, 
								const FGameplayAbilityActivationInfo ActivationInfo,
								const FGameplayEventData* TriggerEventData) override;

private:
	AMyProjectileActor* CreateProjectile(AActor* Owner, FGameplayTag SkillTag, TSubclassOf<AMyProjectileActor> Projectile, const FPredictionKey &key);
};
