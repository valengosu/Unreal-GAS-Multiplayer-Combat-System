#include "MyDecorator_CanUseSkill.h"
#include "GAS/MyMonster.h"
#include "AIController.h"
#include "Components/CapsuleComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

UMyDecorator_CanUseSkill::UMyDecorator_CanUseSkill()
{
	NodeName = TEXT("CanUseSkill");
}

bool UMyDecorator_CanUseSkill::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	AActor* Target = Cast<AActor>(BB->GetValueAsObject(TargetActorKey.SelectedKeyName));
	if (Target == nullptr)
		return false;
	
	AMyMonster *MyMonster = Cast<AMyMonster>(OwnerComp.GetAIOwner()->GetPawn());
	float Distance = FVector::Dist(Target->GetActorLocation(), MyMonster->GetActorLocation());
	
	float SelfRadius = 0.f;
	if (UCapsuleComponent* SelfCapsule = MyMonster->GetCapsuleComponent())
	{
		SelfRadius = SelfCapsule->GetScaledCapsuleRadius();
	}
	
	float TargetRadius = 0.f;
	if (UCapsuleComponent* TargetCapsule = Target->FindComponentByClass<UCapsuleComponent>())
	{
		TargetRadius = TargetCapsule->GetScaledCapsuleRadius();
	}
	
	float SkillRange = BB->GetValueAsFloat(SkillRangeKey.SelectedKeyName);
	return Distance <= SkillRange + SelfRadius + TargetRadius + 1;
}
