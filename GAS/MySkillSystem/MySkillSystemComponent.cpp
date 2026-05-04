#include "MySkillSystemComponent.h"
#include "GameplayAbilitySpec.h"
#include "Net/UnrealNetwork.h"
#include "GAS/MyCharacter.h"

UMySkillSystemComponent::UMySkillSystemComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	SetIsReplicatedByDefault(true);
}

void UMySkillSystemComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UMySkillSystemComponent, CurrentSkillId);
}

bool UMySkillSystemComponent::CanCastSkill()
{
	FGameplayTagContainer Tags;
	Tags.AddTag(FGameplayTag::RequestGameplayTag(TEXT("GAS.Character.Dead")));
	Tags.AddTag(FGameplayTag::RequestGameplayTag(TEXT("GAS.Skill.Hit")));
	Tags.AddTag(FGameplayTag::RequestGameplayTag(TEXT("GAS.Skill.Casting")));
	
	UAbilitySystemComponent *ASC = Cast<AMyCharacter>(GetOwner())->GetAbilitySystemComponent();
	bool bAny = ASC->HasAnyMatchingGameplayTags(Tags);
	return !bAny;
}

void UMySkillSystemComponent::TryCastSkill(FGameplayTag SkillTag)
{
	if (CanCastSkill() == false)
		return;
	
	UAbilitySystemComponent *ASC = Cast<AMyCharacter>(GetOwner())->GetAbilitySystemComponent();
	const FGameplayAbilitySpec *Spec = GetGameplayAbilitySpec(ASC, SkillTag);
	check(Spec);
	
	ASC->TryActivateAbility(Spec->Handle);
}

const FGameplayAbilitySpec* UMySkillSystemComponent::GetGameplayAbilitySpec(UAbilitySystemComponent *ASC, FGameplayTag SkillTag)
{
	for (const FGameplayAbilitySpec& Spec : ASC->GetActivatableAbilities())
	{
		if (Spec.GetDynamicSpecSourceTags().HasTagExact(SkillTag) == true)
		{
			return &Spec;
		}
	}
	return nullptr;
}
