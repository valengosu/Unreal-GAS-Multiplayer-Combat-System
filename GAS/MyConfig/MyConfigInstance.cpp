#include "MyConfigInstance.h"
#include "MyCharacterConfig.h"
#include "MySkillConfig.h"

FSkillConfigItem* GetSkillConfigItem(AActor* Actor, FGameplayTag SkillTag)
{
	if (Actor == nullptr)
		return nullptr;
	
	UMyConfigInstance *MyConfigInstance = Actor->GetGameInstance<UMyConfigInstance>();
	FSkillConfigItem *Item = MyConfigInstance->MySkillConfig->Skills.Find(SkillTag);
	return Item;
}

FMyCharacterMontage* GetCharacterMontage(AActor* Actor, ECharacterType type)
{
	if (Actor == nullptr)
		return nullptr;
	
	UMyConfigInstance *MyConfigInstance = Actor->GetGameInstance<UMyConfigInstance>();
	FMyCharacterMontage *Montages = MyConfigInstance->MyCharacterConfig->MontageTable.Find(type);
	return Montages;
}
