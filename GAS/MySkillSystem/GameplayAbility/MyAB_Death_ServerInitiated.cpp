#include "MyAB_Death_ServerInitiated.h"
#include "../../MyCharacter.h"
#include "Animation/AnimMontage.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "../../MyConfig/MyCharacterConfig.h"
#include "../../MyConfig/MyConfigInstance.h"

UMyAB_Death_ServerInitiated::UMyAB_Death_ServerInitiated()
{
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::ServerInitiated;
	//CurrentTag = FGameplayTag::RequestGameplayTag(TEXT("GAS.Character.Dead"));
}

void UMyAB_Death_ServerInitiated::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
										const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
										const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	AMyCharacter *Target = Cast<AMyCharacter>(ActorInfo->AvatarActor.Get());
	FMyCharacterMontage *CharacterMontage = GetCharacterMontage(ActorInfo->OwnerActor.Get(), Target->SelfType);
	
	if (CharacterMontage->DeadMontage != nullptr)
	{
		UAbilityTask_PlayMontageAndWait* Task = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, CharacterMontage->DeadMontage);
		BindMontageCallbacks(Task);
		Task->ReadyForActivation();
		return;
	}
	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}
