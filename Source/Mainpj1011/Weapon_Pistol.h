// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "Weapon.h"
#include "Weapon_Pistol.generated.h"

/**
 * 
 */
UCLASS()
class MAINPJ1011_API AWeapon_Pistol : public AWeapon
{
	GENERATED_BODY()

public:
	AWeapon_Pistol();
protected:
	virtual void BeginPlay() override;
};
