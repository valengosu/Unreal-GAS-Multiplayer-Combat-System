// Fill out your copyright notice in the Description page of Project Settings.


#include "MyPlayer.h"
#include "MyPlayerState.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

AMyPlayer::AMyPlayer()
{
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 540.f, 0.f);
	
	PrimaryActorTick.bCanEverTick = true;
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(RootComponent);   
	SpringArm->TargetArmLength = 600.f;       
	SpringArm->bUsePawnControlRotation = true;
	
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;
}

void AMyPlayer::BeginPlay()
{
	Super::BeginPlay();
	
	if (HasAuthority() == false)
	{
		TryBindBarWidget(AbilitySystemComponent); 
	}
}

void AMyPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AMyPlayer::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);
	
	AMyPlayerState* PS = GetPlayerState<AMyPlayerState>();
	PS->AbilitySystemComponent->InitAbilityActorInfo(PS, this);
	
	AbilitySystemComponent = PS->AbilitySystemComponent; 
	MyAttributeSet = PS->MyAttributeSet;
	
	InitializeAttributeSet();
	InitializeSkill();
}

void AMyPlayer::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	
	AMyPlayerState* PS = GetPlayerState<AMyPlayerState>();
	PS->AbilitySystemComponent->InitAbilityActorInfo(PS, this);
	
	AbilitySystemComponent = PS->AbilitySystemComponent; 
	MyAttributeSet = PS->MyAttributeSet;
	
	TryBindBarWidget(AbilitySystemComponent);
}

bool AMyPlayer::CanMove()
{
	FGameplayTagContainer BlockTags;
	BlockTags.AddTag(FGameplayTag::RequestGameplayTag(TEXT("GAS.Character.Dead")));
	BlockTags.AddTag(FGameplayTag::RequestGameplayTag(TEXT("GAS.Skill.Hit")));
	
	bool bAny = GetAbilitySystemComponent()->HasAnyMatchingGameplayTags(BlockTags);
	return !bAny;
}


