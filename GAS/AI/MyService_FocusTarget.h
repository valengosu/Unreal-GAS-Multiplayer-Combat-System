// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "MyService_FocusTarget.generated.h"

UCLASS()
class GAS_API UMyService_FocusTarget : public UBTService_BlackboardBase
{
	GENERATED_BODY()
public:
	UMyService_FocusTarget();
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
