#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayTagContainer.h"
#include "../../MyCharacter.h"
#include "MyProjectileActor.generated.h"



class UProjectileMovementComponent;
class UBoxComponent;

UCLASS()
class GAS_API AMyProjectileActor : public AActor
{
	GENERATED_BODY()

public:
	AMyProjectileActor();

protected:
	virtual void BeginPlay() override;
	virtual void PostNetInit() override;
	
	UFUNCTION()
	void OnHit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
public:
	virtual void Tick(float DeltaTime) override;
	
	UPROPERTY(VisibleAnywhere)
	USceneComponent* Root;

	UPROPERTY(VisibleAnywhere)
	UBoxComponent* HitBox;
	
	UPROPERTY(VisibleAnywhere)
	UProjectileMovementComponent* Movement;
	
	UPROPERTY(EditDefaultsOnly, Category="MyGAS")
	UParticleSystem* NiagaraFX;
	
	UPROPERTY(Replicated)
	FPredictionKey PredictionKey;
	
	FGameplayTag SkillTag;
	TSubclassOf<AMyCharacter> EnemyClass;
	
	bool isPredicted = false;
};
