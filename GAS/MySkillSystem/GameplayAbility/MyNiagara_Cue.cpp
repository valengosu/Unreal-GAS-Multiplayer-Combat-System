#include "MyNiagara_Cue.h"
#include "../../MyConfig/MyConfigInstance.h"
#include "Kismet/GameplayStatics.h"
#include "GAS/MyConfig/MySkillConfig.h"

bool AMyNiagara_Cue::OnExecute_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters)
{
	if (MyTarget == nullptr)
		return false;
	
	if (Parameters.AggregatedSourceTags.IsEmpty() == true)
		return false;
	
	FGameplayTag SkillTag = Parameters.AggregatedSourceTags.First();
	FSkillConfigItem *Item = GetSkillConfigItem(this, SkillTag);
	if (Item == nullptr || Item->NiagaraFX == nullptr)
		return false;
	
	UGameplayStatics::SpawnEmitterAtLocation(MyTarget->GetWorld(), Item->NiagaraFX, Parameters.Location, FRotator::ZeroRotator, true);
	//UNiagaraFunctionLibrary::SpawnSystemAtLocation(, Item->NiagaraFX, Parameters.Location, FRotator::ZeroRotator, FVector(1.f), true, true);
	return true;
}
