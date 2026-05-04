#include "MyProjectileActor.h"
#include "Net/UnrealNetwork.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/BoxComponent.h"
#include "GAS/MySkillSystem/GameplayAbility/MyAB_Base.h"

extern TMap<FPredictionKey, TWeakObjectPtr<AActor>> PredictionActorMap;

AMyProjectileActor::AMyProjectileActor()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	HitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("HitBox"));
	HitBox->SetupAttachment(Root);
	HitBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	HitBox->SetCollisionObjectType(ECC_GameTraceChannel1);
	HitBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	HitBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	HitBox->OnComponentBeginOverlap.AddDynamic(this, &AMyProjectileActor::OnHit);
	
	Movement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Movement"));
	Movement->InitialSpeed = 5000.f;
	Movement->MaxSpeed = 0.f;
	Movement->ProjectileGravityScale = 0.f;
	
	InitialLifeSpan = 10.f;
}

void AMyProjectileActor::BeginPlay()
{
	Super::BeginPlay();
}

void AMyProjectileActor::PostNetInit()
{
	Super::PostNetInit();
	
	auto PredictionActor = UMyAB_Base::PredictionActorMap.Find(PredictionKey);
	if (PredictionActor != nullptr)
	{
		if (PredictionActor->IsValid() == true)
		{
			PredictionActor->Get()->Destroy();	
		}
		UMyAB_Base::PredictionActorMap.Remove(PredictionKey);
	}
}

void AMyProjectileActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMyProjectileActor::OnHit(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (isPredicted == true)
		return;
	
	if (OtherActor->IsA(EnemyClass) == false)
		return;
	
	if (IsValid(GetOwner()) == false)
		return;
	
	UAbilitySystemComponent *OwnerASC = Cast<AMyCharacter>(GetOwner())->GetAbilitySystemComponent();
	FGameplayCueParameters CueParams;
	CueParams.Location = GetActorLocation();  
	CueParams.AggregatedSourceTags.AddTag(SkillTag); 
	OwnerASC->ExecuteGameplayCue(FGameplayTag::RequestGameplayTag(TEXT("GameplayCue.Niagara")), CueParams);
	
	FGameplayEventData EventData;
	EventData.EventTag = FGameplayTag::RequestGameplayTag(TEXT("GAS.Skill.Hit"));
	EventData.Instigator = GetOwner();
	EventData.InstigatorTags.AddTag(SkillTag);
	EventData.Target = OtherActor;
	UAbilitySystemBlueprintLibrary::SendGameplayEventToActor(OtherActor, EventData.EventTag, EventData);	
	
	this->Destroy();
}

void AMyProjectileActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AMyProjectileActor, PredictionKey);
}

