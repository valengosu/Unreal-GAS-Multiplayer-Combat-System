#include "MyTask_PrepareSkill.h"
#include "AIController.h"
#include "GAS/MyCharacter.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GAS/MyConfig/MyConfigInstance.h"
#include "GAS/MyConfig/MySkillConfig.h"

UMyTask_PrepareSkill::UMyTask_PrepareSkill()
{
	NodeName = TEXT("PrepareSkill");
}

EBTNodeResult::Type UMyTask_PrepareSkill::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AI = OwnerComp.GetAIOwner();
	AMyCharacter *MyCharacter = Cast<AMyCharacter>(AI->GetPawn());
	UAbilitySystemComponent *ASC = MyCharacter->GetAbilitySystemComponent();
	
	FScopedAbilityListLock AbilityListLock(*ASC);
	for (const FGameplayAbilitySpec& Spec : ASC->GetActivatableAbilities())
	{
		if (Spec.GetDynamicSpecSourceTags().IsEmpty() == false)
		{
			FGameplayTag SkillTag = Spec.GetDynamicSpecSourceTags().First();
			FSkillConfigItem *Item = GetSkillConfigItem(MyCharacter, SkillTag);
			OwnerComp.GetBlackboardComponent()->SetValueAsFloat(SkillRangeKey.SelectedKeyName, Item->SkillRange);
			OwnerComp.GetBlackboardComponent()->SetValueAsName(SkillTagKey.SelectedKeyName, SkillTag.GetTagName());
			break;
		}
	}
	
	return Super::ExecuteTask(OwnerComp, NodeMemory);
}
