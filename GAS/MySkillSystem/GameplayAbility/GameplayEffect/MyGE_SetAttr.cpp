#include "MyGE_SetAttr.h"
#include "../../MyAttributeSet.h"
#include "MySetAttributeNumeric.h"

UMyGE_SetAttr::UMyGE_SetAttr()
{
	DurationPolicy = EGameplayEffectDurationType::Instant;
	
	FGameplayEffectExecutionDefinition ExecDef;
	ExecDef.CalculationClass = UMySetAttributeNumeric::StaticClass(); 
	Executions.Add(ExecDef);
}
