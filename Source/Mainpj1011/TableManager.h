// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "EngineMinimal.h"
#include "WeaponTable.h"
#include "Weapon.h"
/**
 * 
 */
class MAINPJ1011_API TableManager
{
private:
	TableManager() {};
	~TableManager() {};

	static TableManager* Instance;
public:
	static TableManager* GetInstance()
	{
		if (Instance == nullptr)
		{
			Instance = new TableManager();

			Instance->SetDataTable(TEXT("/Game/MyNew/WeaponTable/WeaponTable"));
			return Instance;
		}
		else
		{
			return Instance;
		}
	}
private:
	class UDataTable* WeaponDataTable;
public:
	int32 GetWeaponAMMO(EWeaponType Type)
	{
		int32 WeaponNum = 0;
		switch (Type)
		{
		case EWeaponType::AR:
			WeaponNum = 1;
			break;
		case EWeaponType::PT:
			WeaponNum = 3;
			break;
		case EWeaponType::SR:
			WeaponNum = 2;
			break;
		case EWeaponType::NoWeapon:
			WeaponNum = 4;
			break;
		default:
			WeaponNum = 4;
			break;
		}
		if (WeaponDataTable != nullptr)
		{
			FWeaponTable* WeaponTable = WeaponDataTable->FindRow<FWeaponTable>(FName(*(FString::FormatAsNumber(WeaponNum))), FString(""));
			if (WeaponTable != nullptr)
			{
				return WeaponTable->nAMMO;
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("WeaponTable is nullptr"))
				return 0;
			}
		}
		else
			return 0;
	}
	int32 GetWeaponRPM(EWeaponType Type)
	{
		int32 WeaponNum = 0;
		switch (Type)
		{
		case EWeaponType::AR:
			WeaponNum =1;
			break;
		case EWeaponType::PT:
			WeaponNum = 3;
			break;
		case EWeaponType::SR:
			WeaponNum = 2;
			break;
		case EWeaponType::NoWeapon:
			WeaponNum = 4;
			break;
		default:
			WeaponNum = 4;
			break;
		}
		if (WeaponDataTable != nullptr)
		{
			FWeaponTable* WeaponTable = WeaponDataTable->FindRow<FWeaponTable>(FName(*(FString::FormatAsNumber(WeaponNum))), FString(""));
			if (WeaponTable != nullptr)
			{
				return WeaponTable->nRPM;
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("WeaponTable is nullptr"))
					return 0;
			}
		}
		else
			return 0;
	}

	float GetWeaponDamage(EWeaponType Type)
	{
		int32 WeaponNum = 0;
		switch (Type)
		{
		case EWeaponType::AR:
			WeaponNum = 1;
			break;
		case EWeaponType::PT:
			WeaponNum = 3;
			break;
		case EWeaponType::SR:
			WeaponNum = 2;
			break;
		case EWeaponType::NoWeapon:
			WeaponNum = 4;
			break;
		default:
			WeaponNum = 4;
			break;
		}
		if (WeaponDataTable != nullptr)
		{
			FWeaponTable* WeaponTable = WeaponDataTable->FindRow<FWeaponTable>(FName(*(FString::FormatAsNumber(WeaponNum))), FString(""));
			if (WeaponTable != nullptr)
			{
				return WeaponTable->fDAMAGE;
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("WeaponTable is nullptr"))
					return 0.0f;
			}
		}
		else
			return 0;
	}
	int32 GetWeaponBalance(EWeaponType Type)
	{
		int32 WeaponNum = 0;
		switch (Type)
		{
		case EWeaponType::AR:
			WeaponNum = 1;
			break;
		case EWeaponType::PT:
			WeaponNum = 3;
			break;
		case EWeaponType::SR:
			WeaponNum = 2;
			break;
		case EWeaponType::NoWeapon:
			WeaponNum = 4;
			break;
		default:
			WeaponNum = 4;
			break;
		}
		if (WeaponDataTable != nullptr)
		{
			FWeaponTable* WeaponTable = WeaponDataTable->FindRow<FWeaponTable>(FName(*(FString::FormatAsNumber(WeaponNum))), FString(""));
			if (WeaponTable != nullptr)
			{
				return WeaponTable->nBALANCE;
			}
			else
			{
				UE_LOG(LogTemp, Error, TEXT("WeaponTable is nullptr"))
					return 0;
			}
		}
		else
			return 0;
	}

	void SetDataTable(const TCHAR* ContentPath) //캐릭터 레벨 데이터 테이블 셋
	{
		UDataTable* WeaponDT = Cast<UDataTable>(StaticLoadObject(UDataTable::StaticClass(), NULL, ContentPath));
		if (WeaponDT)
		{
			WeaponDataTable = WeaponDT;
		}
	}
};

TableManager* TableManager::Instance = nullptr;

