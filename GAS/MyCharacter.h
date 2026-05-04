#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "MySkillSystem/MyAttributeSet.h"
#include "MyCharacter.generated.h"

class UAbilitySystemComponent;
class UMyCharacterConfig;
class UMyAttributeSet;
class UWidgetComponent;
class UMySkillSystemComponent;
class UMyBarUserWidget;

UENUM(BlueprintType)
enum class ECharacterType : uint8
{
	Player,
	Monster_Melee,
	Monster_Ranged,
	Count
};

TSubclassOf<AMyCharacter> GetEnemyClass(TSubclassOf<AMyCharacter> SelfClass);

UCLASS()
class GAS_API AMyCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AMyCharacter();
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;
	
	UPROPERTY(EditDefaultsOnly, Category="MyGAS")
	TObjectPtr<UMySkillSystemComponent> MySkillSystemComponent;
	
	UMyAttributeSet *GetMyAttributeSet(){ return MyAttributeSet; };
	
	void InitializeAttributeSet();
	
protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual bool CanMove(){ return true; }
	void SetAttributeValue(EAttributeType type, int Value);
	void InitializeSkill();
	void GiveGameplayAbility(FGameplayTag SkillTag);
	bool TryBindBarWidget(UAbilitySystemComponent *ASC);
	
	UPROPERTY()
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
	
	UPROPERTY()
	TObjectPtr<UMyAttributeSet> MyAttributeSet;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="MyGAS")
	TObjectPtr<UWidgetComponent> MyBarComponent;
	
	UPROPERTY(EditDefaultsOnly, Category="MyGAS")
	TSubclassOf<UMyBarUserWidget> MyBarWidgetClass;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="MyGAS")
	ECharacterType SelfType = ECharacterType::Count;
	
	void PlayHitStop(float Duration);
	void Respawn();
	FVector3d RespawnPosition;
};
