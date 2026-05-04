#include "MyAttributeSet.h"
#include "GameplayEffectExtension.h"
#include "GAS/MyCharacter.h"
#include "../MySkillSystem/GameplayAbility/MyABMelee_Base.h"
#include "AbilitySystemComponent.h"
#include "GameplayAbility/MyAB_Base.h"
#include "GameplayAbility/MyAB_Death_ServerInitiated.h"
#include "GAS/MyPlayer.h"
#include "Net/UnrealNetwork.h"

void UMyAttributeSet::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, Hp, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, MaxHp, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, Mp, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, MaxMp, COND_None, REPNOTIFY_Always);
	
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, Attack, COND_OwnerOnly, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(ThisClass, Defense, COND_OwnerOnly, REPNOTIFY_Always);
}

void UMyAttributeSet::PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);
	UAbilitySystemComponent *ASC = &Data.Target;
	
	if (Data.EvaluatedData.Attribute == GetHpAttribute() && GetMaxHp() > 0)
	{
		int NewHp = GetHp();
		int ClampedHp = FMath::Clamp(NewHp, 0, GetMaxHp());
		SetHp(ClampedHp);
		
		// Ensure updated attributes replicate to client immediately (avoid HP UI delay)
		ASC->ForceReplication();
		
		if (ClampedHp == 0)
		{
			TWeakObjectPtr<AMyCharacter> MyCharacter = Cast<AMyCharacter>(Data.Target.GetAvatarActor());
			
			if (ASC->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(TEXT("GAS.Character.Dead"))) == false)
			{
				ASC->AddLooseGameplayTag(FGameplayTag::RequestGameplayTag(TEXT("GAS.Character.Dead")), 1, EGameplayTagReplicationState::TagAndCountToAll);
				
				// NOTE:
				// one frame delay is for letting the Ability causing Dead finish its call stack, then call CancelAllAbilities is SAFE! 
				MyCharacter->GetWorld()->GetTimerManager().SetTimerForNextTick([ASC, MyCharacter]()
				{
					if (MyCharacter.IsValid() == false)
						return;
					
					// NOTE:
					// Death is a terminal state.
					// It is allowed to interrupt montage !AND! EndAbility aggressively.
					// Do NOT copy this pattern to normal abilities.
					ASC->CancelAllAbilities();
					ASC->TryActivateAbilityByClass(UMyAB_Death_ServerInitiated::StaticClass());
					
					TWeakObjectPtr MyPlayer = Cast<AMyPlayer>(MyCharacter);
					if (MyPlayer.IsValid() == true)
					{
						FTimerHandle DummyHandle;
						MyPlayer->GetWorld()->GetTimerManager().SetTimer
						(
							DummyHandle,
							[MyPlayer]()
							{
								if (MyPlayer.IsValid() == true)
									MyPlayer->Respawn();
							},
							8.0f,false
						);
					}
				});
			}
		}
	}
	
	if (Data.EvaluatedData.Attribute == GetMpAttribute() && GetMaxMp() > 0)
	{
		int NewMp = GetMp();
		int ClampedMp = FMath::Clamp(NewMp, 0, GetMaxMp());
		SetMp(ClampedMp);
		
		// Ensure updated attributes replicate to client immediately (avoid HP UI delay)
		ASC->ForceReplication();
	}
}

void UMyAttributeSet::OnRep_Hp(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, Hp, OldValue);
}

void UMyAttributeSet::OnRep_MaxHp(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, MaxHp, OldValue);
}

void UMyAttributeSet::OnRep_Mp(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, Mp, OldValue);
}

void UMyAttributeSet::OnRep_MaxMp(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, MaxMp, OldValue);
}

void UMyAttributeSet::OnRep_Attack(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, Attack, OldValue);
}

void UMyAttributeSet::OnRep_Defense(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(ThisClass, Defense, OldValue);
}
