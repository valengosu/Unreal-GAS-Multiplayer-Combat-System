#include "MyAB_Base.h"
#include "../../MyConfig/MyConfigInstance.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "GameplayEffect/MyGE_Cooldown.h"
#include "GAS/MyCharacter.h"
#include "GAS/MyConfig/MySkillConfig.h"


TMap<FPredictionKey, TWeakObjectPtr<AActor>> UMyAB_Base::PredictionActorMap;

FGameplayTag UMyAB_Base::GetSkillTag(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpecHandle Handle) const
{
	UAbilitySystemComponent* ASC = ActorInfo->AbilitySystemComponent.Get();
	FGameplayAbilitySpec* Spec = ASC->FindAbilitySpecFromHandle(Handle);
	return Spec->GetDynamicSpecSourceTags().IsEmpty() ? FGameplayTag() : Spec->GetDynamicSpecSourceTags().First();
}

void UMyAB_Base::OnMontageEnded_Completed()
{
	UE_LOG(LogTemp, Log, TEXT("GoodBye1"));
	HandleMontageCompleted();
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
}

void UMyAB_Base::OnMontageEnded_Interrupted()
{
	UE_LOG(LogTemp, Log, TEXT("GoodBye2"));
	HandleMontageInterrupted();
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
}

void UMyAB_Base::OnMontageEnded_Cancelled()
{
	UE_LOG(LogTemp, Log, TEXT("GoodBye3"));
	HandleMontageCancelled();
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
}

void UMyAB_Base::OnMontageEnded_BlendOut()
{
	UE_LOG(LogTemp, Log, TEXT("GoodBye4"));
	HandleMontageBlendOut();
	EndAbility(CurrentSpecHandle, CurrentActorInfo, CurrentActivationInfo, false, false);
}

void UMyAB_Base::Client_ConfirmPrediction_Implementation(const FPredictionKey& PredictionKey)
{
	PredictionActorMap.Remove(PredictionKey);
}

void UMyAB_Base::BindMontageCallbacks(UAbilityTask_PlayMontageAndWait* Task)
{
	Task->OnCompleted.AddDynamic(this, &UMyAB_Base::OnMontageEnded_Completed);
	Task->OnInterrupted.AddDynamic(this, &UMyAB_Base::OnMontageEnded_Interrupted);
	Task->OnCancelled.AddDynamic(this, &UMyAB_Base::OnMontageEnded_Cancelled);
	Task->OnBlendOut.AddDynamic(this, &UMyAB_Base::OnMontageEnded_BlendOut);
}

void UMyAB_Base::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	UAbilitySystemComponent *ASC = ActorInfo->AbilitySystemComponent.Get();
	ASC->AddLooseGameplayTag(CurrentTag, 1, EGameplayTagReplicationState::TagAndCountToAll);
}

void UMyAB_Base::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	UAbilitySystemComponent *ASC = ActorInfo->AbilitySystemComponent.Get();
	ASC->RemoveLooseGameplayTag(CurrentTag, 1, EGameplayTagReplicationState::TagAndCountToAll);
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UMyAB_Base::ApplayCooldown(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpecHandle Handle)
{
	FGameplayTag SkillTag = GetSkillTag(ActorInfo, Handle);
	FSkillConfigItem *SkillConfigItem = GetSkillConfigItem(ActorInfo->OwnerActor.Get(), SkillTag);
	if (SkillConfigItem == nullptr)
		return;
	
	float time = SkillConfigItem->Cooldown;
	UAbilitySystemComponent *ASC = ActorInfo->AbilitySystemComponent.Get();
	
	FGameplayEffectSpecHandle SpecHandle = ASC->MakeOutgoingSpec(UMyGE_Cooldown::StaticClass(), 1.f, ASC->MakeEffectContext());
	SpecHandle.Data->DynamicGrantedTags.AddTag(SkillTag);
	SpecHandle.Data->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(TEXT("GAS.Skill.CooldownTime")), time);
	ASC->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
}

bool UMyAB_Base::CheckSkillCooldown(const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilitySpecHandle Handle) const
{
	FGameplayTag SkillTag = GetSkillTag(ActorInfo, Handle);
	return !ActorInfo->AbilitySystemComponent.Get()->HasMatchingGameplayTag(SkillTag);
}

bool UMyAB_Base::CanActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
									const FGameplayTagContainer* SourceTags, const FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags) const
{
	if (Super::CanActivateAbility(Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags) == false)
		return false;
	
	if (CheckSkillCooldown(ActorInfo, Handle) == false)
		return false;
	
	return true;
}

bool UMyAB_Base::CommitAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
							   const FGameplayAbilityActivationInfo ActivationInfo, FGameplayTagContainer* OptionalRelevantTags)
{
	if (Super::CommitAbility(Handle, ActorInfo, ActivationInfo, OptionalRelevantTags) == false)
		return false;
	
	ApplayCooldown(ActorInfo, Handle);
	return true;
}
