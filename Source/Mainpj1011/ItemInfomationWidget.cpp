// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemInfomationWidget.h"
#include "Components/TextBlock.h"
#include "Components/Border.h"
//-------------------------------------------------------------------
void UItemInfomationWidget::bindArmor(ASpawningArmor* NewArmor)
{
	if (NewArmor == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("NewArmor is nullptr"));
		return;
	}
	UE_LOG(LogTemp, Error, TEXT("NewArmor is not nullptr"));
	SpawningArmor = NewArmor;
}


void UItemInfomationWidget::NativeConstruct()
{
	ItemName = Cast<UTextBlock>(GetWidgetFromName(TEXT("ItemName")));
	if (ItemName == nullptr)
	{
		UE_LOG(LogTexture, Error, TEXT("ItemName is nullptr"));
	}
}

void UItemInfomationWidget::UpdateWidget()
{/*
	ItemName->SetText(FText::FromName(Item.ArmorName));
	switch (Item.Rank)
	{
	case ERank::Common:
		Border->SetBrushColor(FColor(0, 0, 0));//���
		break;
	case ERank::UnCommon:
		Border->SetBrushColor(FColor(0, 1, 0));//�ʷ�
		break;
	case ERank::Rare:
		Border->SetBrushColor(FColor(0, 1, 1));//�þ�
		break;
	case ERank::Epic:
		Border->SetBrushColor(FColor(1, 0, 1));//����Ÿ
		break;
	case ERank::Legendary:
		Border->SetBrushColor(FColor(1, 1, 0)); //���
		break;
	default:
		break;
	}
	*/
	//-------------------------------------------------------------
}

