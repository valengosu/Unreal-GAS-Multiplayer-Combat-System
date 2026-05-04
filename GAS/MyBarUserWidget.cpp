#include "MyBarUserWidget.h"
#include "MySkillSystem/MyAttributeSet.h"

void UMyBarUserWidget::OnHpChanged(const FOnAttributeChangeData& Data)
{	
	Hp = Data.NewValue;
	RefreshBarPercent();
	UE_LOG(LogTemp, Warning, TEXT("HP %d"), Hp);
}

void UMyBarUserWidget::OnMpChanged(const FOnAttributeChangeData& Data)
{
	Mp = Data.NewValue;
	RefreshBarPercent();
	UE_LOG(LogTemp, Warning, TEXT("MP %d"), Mp);
}

void UMyBarUserWidget::OnMaxHpChanged(const FOnAttributeChangeData& Data)
{
	MaxHp = Data.NewValue;
	RefreshBarPercent();
	UE_LOG(LogTemp, Warning, TEXT("MAXHP %d"), MaxHp);
}

void UMyBarUserWidget::OnMaxMpChanged(const FOnAttributeChangeData& Data)
{
	MaxMp = Data.NewValue;
	RefreshBarPercent();
	UE_LOG(LogTemp, Warning, TEXT("MaxMp %d"), MaxMp);
}

void UMyBarUserWidget::RefreshBarPercent()
{
	//UE_LOG(LogTemp, Error, TEXT("RefreshBarPercent at %f"), GetWorld()->GetTimeSeconds());
	
	HpBar->SetPercent(0);
	MpBar->SetPercent(0);
	
	if (MaxHp > 0)
		HpBar->SetPercent((float)Hp / (float)MaxHp);
	
	if (MaxMp > 0)
		MpBar->SetPercent((float)Mp / (float)MaxMp);
	
	UE_LOG(LogTemp, Warning, TEXT("MpP %f"), (float)Mp / (float)MaxHp);
}