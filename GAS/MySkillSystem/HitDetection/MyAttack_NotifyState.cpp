#include "MyAttack_NotifyState.h"
#include "MyHitBoxActor_NonRep.h"

void UMyAttack_NotifyState::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration,  const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration);
	
	TArray<AActor*> AttachedActors;
	MeshComp->GetOwner()->GetAttachedActors(AttachedActors);
	
	for (AActor* Actor : AttachedActors)
	{
		if (AMyHitBoxActor_NonRep* HitBoxActor = Cast<AMyHitBoxActor_NonRep>(Actor))
		{
			HitBoxActor->SetActive(true);
			break;
		}
	}
}

void UMyAttack_NotifyState::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyTick(MeshComp, Animation, FrameDeltaTime); 
}

void UMyAttack_NotifyState::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation);
	
	TArray<AActor*> AttachedActors;
	MeshComp->GetOwner()->GetAttachedActors(AttachedActors);
	
	for (AActor* Actor : AttachedActors)
	{
		if (AMyHitBoxActor_NonRep* HitBoxActor = Cast<AMyHitBoxActor_NonRep>(Actor))
		{
			HitBoxActor->SetActive(false);
			break;
		}
	}
}
