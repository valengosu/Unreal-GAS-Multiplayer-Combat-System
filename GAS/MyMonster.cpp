#include "MyMonster.h"
#include "AbilitySystemComponent.h"
#include "MySkillSystem/MyAttributeSet.h"
#include "GameFramework/CharacterMovementComponent.h"

AMyMonster::AMyMonster()
{
	PrimaryActorTick.bCanEverTick = false;
	
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw   = false;
	bUseControllerRotationRoll  = false;
	GetCharacterMovement()->bOrientRotationToMovement = false;
	
	MyAttributeSet = CreateDefaultSubobject<UMyAttributeSet>(FName("MyAttributeSet"));
	
	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>("MyAbilitySystemComponent");
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
	AbilitySystemComponent->SetIsReplicated(true);
}

void AMyMonster::BeginPlay()
{
	Super::BeginPlay();
	
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	
	if (HasAuthority() == true)
	{
		InitializeSkill();
		InitializeAttributeSet();
	}
	
	if (HasAuthority() == false)
	{
		TryBindBarWidget(AbilitySystemComponent); 
	}
}

bool AMyMonster::CanMove()
{
	FGameplayTagContainer BlockTags;
	BlockTags.AddTag(FGameplayTag::RequestGameplayTag(TEXT("GAS.Character.Dead")));
	BlockTags.AddTag(FGameplayTag::RequestGameplayTag(TEXT("GAS.Skill.Hit")));
	BlockTags.AddTag(FGameplayTag::RequestGameplayTag(TEXT("GAS.Skill.Casting")));
	
	bool bAny = GetAbilitySystemComponent()->HasAnyMatchingGameplayTags(BlockTags);
	return !bAny;
}

