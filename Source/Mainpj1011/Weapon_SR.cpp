// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon_SR.h"

AWeapon_SR::AWeapon_SR()
{
	ConstructorHelpers::FObjectFinder<USkeletalMesh>SK_BODY(TEXT("/Game/MilitaryWeapDark/Weapons/Sniper_Rifle_B"));
	if (SK_BODY.Succeeded())
	{
		WeaponBody->SetSkeletalMesh(SK_BODY.Object);
	}
	WeaponBody->SetWorldScale3D(FVector(1.0f, 1.0f, 1.0f));
	WeaponType = EWeaponType::SR;
	FirePos->SetRelativeLocation(FVector(0.0f, 80.0f, 14.0f));
	FirePos->SetRelativeRotation(FRotator(0, 90, 0));
}