// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"
#include "AbilitySystemComponent.h"
#include "MyAttributeSet.generated.h"


#define ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_SETTER(PropertyName) \
GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

UENUM(BlueprintType)
enum class EAttributeType : uint8
{
	Hp,
	MaxHp,
	Mp,
	MaxMp,
	Attack,
	Defense,
	Count
};

UCLASS()
class GAS_API UMyAttributeSet : public UAttributeSet
{
	GENERATED_BODY()
	
public:
	virtual void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void PostGameplayEffectExecute(const FGameplayEffectModCallbackData& Data) override;
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Hp)
	FGameplayAttributeData Hp;
	UFUNCTION()
	void OnRep_Hp(const FGameplayAttributeData& OldValue);
	ATTRIBUTE_ACCESSORS(ThisClass, Hp);
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxHp)
	FGameplayAttributeData MaxHp;
	UFUNCTION()
	void OnRep_MaxHp(const FGameplayAttributeData& OldValue);
	ATTRIBUTE_ACCESSORS(ThisClass, MaxHp);
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Mp)
	FGameplayAttributeData Mp;
	UFUNCTION()
	void OnRep_Mp(const FGameplayAttributeData& OldValue);
	ATTRIBUTE_ACCESSORS(ThisClass, Mp);
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_MaxMp)
	FGameplayAttributeData MaxMp;
	UFUNCTION()
	void OnRep_MaxMp(const FGameplayAttributeData& OldValue);
	ATTRIBUTE_ACCESSORS(ThisClass, MaxMp);
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Attack)
	FGameplayAttributeData Attack;
	UFUNCTION()
	void OnRep_Attack(const FGameplayAttributeData& OldValue);
	ATTRIBUTE_ACCESSORS(ThisClass, Attack);
	
	UPROPERTY(BlueprintReadOnly, ReplicatedUsing = OnRep_Defense)
	FGameplayAttributeData Defense;
	UFUNCTION()
	void OnRep_Defense(const FGameplayAttributeData& OldValue);
	ATTRIBUTE_ACCESSORS(ThisClass, Defense);
};
