#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "../MySkillSystem/MyAttributeSet.h"
#include "../MyCharacter.h"
#include "MyCharacterConfig.generated.h"

USTRUCT(BlueprintType)
struct FMyConfigItem
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<EAttributeType, int> Attributes;
};

USTRUCT(BlueprintType)
struct FMySkillItem
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TArray<FGameplayTag> Skills;
};

USTRUCT(BlueprintType)
struct FMyCharacterMontage
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UAnimMontage> HitMontage;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TObjectPtr<UAnimMontage> DeadMontage;
};

UCLASS(BlueprintType)
class GAS_API UMyCharacterConfig : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<ECharacterType, FMyConfigItem> AttributeTable;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<ECharacterType, FMySkillItem> SkillTable;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<ECharacterType, FMyCharacterMontage> MontageTable;
};
