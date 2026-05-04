#include "MyHitBoxActor_NonRep.h"
#include "Components/BoxComponent.h"
#include "Animation/AnimInstance.h"
#include "Components/SkeletalMeshComponent.h"
#include "../../MyCharacter.h"
#include "AbilitySystemBlueprintLibrary.h"

AMyHitBoxActor_NonRep::AMyHitBoxActor_NonRep()
{
	bReplicates = false;
	
	bTracing = false;
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
}

void AMyHitBoxActor_NonRep::BeginPlay()
{
	Super::BeginPlay();
}

void AMyHitBoxActor_NonRep::SetActive(bool NewState)
{
	bTracing = NewState;
	
	if (bTracing == true && IsValid(HitBox) == true)
	{
		PreviousLocation = HitBox->GetComponentLocation();
	}
}

void AMyHitBoxActor_NonRep::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (bTracing == false || IsValid(HitBox) == false || IsValid(GetOwner()) == false)
		return;
	
	FVector BoxHalfSize = HitBox->GetScaledBoxExtent();
	TArray<FHitResult> Hits;
	FCollisionQueryParams Params;
	GetWorld()->SweepMultiByChannel(Hits,PreviousLocation, HitBox->GetComponentLocation(), HitBox->GetComponentQuat(), ECC_GameTraceChannel1, FCollisionShape::MakeBox(BoxHalfSize), Params);
	PreviousLocation = HitBox->GetComponentLocation();
	
	for (const FHitResult& Hit : Hits)
	{
		AActor* OtherActor = Hit.GetActor();
		
		if (OtherActor->IsA(EnemyClass) == false)
			continue;
		
		UAbilitySystemComponent *OtherASC = Cast<AMyCharacter>(OtherActor)->GetAbilitySystemComponent();
		if (OtherASC == nullptr || OtherASC->HasMatchingGameplayTag(FGameplayTag::RequestGameplayTag(TEXT("GAS.Character.Dead"))))
			continue;
		
		if (AlreadyHitCharacters.Contains(OtherActor) == true)
			continue;
		
		AlreadyHitCharacters.Add(OtherActor);
		//DrawDebugSphere(GetWorld(), Hit.ImpactPoint,10.f,12, FColor::Red,false,10.f);
		
		if (GetNetMode() == NM_Client)
		{
			Cast<AMyCharacter>(GetOwner())->PlayHitStop(0.07);
			return;
		}
		
		// GetNetMode() != NM_Client, On Server:
		UAbilitySystemComponent *OwnerASC = Cast<AMyCharacter>(GetOwner())->GetAbilitySystemComponent();
		{
			FGameplayCueParameters CueParams;
			CueParams.Location= Hit.ImpactPoint;
			CueParams.AggregatedSourceTags.AddTag(SkillTag); 
			OwnerASC->ExecuteGameplayCue(FGameplayTag::RequestGameplayTag(TEXT("GameplayCue.Niagara")), CueParams);
		} 
			
		{
			FGameplayCueParameters CueParams;
			CueParams.RawMagnitude = 0.07;
			CueParams.AggregatedSourceTags.AddTag(SkillTag);
			OwnerASC->ExecuteGameplayCue(FGameplayTag::RequestGameplayTag(TEXT("GameplayCue.HitStop")), CueParams);	
		}
			
		FGameplayEventData EventData;
		EventData.EventTag = FGameplayTag::RequestGameplayTag(TEXT("GAS.Skill.Hit"));
		EventData.Instigator = GetOwner();
		EventData.InstigatorTags.AddTag(SkillTag);
		EventData.Target = OtherActor;
		UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(OtherActor, EventData.EventTag, EventData);	
	}
}


