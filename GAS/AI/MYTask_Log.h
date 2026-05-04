// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "MYTask_Log.generated.h"

/**
 * 
 */
UCLASS()
class GAS_API UMYTask_Log : public UBTTaskNode
{
	GENERATED_BODY()
public:
	UMYTask_Log();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
