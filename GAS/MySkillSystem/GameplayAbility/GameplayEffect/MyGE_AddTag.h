#pragma once

#include "CoreMinimal.h"
#include "GameplayEffect.h"
#include "MyGE_AddTag.generated.h"

UCLASS()
class GAS_API UMyGE_AddTag : public UGameplayEffect
{
	GENERATED_BODY()
	
public:
	UMyGE_AddTag()
	{
		DurationPolicy = EGameplayEffectDurationType::Infinite;
	}
};
