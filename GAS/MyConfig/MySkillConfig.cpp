#include "MySkillConfig.h"

FSkillConfigItem* UMySkillConfig::GetSkillConfigItem(FGameplayTag SkillTag)
{
	return Skills.Find(SkillTag);
}
