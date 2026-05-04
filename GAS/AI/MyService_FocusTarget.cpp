#include "MyService_FocusTarget.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "GAS/MyMonster.h"
#include "GAS/MyPlayer.h"

UMyService_FocusTarget::UMyService_FocusTarget()
{
	NodeName = TEXT("FocusTarget");
	Interval = 0.1f;
	RandomDeviation = 0.f;
}

void UMyService_FocusTarget::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	
	UBlackboardComponent* BB = OwnerComp.GetBlackboardComponent();
	AMyPlayer* Target = Cast<AMyPlayer>(BB->GetValueAsObject(BlackboardKey.SelectedKeyName));
	if (Target == nullptr)
		return;
	
	AMyMonster* MyMonster = Cast<AMyMonster>(OwnerComp.GetAIOwner()->GetPawn());
	
	FVector ToTarget = Target->GetActorLocation() - MyMonster->GetActorLocation();
	ToTarget.Z = 0.f;
	if (ToTarget.IsNearlyZero())
		return;
	
	FRotator Rotation = ToTarget.Rotation();
	FRotator CurrentRotation = MyMonster->GetActorRotation();
	FRotator NewRotation = FMath::RInterpTo(CurrentRotation, Rotation, DeltaSeconds, 8.f);
	
	MyMonster->SetActorRotation(NewRotation);
}
