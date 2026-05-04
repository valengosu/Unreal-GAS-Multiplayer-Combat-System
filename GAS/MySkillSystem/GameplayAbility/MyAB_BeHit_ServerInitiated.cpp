#include "MyAB_BeHit_ServerInitiated.h"
#include "GameplayEffect/MyGE_Hurt.h"
#include "AbilitySystemComponent.h"
#include "../../MyConfig/MyConfigInstance.h"
#include "../../MyConfig/MyCharacterConfig.h"
#include "Animation/AnimMontage.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "Abilities/GameplayAbility.h"
#include "../../MyCharacter.h"
#include "GAS/MyConfig/MySkillConfig.h"

UMyAB_BeHit_ServerInitiated::UMyAB_BeHit_ServerInitiated()
{
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::ServerInitiated;
	ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag(TEXT("GAS.Character.Dead")));
	CurrentTag = FGameplayTag::RequestGameplayTag(TEXT("GAS.Skill.Hit"));
	
	FAbilityTriggerData Trigger; 
	Trigger.TriggerSource = EGameplayAbilityTriggerSource::GameplayEvent; 
	Trigger.TriggerTag = FGameplayTag::RequestGameplayTag(TEXT("GAS.Skill.Hit"));
	AbilityTriggers.Add(Trigger);
}

void UMyAB_BeHit_ServerInitiated::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                  const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                  const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	AMyCharacter *Instigator = Cast<AMyCharacter>(const_cast<AActor*>(TriggerEventData->Instigator.Get()));
	int Damage = Instigator->GetMyAttributeSet()->GetAttack();
	
	UAbilitySystemComponent *ASC = ActorInfo->AbilitySystemComponent.Get();
	FGameplayEffectSpecHandle Spec = ASC->MakeOutgoingSpec(UMyGE_Hurt::StaticClass(), 1.f, ASC->MakeEffectContext());
	Spec.Data->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(TEXT("GAS.Skill.Damage")), Damage);
	ASC->ApplyGameplayEffectSpecToSelf(*Spec.Data.Get());
	
	FGameplayTag SkillTag = TriggerEventData->InstigatorTags.First();
	FSkillConfigItem *Item = GetSkillConfigItem(ActorInfo->AvatarActor.Get(), SkillTag);
	 
	AMyCharacter *Target = Cast<AMyCharacter>(ActorInfo->AvatarActor);
	FMyCharacterMontage *CharacterMontage = GetCharacterMontage(ActorInfo->OwnerActor.Get(), Target->SelfType);
	
	if (Item == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("SkillId failed: %s reason"), *SkillTag.GetTagName().ToString());
		return;
	}
	
	if(Item->InterruptLevel > 0 && CharacterMontage->HitMontage != nullptr)
	{
		UAbilityTask_PlayMontageAndWait* Task = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, CharacterMontage->HitMontage);
		BindMontageCallbacks(Task);
		Task->ReadyForActivation();
		return;
	}
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}
