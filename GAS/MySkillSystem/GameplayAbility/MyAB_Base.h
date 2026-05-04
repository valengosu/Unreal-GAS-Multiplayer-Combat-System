#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include "GameplayTagContainer.h"
#include "Abilities/GameplayAbility.h"
#include "MyAB_Base.generated.h"

class UAbilityTask_PlayMontageAndWait;

UCLASS()
class GAS_API UMyAB_Base : public UGameplayAbility
{
	GENERATED_BODY()
	
protected:
	FGameplayTag GetSkillTag(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpecHandle Handle) const;
	
	void BindMontageCallbacks(UAbilityTask_PlayMontageAndWait* Task);
	
	
	virtual void ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
											const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData) override;

	virtual void EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
										const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled) override;
	
	bool CheckSkillCooldown(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpecHandle Handle) const;
	void ApplayCooldown(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpecHandle Handle);
	
	virtual bool CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, 
	const FGameplayTagContainer* SourceTags = nullptr, const FGameplayTagContainer* TargetTags = nullptr, FGameplayTagContainer* OptionalRelevantTags = nullptr) const override;
	
	virtual bool CommitAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, 
		const FGameplayAbilityActivationInfo ActivationInfo, FGameplayTagContainer* OptionalRelevantTags = nullptr) override;
	
	UFUNCTION()
	void OnMontageEnded_Completed();
	
	UFUNCTION()
	void OnMontageEnded_Interrupted();

	UFUNCTION()
	void OnMontageEnded_Cancelled();

	UFUNCTION()
	void OnMontageEnded_BlendOut();
	
	UFUNCTION()
	virtual void HandleMontageCompleted(){};
	
	UFUNCTION()
	virtual void HandleMontageInterrupted(){};
	
	UFUNCTION()
	virtual void HandleMontageCancelled(){};
	
	UFUNCTION()
	virtual void HandleMontageBlendOut(){};

protected:
	FGameplayTag CurrentTag = FGameplayTag::EmptyTag;
	FActiveGameplayEffectHandle CastingGEHandle;
	
public:
	UFUNCTION(Client, Reliable)
	void Client_ConfirmPrediction(const FPredictionKey& PredictionKey);
	static TMap<FPredictionKey, TWeakObjectPtr<AActor>> PredictionActorMap;
};
