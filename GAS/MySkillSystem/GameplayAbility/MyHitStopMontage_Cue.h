// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameplayCueNotify_Actor.h"
#include "MyHitStopMontage_Cue.generated.h"

UCLASS()
class GAS_API AMyHitStopMontage_Cue : public AGameplayCueNotify_Actor
{
	GENERATED_BODY()

public:
	virtual bool OnExecute_Implementation(AActor* MyTarget, const FGameplayCueParameters& Parameters) override;
};
