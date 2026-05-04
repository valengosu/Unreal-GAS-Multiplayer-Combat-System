// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "MyTask_UseSkill.generated.h"

/**
 * 
 */
UCLASS()
class GAS_API UMyTask_UseSkill : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UMyTask_UseSkill();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
	UPROPERTY(EditAnywhere, Category="Blackboard")
	FBlackboardKeySelector SkillTagKey;
};
