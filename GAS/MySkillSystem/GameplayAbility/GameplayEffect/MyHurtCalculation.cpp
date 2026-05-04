#include "MyHurtCalculation.h"
#include "../../MyAttributeSet.h"

struct FMyHurtStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(Defense);

	FMyHurtStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UMyAttributeSet, Defense, Target, true);
	}
};

static const FMyHurtStatics& HurtStatics()
{
	static FMyHurtStatics Statics;
	return Statics;
}

UMyHurtCalculation::UMyHurtCalculation()
{
	RelevantAttributesToCapture.Add(HurtStatics().DefenseDef);
}

void UMyHurtCalculation::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& Params,
                                                FGameplayEffectCustomExecutionOutput& Out) const
{
	Super::Execute_Implementation(Params, Out);
	
	const FGameplayEffectSpec& Spec = Params.GetOwningSpec();
	
	int Damage = Spec.GetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag("GAS.Skill.Damage"),false, -1);
	if (Damage == -1)
		return;
	
	FAggregatorEvaluateParameters EvalParams;
	float Defense = 0;
	Params.AttemptCalculateCapturedAttributeMagnitude(HurtStatics().DefenseDef, EvalParams,Defense);
	
	int FinalDamage = Damage - Defense;
	FinalDamage = FMath::Max(0, FinalDamage);
	Out.AddOutputModifier(FGameplayModifierEvaluatedData(UMyAttributeSet::GetHpAttribute(), EGameplayModOp::Additive, -FinalDamage));
	
	//UE_LOG(LogTemp, Error, TEXT("Execute_Implementation at %f"),  Params.GetOwningSpec().GetContext().GetInstigator()->GetWorld()->GetTimeSeconds());
}
