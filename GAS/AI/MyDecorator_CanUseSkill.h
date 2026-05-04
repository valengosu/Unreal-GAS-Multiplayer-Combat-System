#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "MyDecorator_CanUseSkill.generated.h"

UCLASS()
class GAS_API UMyDecorator_CanUseSkill : public UBTDecorator
{
	GENERATED_BODY()
	
public:
	UMyDecorator_CanUseSkill();
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
	
	UPROPERTY(EditAnywhere, Category="Blackboard")
	FBlackboardKeySelector TargetActorKey;
	
	UPROPERTY(EditAnywhere, Category="Blackboard")
	FBlackboardKeySelector SkillRangeKey;
};
