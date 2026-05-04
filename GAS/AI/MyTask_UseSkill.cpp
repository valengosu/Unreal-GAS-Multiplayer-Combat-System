#include "MyTask_UseSkill.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "../MySkillSystem/MySkillSystemComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GAS/MyMonster.h"

UMyTask_UseSkill::UMyTask_UseSkill()
{
	NodeName = TEXT("UseSkill");
}

EBTNodeResult::Type UMyTask_UseSkill::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AI = OwnerComp.GetAIOwner();
	AMyMonster* MyMonster = Cast<AMyMonster>(AI->GetPawn());
	
	FName TagName = OwnerComp.GetBlackboardComponent()->GetValueAsName(SkillTagKey.SelectedKeyName);
	FGameplayTag SkillTag = FGameplayTag::RequestGameplayTag(TagName);
	MyMonster->MySkillSystemComponent->TryCastSkill(SkillTag);
	
	return Super::ExecuteTask(OwnerComp, NodeMemory);
}
