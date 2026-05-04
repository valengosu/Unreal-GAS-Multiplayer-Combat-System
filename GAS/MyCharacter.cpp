#include "MyCharacter.h"
#include "MySkillSystem/MyAttributeSet.h"
#include "MyConfig/MyCharacterConfig.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/PlayerCameraManager.h"
#include "MyConfig/MyConfigInstance.h"
#include "AbilitySystemComponent.h"
#include "MySkillSystem/GameplayAbility/GameplayEffect/MyGE_SetAttr.h"
#include "MyBarWidgetComponent.h"
#include "MyBarUserWidget.h"
#include "Components/SkeletalMeshComponent.h"
#include "MyPlayerState.h"
#include "MyMonster.h"
#include "MyPlayer.h"
#include "MySkillSystem/MySkillSystemComponent.h"
#include "MyConfig/MySkillConfig.h"
#include "MySkillSystem/GameplayAbility/MyAB_BeHit_ServerInitiated.h"
#include "MySkillSystem/GameplayAbility/MyAB_Death_ServerInitiated.h"

TSubclassOf<AMyCharacter> GetEnemyClass(TSubclassOf<AMyCharacter> SelfClass)
{
	if (SelfClass->IsChildOf(AMyMonster::StaticClass()))
	{
		return AMyPlayer::StaticClass();
	}
	if (SelfClass->IsChildOf(AMyPlayer::StaticClass()))
	{
		return AMyMonster::StaticClass();
	}
	return nullptr;
}

void AMyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

AMyCharacter::AMyCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	
	MyBarComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("BarComponent"));
	MyBarComponent->SetupAttachment(GetRootComponent());
	MyBarComponent->SetDrawAtDesiredSize(true);
	MyBarComponent->SetVisibility(true);
	MyBarComponent->SetWidgetSpace(EWidgetSpace::Screen);
	
	MySkillSystemComponent = CreateDefaultSubobject<UMySkillSystemComponent>(TEXT("MySkillSystemComponent"));
	MySkillSystemComponent->SetIsReplicated(true);
}

UAbilitySystemComponent* AMyCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AMyCharacter::Respawn()
{
	FGameplayAbilitySpec* Spec = AbilitySystemComponent->FindAbilitySpecFromClass(UMyAB_Death_ServerInitiated::StaticClass());
	AbilitySystemComponent->CancelAbilityHandle(Spec->Handle);
	AbilitySystemComponent->RemoveLooseGameplayTag(FGameplayTag::RequestGameplayTag(TEXT("GAS.Character.Dead")));
	
	SetActorLocation(RespawnPosition);
	InitializeAttributeSet();
	
	UAnimInstance* Anim = GetMesh()->GetAnimInstance();
	Anim->Montage_Stop(0.0f);
}

void AMyCharacter::BeginPlay()
{
	Super::BeginPlay();
	GetMesh()->VisibilityBasedAnimTickOption = EVisibilityBasedAnimTickOption::AlwaysTickPoseAndRefreshBones;
	RespawnPosition = GetActorLocation();
	
	if (HasAuthority() == false)
	{
		MyBarComponent->SetWidgetClass(MyBarWidgetClass);
		MyBarComponent->SetVisibility(true);
		//MyBarComponent->InitWidget();
	}
}

void AMyCharacter::SetAttributeValue(EAttributeType type, int Value)
{
	FGameplayEffectSpecHandle Spec = AbilitySystemComponent->MakeOutgoingSpec(UMyGE_SetAttr::StaticClass(), 1.f, AbilitySystemComponent->MakeEffectContext());
	Spec.Data->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(TEXT("Attribute.Index")), (float)type);
	Spec.Data->SetSetByCallerMagnitude(FGameplayTag::RequestGameplayTag(TEXT("Attribute.Value")),Value);
	AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*Spec.Data.Get());
}

void AMyCharacter::InitializeAttributeSet()
{
	UMyConfigInstance* MyConfigInstance = GetGameInstance<UMyConfigInstance>();
	FMyConfigItem* Config = MyConfigInstance->MyCharacterConfig->AttributeTable.Find(SelfType);
	check(Config);

	for (auto& Item : Config->Attributes)
	{
		SetAttributeValue(Item.Key, Item.Value);
	}
}

void AMyCharacter::InitializeSkill()
{
	UMyConfigInstance* MyConfigInstance = GetGameInstance<UMyConfigInstance>();
	FMySkillItem* Config = MyConfigInstance->MyCharacterConfig->SkillTable.Find(SelfType);
	check(Config);
	
	for (auto& Item : Config->Skills)
	{
		GiveGameplayAbility(Item);
	}
	
	FGameplayAbilitySpec BeHitSpec(UMyAB_BeHit_ServerInitiated::StaticClass());
	AbilitySystemComponent->GiveAbility(BeHitSpec);
	
	FGameplayAbilitySpec DeathSpec(UMyAB_Death_ServerInitiated::StaticClass());
	AbilitySystemComponent->GiveAbility(DeathSpec);
}

void AMyCharacter::GiveGameplayAbility(FGameplayTag SkillTag)
{
	UMyConfigInstance* MyConfigInstance = GetGameInstance<UMyConfigInstance>();
	FSkillConfigItem *Item = MyConfigInstance->MySkillConfig->Skills.Find(SkillTag);
	check(Item);
	
	FGameplayAbilitySpec Spec(Item->GameplayAbility, 1);
	Spec.GetDynamicSpecSourceTags().AddTag(SkillTag);
	AbilitySystemComponent->GiveAbility(Spec);
}

bool AMyCharacter::TryBindBarWidget(UAbilitySystemComponent *ASC)
{
	if (ASC == nullptr || MyBarComponent == nullptr)
		return false;
	
	UMyBarUserWidget* MyBarWidget = Cast<UMyBarUserWidget>(MyBarComponent->GetUserWidgetObject());
	if (MyBarWidget == nullptr)
		return false;
	
	ASC->GetGameplayAttributeValueChangeDelegate(UMyAttributeSet::GetHpAttribute()).AddUObject(MyBarWidget, &UMyBarUserWidget::OnHpChanged);
	ASC->GetGameplayAttributeValueChangeDelegate(UMyAttributeSet::GetMpAttribute()).AddUObject(MyBarWidget, &UMyBarUserWidget::OnMpChanged);
	ASC->GetGameplayAttributeValueChangeDelegate(UMyAttributeSet::GetMaxHpAttribute()).AddUObject(MyBarWidget, &UMyBarUserWidget::OnMaxHpChanged);
	ASC->GetGameplayAttributeValueChangeDelegate(UMyAttributeSet::GetMaxMpAttribute()).AddUObject(MyBarWidget, &UMyBarUserWidget::OnMaxMpChanged);
	
	MyBarWidget->Hp = ASC->GetNumericAttribute(UMyAttributeSet::GetHpAttribute());
	MyBarWidget->MaxHp = ASC->GetNumericAttribute(UMyAttributeSet::GetMaxHpAttribute());
	MyBarWidget->Mp = ASC->GetNumericAttribute(UMyAttributeSet::GetMpAttribute());
	MyBarWidget->MaxMp = ASC->GetNumericAttribute(UMyAttributeSet::GetMaxMpAttribute());
	
	MyBarWidget->RefreshBarPercent();
	return true;
}

void AMyCharacter::PlayHitStop(float Duration)
{
	TWeakObjectPtr<USkeletalMeshComponent> MesPtr = GetMesh();
	if (MesPtr.IsValid() == false)
		return;
	
	MesPtr->GlobalAnimRateScale = 0.f;
	
	FTimerHandle TimerHandle;
	GetWorldTimerManager().SetTimer
	(
		TimerHandle,
		[MesPtr]()
		{
			if (MesPtr.IsValid() == true)
				MesPtr->GlobalAnimRateScale = 1.f;
		},
		Duration,
		false
	);
}

