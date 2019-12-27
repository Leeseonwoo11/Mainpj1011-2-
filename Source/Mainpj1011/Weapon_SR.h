// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "Weapon_SR.generated.h"

/**
 * 
 */
UCLASS()
class MAINPJ1011_API AWeapon_SR : public AWeapon
{
	GENERATED_BODY()
	AWeapon_SR();
protected:
	virtual void BeginPlay() override;
public:
	UFUNCTION(BlueprintCallable)
	void SetScopeMode();
	UFUNCTION(BlueprintCallable)
	void SetAimMode();
};
