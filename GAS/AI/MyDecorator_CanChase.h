#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "MyDecorator_CanChase.generated.h"

UCLASS()
class GAS_API UMyDecorator_CanChase : public UBTDecorator
{
	GENERATED_BODY()
public:
	UMyDecorator_CanChase();
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
	
	UPROPERTY(EditAnywhere, Category="Blackboard")
	FBlackboardKeySelector TargetActorKey;
};
