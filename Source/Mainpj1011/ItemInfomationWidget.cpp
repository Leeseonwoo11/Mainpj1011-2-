// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemInfomationWidget.h"
#include "Components/TextBlock.h"
#include "Components/Border.h"

void UItemInfomationWidget::bindArmor(ASpawningArmor* NewArmor)
{
	if (NewArmor == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("NewArmor is nullptr"));
		return;
	}
	UE_LOG(LogTemp, Error, TEXT("NewArmor is not nullptr"));
	SpawningArmor = NewArmor;
	UpdateWidget();
}

void UItemInfomationWidget::NativeConstruct()
{
	BrandName = Cast<UTextBlock>(GetWidgetFromName(TEXT("BrandName")));
	TypeName = Cast<UTextBlock>(GetWidgetFromName(TEXT("TypeName")));
	Border = Cast<UBorder>(GetWidgetFromName(TEXT("Border_155")));
	if (BrandName == nullptr)
	{
		UE_LOG(LogTexture, Error, TEXT("ItemName is nullptr"));
	}
	if (TypeName == nullptr)
	{
		UE_LOG(LogTexture, Error, TEXT("TypeName is nullptr"));
	}
	if (Border == nullptr)
	{
		UE_LOG(LogTexture, Error, TEXT("Border is nullptr"));
	}
}

void UItemInfomationWidget::UpdateWidget()
{
	BrandName->SetText(FText::FromString(SpawningArmor->BrandName));
	TypeName->SetText(FText::FromString(SpawningArmor->TypeName));
	switch (SpawningArmor->ArmorProperty.Rank)
	{
	case ERank::Common:
		Border->SetBrushColor(FLinearColor(255.0f, 255.0f, 255.0f, 0.5f));//Èò»ö
		break;
	case ERank::UnCommon:
		Border->SetBrushColor(FLinearColor(0.0f, 255.0f, 0.0f, 0.5f));//ÃÊ·Ï
		break;
	case ERank::Rare:
		Border->SetBrushColor(FLinearColor(0.0f, 255.0f, 255.0f, 0.5f));//½Ã¾È
		break;
	case ERank::Epic:
		Border->SetBrushColor(FLinearColor(255.0f,0.0f,255.0f, 0.5f));//¸¶Á¨Å¸
		break;
	case ERank::Legendary:
		Border->SetBrushColor(FLinearColor(255.0f, 255.0f, 0.0f,0.5f)); //³ë¶û
		break;
	default:
		break;
	}
	
	//-------------------------------------------------------------
}

