// Fill out your copyright notice in the Description page of Project Settings.


#include "ItemInfomationWidget.h"
#include "Components/TextBlock.h"
#include "Components/Border.h"
#include "SpawningArmor.h"
#include "SpawningWeapon.h"
#include "ArmorStruct.h"
#include "WeaponStruct.h"

void UItemInfomationWidget::BindArmor(ASpawningArmor* NewArmor) //스폰방어구액터 포인터가 넘어옴
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

void UItemInfomationWidget::BindWeapon(ASpawningWeapon * NewWeapon) // 스폰무기액터 포인터가 넘어옴
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

void UItemInfomationWidget::UpdateArmorWidget()// 방어구 위젯 업데이트(색깔을 바꿈)
{
	BrandName->SetText(FText::FromString(SpawningArmor->BrandName));
	TypeName->SetText(FText::FromString(SpawningArmor->TypeName));
	switch (SpawningArmor->ArmorProperty.Rank)
	{
	case EArmorRank::Common:
		Border->SetBrushColor(FLinearColor(255.0f, 255.0f, 255.0f, 0.5f));//흰색
		break;
	case EArmorRank::UnCommon:
		Border->SetBrushColor(FLinearColor(0.0f, 255.0f, 0.0f, 0.5f));//초록
		break;
	case EArmorRank::Rare:
		Border->SetBrushColor(FLinearColor(0.0f, 255.0f, 255.0f, 0.5f));//시안
		break;
	case EArmorRank::Epic:
		Border->SetBrushColor(FLinearColor(255.0f,0.0f,255.0f, 0.5f));//마젠타
		break;
	case EArmorRank::Legendary:
		Border->SetBrushColor(FLinearColor(255.0f, 255.0f, 0.0f,0.5f)); //노랑
		break;
	default:
		break;
	}
	
	//-------------------------------------------------------------
}

void UItemInfomationWidget::UpadateWeaponWidget() // 무기 위젯 업데이트 (색깔을 바꿈)
{
	BrandName->SetText(FText::FromString(SpawningWeapon->FullName));
	switch (SpawningWeapon->WeaponProperty.Rank)
	{
	case EWeaponRank::Common:
		Border->SetBrushColor(FLinearColor(255.0f, 255.0f, 255.0f, 0.5f));//흰색
		break;
	case EWeaponRank::UnCommon:
		Border->SetBrushColor(FLinearColor(0.0f, 255.0f, 0.0f, 0.5f));//초록
		break;
	case EWeaponRank::Rare:
		Border->SetBrushColor(FLinearColor(0.0f, 255.0f, 255.0f, 0.5f));//시안
		break;
	case EWeaponRank::Epic:
		Border->SetBrushColor(FLinearColor(255.0f, 0.0f, 255.0f, 0.5f));//마젠타
		break;
	case EWeaponRank::Legendary:
		Border->SetBrushColor(FLinearColor(255.0f, 255.0f, 0.0f, 0.5f)); //노랑
		break;
	default:
		break;
	}
}

