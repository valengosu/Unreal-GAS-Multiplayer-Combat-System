#include "MYTask_Log.h"

UMYTask_Log::UMYTask_Log()
{
	NodeName = TEXT("Log");
}

EBTNodeResult::Type UMYTask_Log::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	//UE_LOG(LogTemp, Warning, TEXT("BTTask_Log triggered"));
	return Super::ExecuteTask(OwnerComp, NodeMemory);
}
