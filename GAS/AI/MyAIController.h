#pragma once

#include "CoreMinimal.h"
#include "Runtime/AIModule/Classes/AIController.h"
#include "MyAIController.generated.h"

class AMyPlayer;

UCLASS()
class GAS_API AMyAIController : public AAIController
{
	GENERATED_BODY()

public:
	AMyAIController();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;
	virtual void OnPossess(APawn* InPawn) override;
	
	UFUNCTION()
	void OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "GAS")
	UAIPerceptionComponent* PerceptionComp;
	
	UPROPERTY(EditDefaultsOnly, Category = "GAS")
	TObjectPtr<UBehaviorTree> BT_Monster;
	
	UPROPERTY(EditDefaultsOnly, Category = "GAS")
	TObjectPtr<UBlackboardData> BB_Monster;
	
	UBlackboardComponent* BB_MonsterComponent = nullptr;
	
public:
	TSet<TWeakObjectPtr<AMyPlayer>> PerceivedPlayers;
};
