#include "MyABMelee_Base.h"
#include "../../MyConfig/MySkillConfig.h"
#include "../../MyConfig/MyConfigInstance.h"
#include "Animation/AnimMontage.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"
#include "GAS/MyCharacter.h"
#include "../HitDetection/MyHitBoxActor_NonRep.h"
#include "Components/BoxComponent.h"

UMyABMelee_Base::UMyABMelee_Base()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	
	ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag(TEXT("GAS.Skill.Casting")));
	ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag(TEXT("GAS.Character.Dead")));
	ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag(TEXT("GAS.Skill.Hit")));
	
	CurrentTag = FGameplayTag::RequestGameplayTag(TEXT("GAS.Skill.Casting"));
}

void UMyABMelee_Base::ActivateAbility(const FGameplayAbilitySpecHandle Handle,
                                       const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo,
                                       const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
		UE_LOG(LogTemp, Warning, TEXT("Server1: %d"), ActorInfo->IsNetAuthority());
		EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
		return;
	}
	
	FGameplayTag SkillTag = GetSkillTag(ActorInfo, Handle);
	FSkillConfigItem *SkillConfigItem = GetSkillConfigItem(ActorInfo->OwnerActor.Get(), SkillTag);
	
	if (SkillConfigItem->SkillMontage != nullptr)
	{
		UAbilityTask_PlayMontageAndWait* Task = UAbilityTask_PlayMontageAndWait::CreatePlayMontageAndWaitProxy(this, NAME_None, SkillConfigItem->SkillMontage);
		BindMontageCallbacks(Task);
		Task->ReadyForActivation();
	}
	
	CreateHitBoxActor(ActorInfo->AvatarActor.Get(), SkillTag);
	//Force immediate ASC replication to avoid montage start delay caused by low NetUpdateFrequency (replication tick lag)
	ActorInfo->AbilitySystemComponent.Get()->ForceReplication();
}

void UMyABMelee_Base::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo,
									const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	DestoryHitBoxActor(ActorInfo->AvatarActor.Get());	
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);
}

void UMyABMelee_Base::DestoryHitBoxActor(AActor* Owner)
{
	TArray<AActor*> AttachedActors;
	Owner->GetAttachedActors(AttachedActors);
	
	for (AActor* Actor : AttachedActors)
	{
		if (AMyHitBoxActor_NonRep* HitBoxActor = Cast<AMyHitBoxActor_NonRep>(Actor))
		{
			HitBoxActor->Destroy();
			break;
		}
	}
}

void UMyABMelee_Base::CreateHitBoxActor(AActor* Owner, FGameplayTag SkillTag)
{
	USkeletalMeshComponent* MeshComp = Cast<AMyCharacter>(Owner)->GetMesh();
	
	UBoxComponent* HitBox = nullptr;
	TArray<USceneComponent*> Children;
	MeshComp->GetChildrenComponents(false, Children);
	for (USceneComponent* Child : Children)
	{
		if (Child->GetName().Contains(TEXT("HitBox")))
		{
			HitBox = Cast<UBoxComponent>(Child);
			break;
		}
	}
	//UE_LOG(LogTemp, Warning, TEXT("No HitBox!!= %s"), *MeshComp->GetName());
	check(HitBox);
	
	FActorSpawnParameters Params;
	Params.Owner = Owner;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	AMyHitBoxActor_NonRep* MyHitBoxActor = MeshComp->GetWorld()->SpawnActor<AMyHitBoxActor_NonRep>(AMyHitBoxActor_NonRep::StaticClass(), FTransform::Identity, Params);
	
	MyHitBoxActor->SkillTag = SkillTag;
	MyHitBoxActor->Tags.Add(TEXT("AttackHitBox"));
	MyHitBoxActor->HitBox = HitBox;
	MyHitBoxActor->AttachToActor(Owner, FAttachmentTransformRules::SnapToTargetIncludingScale);
	MyHitBoxActor->EnemyClass = GetEnemyClass(Owner->GetClass());
}    
