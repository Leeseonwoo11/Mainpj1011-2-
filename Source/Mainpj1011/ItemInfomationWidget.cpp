// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemInfomationWidget.h"
#include "Components/TextBlock.h"
#include "Components/Border.h"
#include "SpawningArmor.h"
#include "SpawningWeapon.h"
#include "ArmorStruct.h"
#include "WeaponStruct.h"

void UItemInfomationWidget::BindArmor(ASpawningArmor* NewArmor)
{
	if (NewArmor == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("NewArmor is nullptr"));
		return;
	}
	UE_LOG(LogTemp, Error, TEXT("NewArmor is not nullptr"));
	SpawningArmor = NewArmor;
	UpdateArmorWidget();
}

void UItemInfomationWidget::BindWeapon(ASpawningWeapon * NewWeapon)
{
	if (NewWeapon == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("NewWeapon is nullptr"));
		return;
	}
	UE_LOG(LogTemp, Error, TEXT("NewWeapon is not nullptr"));
	SpawningWeapon = NewWeapon;
	UpadateWeaponWidget();
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

void UItemInfomationWidget::UpdateArmorWidget()
{
	BrandName->SetText(FText::FromString(SpawningArmor->BrandName));
	TypeName->SetText(FText::FromString(SpawningArmor->TypeName));
	switch (SpawningArmor->ArmorProperty.Rank)
	{
	case EArmorRank::Common:
		Border->SetBrushColor(FLinearColor(255.0f, 255.0f, 255.0f, 0.5f));//Èò»ö
		break;
	case EArmorRank::UnCommon:
		Border->SetBrushColor(FLinearColor(0.0f, 255.0f, 0.0f, 0.5f));//ÃÊ·Ï
		break;
	case EArmorRank::Rare:
		Border->SetBrushColor(FLinearColor(0.0f, 255.0f, 255.0f, 0.5f));//½Ã¾È
		break;
	case EArmorRank::Epic:
		Border->SetBrushColor(FLinearColor(255.0f,0.0f,255.0f, 0.5f));//¸¶Á¨Å¸
		break;
	case EArmorRank::Legendary:
		Border->SetBrushColor(FLinearColor(255.0f, 255.0f, 0.0f,0.5f)); //³ë¶û
		break;
	default:
		break;
	}
	
	//-------------------------------------------------------------
}

void UItemInfomationWidget::UpadateWeaponWidget()
{
	BrandName->SetText(FText::FromString(SpawningWeapon->FullName));
	switch (SpawningWeapon->WeaponProperty.Rank)
	{
	case EWeaponRank::Common:
		Border->SetBrushColor(FLinearColor(255.0f, 255.0f, 255.0f, 0.5f));//Èò»ö
		break;
	case EWeaponRank::UnCommon:
		Border->SetBrushColor(FLinearColor(0.0f, 255.0f, 0.0f, 0.5f));//ÃÊ·Ï
		break;
	case EWeaponRank::Rare:
		Border->SetBrushColor(FLinearColor(0.0f, 255.0f, 255.0f, 0.5f));//½Ã¾È
		break;
	case EWeaponRank::Epic:
		Border->SetBrushColor(FLinearColor(255.0f, 0.0f, 255.0f, 0.5f));//¸¶Á¨Å¸
		break;
	case EWeaponRank::Legendary:
		Border->SetBrushColor(FLinearColor(255.0f, 255.0f, 0.0f, 0.5f)); //³ë¶û
		break;
	default:
		break;
	}
}

