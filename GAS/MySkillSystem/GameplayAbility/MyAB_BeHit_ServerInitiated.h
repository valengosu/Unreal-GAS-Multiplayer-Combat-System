#pragma once

#include "CoreMinimal.h"
#include "MyAB_Base.h"
#include "Abilities/GameplayAbility.h"
#include "MyAB_BeHit_ServerInitiated.generated.h"

UCLASS()
class GAS_API UMyAB_BeHit_ServerInitiated : public UMyAB_Base
{
	GENERATED_BODY()
	
protected:
	UMyAB_BeHit_ServerInitiated();
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, 
								const FGameplayAbilityActorInfo* ActorInfo, 
								const FGameplayAbilityActivationInfo ActivationInfo,
								const FGameplayEventData* TriggerEventData) override;
};
