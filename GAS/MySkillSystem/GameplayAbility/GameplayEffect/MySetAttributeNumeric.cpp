// Fill out your copyright notice in the Description page of Project Settings.


#include "MySetAttributeNumeric.h"
#include "../../MyAttributeSet.h"

void UMySetAttributeNumeric::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& Params, FGameplayEffectCustomExecutionOutput& Out) const
{
	Super::Execute_Implementation(Params, Out);
	
	const FGameplayEffectSpec& Spec = Params.GetOwningSpec();
	
	int index = Spec.GetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag("Attribute.Index"),false, (float)EAttributeType::Count);
	EAttributeType AttrType = static_cast<EAttributeType>(index);
	float Value = Spec.GetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag("Attribute.Value"),false, -1);
	
	switch (AttrType)
	{
	case EAttributeType::Hp:
		Out.AddOutputModifier(FGameplayModifierEvaluatedData(UMyAttributeSet::GetHpAttribute(), EGameplayModOp::Override, Value));
		break;
		
	case EAttributeType::MaxHp:
		Out.AddOutputModifier(FGameplayModifierEvaluatedData(UMyAttributeSet::GetMaxHpAttribute(), EGameplayModOp::Override, Value));
		break;
		
	case EAttributeType::Mp:
		Out.AddOutputModifier(FGameplayModifierEvaluatedData(UMyAttributeSet::GetMpAttribute(), EGameplayModOp::Override, Value));
		break;
		
	case EAttributeType::MaxMp:
		Out.AddOutputModifier(FGameplayModifierEvaluatedData(UMyAttributeSet::GetMaxMpAttribute(), EGameplayModOp::Override, Value));
		break;
		
	case EAttributeType::Attack:
		Out.AddOutputModifier(FGameplayModifierEvaluatedData(UMyAttributeSet::GetAttackAttribute(), EGameplayModOp::Override, Value));
		break;
		
	case EAttributeType::Defense:
		Out.AddOutputModifier(FGameplayModifierEvaluatedData(UMyAttributeSet::GetDefenseAttribute(), EGameplayModOp::Override, Value));
		break;
		
	default:
		break;
	}
}
