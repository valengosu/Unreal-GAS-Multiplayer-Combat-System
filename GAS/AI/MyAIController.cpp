#include "MyAIController.h"

#include "BehaviorTree/BlackboardComponent.h"
#include "Perception/AISense.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AISense_Sight.h"
#include "GAS/MyMonster.h"
#include "GAS/MyPlayer.h"
#include "Microsoft/AllowMicrosoftPlatformTypes.h"
#include "Perception/AIPerceptionComponent.h"

AMyAIController::AMyAIController()
{
	PrimaryActorTick.bCanEverTick = false;
	
	PerceptionComp = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComp"));
	SetPerceptionComponent(*PerceptionComp);
}

void AMyAIController::BeginPlay()
{
	Super::BeginPlay();
	
	PerceptionComp->OnTargetPerceptionUpdated.AddDynamic(this, &AMyAIController::OnPerceptionUpdated);
	
	UAISenseConfig_Sight* SightConfig = Cast<UAISenseConfig_Sight>(PerceptionComp->GetSenseConfig(UAISense::GetSenseID<UAISense_Sight>()));
	if (SightConfig == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("Can not find Sight sense!!!"));
		return;
	}
	//SightConfig->SightRadius = MyMonste->SightDistance;
	//PerceptionComp->RequestStimuliListenerUpdate();
}

void AMyAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMyAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	
	UseBlackboard(BB_Monster, BB_MonsterComponent);
	RunBehaviorTree(BT_Monster);
}

void AMyAIController::OnPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	AMyPlayer* Player = Cast<AMyPlayer>(Actor);
	if (Player == nullptr)
		return;
	
	if (Stimulus.WasSuccessfullySensed() == false)
	{
		PerceivedPlayers.Remove(Player);
		return;
	}
	
	PerceivedPlayers.Add(Player);
	
	float MinDistance = FLT_MAX;
	AMyPlayer *Target = nullptr;
	for (auto Elem : PerceivedPlayers)
	{
		if (Elem.IsValid() == false)
			continue;
		
		float Distance = FVector::Dist(Actor->GetActorLocation(), GetPawn()->GetActorLocation());
		if (Distance < MinDistance)
		{
			UE_LOG(LogTemp, Warning, TEXT("Distance is: %f"), Distance);
			MinDistance = Distance;
			Target = Elem.Get();
		}
	}
	
	BB_MonsterComponent->SetValueAsObject(TEXT("TargetActor"), Target);
	
	for (auto Elem = PerceivedPlayers.CreateIterator(); Elem; ++Elem)
	{
		if (Elem->IsValid() == false)
		{
			Elem.RemoveCurrent();
		}
	}
}

