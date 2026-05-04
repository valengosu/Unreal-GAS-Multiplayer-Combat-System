#include "MYTask_Respawn.h"
#include "AIController.h"
#include "GAS/MyMonster.h"
#include "GAS/MyPlayer.h"
#include "GAS/MySkillSystem/GameplayAbility/MyAB_Death_ServerInitiated.h"

UMYTask_Respawn::UMYTask_Respawn()
{
	NodeName = TEXT("Respawn");
}

EBTNodeResult::Type UMYTask_Respawn::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* AI = OwnerComp.GetAIOwner();
	AMyMonster* MyMonster = Cast<AMyMonster>(AI->GetPawn());
	MyMonster->Respawn();
	return Super::ExecuteTask(OwnerComp, NodeMemory);
}
