#include "MyDecorator_CheckTag.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GAS/MyCharacter.h"

UMyDecorator_CheckTag::UMyDecorator_CheckTag()
{
	NodeName = TEXT("CheckTag");
}

bool UMyDecorator_CheckTag::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	if (Super::CalculateRawConditionValue(OwnerComp, NodeMemory) == false)
		return false;
	
	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	AMyCharacter* MyCharacter = Cast<AMyCharacter>(BB->GetValueAsObject(TargetActorKey.SelectedKeyName));
	if (MyCharacter == nullptr)
		return false;
	
	UAbilitySystemComponent *ASC = MyCharacter->GetAbilitySystemComponent();
	if (ASC == nullptr)
		return false;
	
	bool bHasTag = ASC->HasMatchingGameplayTag(TagToCheck);
	return bHasTag;
}
