#include "MyHitStopMontage_Cue.h"
#include "../MySkillSystemComponent.h"
#include "../../MyCharacter.h"

bool AMyHitStopMontage_Cue::OnExecute_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters)
{
	if (MyTarget == nullptr)
		return false;
	
	AMyCharacter* MyCharacter = Cast<AMyCharacter>(MyTarget);
	if (MyCharacter == nullptr)
		return false;
	
	// LocalPredicted ability:
	// - client plays hitstop immediately
	// - server cue must be filtered locally
	// - other players must still receive server cue
	if (MyCharacter->IsLocallyControlled() == true)
	{
		if (Parameters.AggregatedSourceTags.IsEmpty() == true)
			return false;
		
		FGameplayTag SkillTag = Parameters.AggregatedSourceTags.First();
		const FGameplayAbilitySpec *Spec = MyCharacter->MySkillSystemComponent->GetGameplayAbilitySpec(MyCharacter->GetAbilitySystemComponent(), SkillTag);
		
		if (Spec->Ability->GetNetExecutionPolicy() == EGameplayAbilityNetExecutionPolicy::LocalPredicted)
		{
			UE_LOG(LogTemp, Warning, TEXT("LocalPredicted has Played"));
			return false;
		}
	}
	
	MyCharacter->PlayHitStop(Parameters.RawMagnitude);
	UE_LOG(LogTemp, Warning, TEXT("AMyHitMontage_Cue EXECUTED"));
	return true;
}
