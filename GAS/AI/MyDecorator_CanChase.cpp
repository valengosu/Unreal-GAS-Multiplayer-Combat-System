#include "MyDecorator_CanChase.h"
#include "GAS/MyMonster.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"

UMyDecorator_CanChase::UMyDecorator_CanChase()
{
	NodeName = TEXT("CanChase");
}

bool UMyDecorator_CanChase::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	AActor* Target = Cast<AActor>(BB->GetValueAsObject(TargetActorKey.SelectedKeyName));
	if (Target == nullptr)
		return false;
	
	AMyMonster *MyMonster = Cast<AMyMonster>(OwnerComp.GetAIOwner()->GetPawn());
	if (MyMonster->CanMove() == false)
		return false;
	
	float Distance = FVector::Dist(Target->GetActorLocation(), MyMonster->GetActorLocation());
	return Distance <= MyMonster->ChaseRange;
}
