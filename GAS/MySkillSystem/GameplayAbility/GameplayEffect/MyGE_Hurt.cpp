#include "MyGE_Hurt.h"
#include "MyHurtCalculation.h"

UMyGE_Hurt::UMyGE_Hurt()
{
	DurationPolicy = EGameplayEffectDurationType::Instant;
	
	FGameplayEffectExecutionDefinition ExecDef;
	ExecDef.CalculationClass = UMyHurtCalculation::StaticClass();
	Executions.Add(ExecDef);
}
