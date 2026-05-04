#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "Components/ActorComponent.h"
#include "MySkillSystemComponent.generated.h"

struct FSkillConfigItem;
struct FGameplayAbilitySpec;
class UAbilitySystemComponent;

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GAS_API UMySkillSystemComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UMySkillSystemComponent();
	bool CanCastSkill();
	const FGameplayAbilitySpec* GetGameplayAbilitySpec(UAbilitySystemComponent *ASC, FGameplayTag SkillTag);
	void TryCastSkill(FGameplayTag SkillTag);
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	UPROPERTY(Replicated)
	int CurrentSkillId;
};
