// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "WeaponTable.generated.h"

// 무기테이블, 데이터 테이블을 만들기 위해서 설정함.

USTRUCT(BlueprintType)
struct FWeaponTable : public FTableRowBase
{
	GENERATED_BODY()
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	FString nID; // 1돌격소총,2스나이퍼라이플,3권총
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	int32 nAMMO;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	int32 nRPM;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float fDAMAGE;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	int32 nBALANCE;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	int32 nTotalAMMO;
};

