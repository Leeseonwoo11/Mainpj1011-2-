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
	HPProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("ProgressBar_92"))); //미리 만든 블루프린트 위젯의 ProgresBar이름이 ProgressBar_92이므로 캐스트 해준다.
	if (HPProgressBar == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Progressbar is nullptr"));
		return;
	}
	UpdateHPWidget();
}

void UEnemyHPBarWidget::UpdateHPWidget()
{
	if (CurrentStatComponent.IsValid())
	{
		if (HPProgressBar != nullptr)
		{
			HPProgressBar->SetPercent(CurrentStatComponent->GetHPRatio()); //프로그래스 바에 HP비율을 설정한다.
			UE_LOG(LogTemp, Error, TEXT("HPRatio = %f"), CurrentStatComponent->GetHPRatio());
		}
	}
}
