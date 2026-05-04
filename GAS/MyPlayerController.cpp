#include "MyPlayerController.h"

#include <rapidjson/document.h>

#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "MyCharacter.h"
#include "MyPlayer.h"
#include "MySkillSystem/MySkillSystemComponent.h"
#include "GameFramework/Character.h"

void AMyPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	
	if (IsLocalController() == true)
	{
		UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
		for (auto imc : InputMappingContexts)
		{
			Subsystem->AddMappingContext(imc, 0);
		}
	}
	
	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent);
	EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ThisClass::Look);
	EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ThisClass::Move);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ThisClass::Jump);
	EnhancedInputComponent->BindAction(PrimaryAction, ETriggerEvent::Started, this, &ThisClass::Primary);
}

void AMyPlayerController::Primary()
{
	AMyCharacter* MyCharacter = Cast<AMyCharacter>(GetPawn());
	MyCharacter->MySkillSystemComponent->TryCastSkill(FGameplayTag::RequestGameplayTag(TEXT("GAS.Skill.Id.1001")));
}

void AMyPlayerController::Jump()
{
	AMyPlayer* MyPlayer = Cast<AMyPlayer>(GetPawn());
	
	if(MyPlayer != nullptr && MyPlayer->CanMove() == true)
	{
		MyPlayer->Jump();
	} 
}

void AMyPlayerController::Look(const FInputActionValue& Value)
{
	FVector2D Vector2D = Value.Get<FVector2D>();
	
	AddYawInput(Vector2D.X * 0.2);
	AddPitchInput(Vector2D.Y * 0.2);
}

void AMyPlayerController::Move(const FInputActionValue& Value)
{
	AMyPlayer* MyPlayer = Cast<AMyPlayer>(GetPawn());
	
	if (MyPlayer != nullptr && MyPlayer->CanMove() == true)
	{
		FVector2D MoveVector2D = Value.Get<FVector2D>();
		FRotator YawRotation(0.f, GetControlRotation().Yaw, 0.f);
		FVector ForwardVector = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		FVector RightVector = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	
		MyPlayer->AddMovementInput(RightVector, MoveVector2D.X);
		MyPlayer->AddMovementInput(ForwardVector, MoveVector2D.Y);	
	}
}
