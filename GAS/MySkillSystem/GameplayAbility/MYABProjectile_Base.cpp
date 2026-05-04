#include "MYABProjectile_Base.h"
#include "../../MyConfig/MySkillConfig.h"
#include "../../MyConfig/MyConfigInstance.h"
#include "../HitDetection/MyProjectileActor.h"
#include "Abilities/Tasks/AbilityTask_PlayMontageAndWait.h"


UMYABProjectile_Base::UMYABProjectile_Base()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	
	ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag(TEXT("GAS.Skill.Casting")));
	ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag(TEXT("GAS.Character.Dead")));
	ActivationBlockedTags.AddTag(FGameplayTag::RequestGameplayTag(TEXT("GAS.Skill.Hit")));
	
	CurrentTag = FGameplayTag::RequestGameplayTag(TEXT("GAS.Skill.Casting"));
}

void UMYABProjectile_Base::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, 
											const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
	
	if (!CommitAbility(Handle, ActorInfo, ActivationInfo))
	{
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
	
	FPredictionKey PredictionKey = ActivationInfo.GetActivationPredictionKey();
	AMyProjectileActor* MyProjectileActor = CreateProjectile(ActorInfo->AvatarActor.Get(), SkillTag, SkillConfigItem->ProjectileClass, PredictionKey);
	if (MyProjectileActor == nullptr)
		return;
	
	if (NetExecutionPolicy == EGameplayAbilityNetExecutionPolicy::LocalPredicted)
	{
		if (ActorInfo->IsNetAuthority() == true)
		{
			Client_ConfirmPrediction(PredictionKey);	
		}
		
		else if (ActorInfo->IsNetAuthority() == false)
		{
			PredictionKey.NewRejectedDelegate().BindLambda(
		[Projectile = TWeakObjectPtr<AMyProjectileActor>(MyProjectileActor)]()
			{
				if (Projectile.IsValid())
				{
					Projectile->Destroy();
				}
			});
		
			MyProjectileActor->isPredicted = true;
			PredictionActorMap[PredictionKey] = MyProjectileActor;
		}
	}
}

AMyProjectileActor* UMYABProjectile_Base::CreateProjectile(AActor* Owner, FGameplayTag SkillTag, TSubclassOf<AMyProjectileActor> ProjectileClass, const FPredictionKey &PredictionKey)
{
	USkeletalMeshComponent* MeshComp = Cast<AMyCharacter>(Owner)->GetMesh();
	FName SocketName = TEXT("Muzzle_Front");
	
	if (MeshComp->DoesSocketExist(SocketName) == false)
	{
		UE_LOG(LogTemp, Error, TEXT("No Socket: %s"), *SocketName.ToString());
		return nullptr;
	}
	
	FVector SpawnLoc = MeshComp->GetSocketLocation(SocketName);
	FRotator SpawnRot = MeshComp->GetSocketRotation(SocketName);
	SpawnRot.Pitch = 0;
	
	FActorSpawnParameters Params;
	Params.Owner = Owner;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	AMyProjectileActor* Projectile = Owner->GetWorld()->SpawnActor<AMyProjectileActor>(ProjectileClass, SpawnLoc, SpawnRot, Params);
	Projectile->SkillTag = SkillTag;
	Projectile->PredictionKey = PredictionKey;
	Projectile->EnemyClass = GetEnemyClass(Owner->GetClass());
	
	return Projectile;
}

