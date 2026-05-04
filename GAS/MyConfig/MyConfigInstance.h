#pragma once

#include "CoreMinimal.h"
#include "../MyCharacter.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "MyConfigInstance.generated.h"

class UMyCharacterConfig;
class UMySkillConfig;
struct FMyCharacterMontage;
struct FSkillConfigItem;

FSkillConfigItem *GetSkillConfigItem(AActor *Actor, FGameplayTag SkillTag);
FMyCharacterMontage *GetCharacterMontage(AActor* Actor, ECharacterType type);

UCLASS(BlueprintType, Blueprintable)
class GAS_API UMyConfigInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Gas")
	TObjectPtr<UMyCharacterConfig> MyCharacterConfig;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Gas")
	TObjectPtr<UMySkillConfig> MySkillConfig;
};
