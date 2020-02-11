// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "Weapon.h"
#include "Weapon_AR.generated.h"

/**
 * 
 */
// 테이블매니져에서 무기의 속성들을 받아와서 설정한다.
UCLASS()
class MAINPJ1011_API AWeapon_AR : public AWeapon
{
	GENERATED_BODY()
	AWeapon_AR();
protected:
	virtual void BeginPlay() override;
};
