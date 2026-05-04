// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "GameplayTagContainer.h"
#include "MyBarWidgetComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class GAS_API UMyBarWidgetComponent : public UWidgetComponent
{
	GENERATED_BODY()
public:
	UMyBarWidgetComponent();
	void CastSkill(int SkillId);

protected:
	virtual void BeginPlay() override;
	
public:
};
