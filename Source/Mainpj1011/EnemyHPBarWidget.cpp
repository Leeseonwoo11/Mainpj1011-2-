 // Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyHPBarWidget.h"
#include "TPSEnemyStatComponent.h"
#include "Components/ProgressBar.h"

void UEnemyHPBarWidget::BindEnemyHP(UTPSEnemyStatComponent* NewStatComponent)
{
	if (NewStatComponent == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("NewStatComponent is nullptr"));
		return;
	}
	UE_LOG(LogTemp, Error, TEXT("NewStatComponent is not nullptr"));

	CurrentStatComponent = NewStatComponent;
	NewStatComponent->HPChangeDel.AddUObject(this, &UEnemyHPBarWidget::UpdateHPWidget);
}

void UEnemyHPBarWidget::NativeConstruct()
{
	Super::NativeConstruct();
	HPProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("ProgressBar_92")));
	if (HPProgressBar == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Progressbar is nullptr"));
		return;
	}
	UpdateHPWidget();
}

void UEnemyHPBarWidget::UpdateHPWidget()
{
w	if (CurrentStatComponent.IsValid())
	{
		if (HPProgressBar != nullptr)
		{
			HPProgressBar->SetPercent(CurrentStatComponent->GetHPRatio());
			UE_LOG(LogTemp, Error, TEXT("HPRatio = %f"), CurrentStatComponent->GetHPRatio());
		}
	}
}
