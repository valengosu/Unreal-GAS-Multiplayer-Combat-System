// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "GameplayTagContainer.h"
#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "MySkillConfig.generated.h"

class UGameplayAbility;
class AMyProjectileActor;
class UNiagaraSystem;

USTRUCT(BlueprintType) 
struct FSkillConfigItem
{
	GENERATED_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	int InterruptLevel;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UParticleSystem* NiagaraFX;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UAnimMontage> SkillMontage;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UGameplayAbility> GameplayAbility;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<AMyProjectileActor> ProjectileClass;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float SkillRange;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float Cooldown;
};

UCLASS(BlueprintType)
class GAS_API UMySkillConfig : public UDataAsset
{
	GENERATED_BODY()

public:
	FSkillConfigItem* GetSkillConfigItem(FGameplayTag SkillTag);
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<FGameplayTag, FSkillConfigItem> Skills;
};

