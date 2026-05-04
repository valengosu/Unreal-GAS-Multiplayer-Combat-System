#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayTagContainer.h"
#include "MyHitBoxActor_NonRep.generated.h"

class AMyCharacter;
class UBoxComponent;

UCLASS()
class GAS_API AMyHitBoxActor_NonRep : public AActor
{
	GENERATED_BODY()

public:
	AMyHitBoxActor_NonRep();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	
public:
	void SetActive(bool NewState);
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HitBox")
	UBoxComponent* HitBox;
	
	FGameplayTag SkillTag;
	TSubclassOf<AMyCharacter> EnemyClass;
	
private:
	bool bTracing;
	TSet<AActor*> AlreadyHitCharacters;
	FVector PreviousLocation;
};
