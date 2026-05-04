// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "MyDecorator_CheckTag.generated.h"

/**
 * 
 */
UCLASS()
class GAS_API UMyDecorator_CheckTag : public UBTDecorator
{
	GENERATED_BODY()
	
public:
	UMyDecorator_CheckTag();
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
	
	UPROPERTY(EditAnywhere, Category = "GAS")
	FGameplayTag TagToCheck;
	
	UPROPERTY(EditAnywhere, Category="Blackboard")
	FBlackboardKeySelector TargetActorKey;
};
