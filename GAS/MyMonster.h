// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyCharacter.h"
#include "MyMonster.generated.h"

UCLASS()
class GAS_API AMyMonster : public AMyCharacter
{
	GENERATED_BODY()

public:
	AMyMonster();

public:
	virtual void BeginPlay() override;
	virtual bool CanMove() override;
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Config")
	float ChaseRange = 0;
};
