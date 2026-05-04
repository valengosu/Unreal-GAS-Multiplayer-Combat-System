#include "MyGE_Cooldown.h"

UMyGE_Cooldown::UMyGE_Cooldown()
{
	DurationPolicy = EGameplayEffectDurationType::HasDuration;
	
	FSetByCallerFloat SetByCallerDuration;
	SetByCallerDuration.DataTag = FGameplayTag::RequestGameplayTag(TEXT("GAS.Skill.CooldownTime"));

	DurationMagnitude = FGameplayEffectModifierMagnitude(SetByCallerDuration);
}
