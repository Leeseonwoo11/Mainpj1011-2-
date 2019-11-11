// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon_AR.h"

AWeapon_AR::AWeapon_AR()
{
	ConstructorHelpers::FObjectFinder<USkeletalMesh>SK_BODY(TEXT("/Game/MilitaryWeapDark/Weapons/Assault_Rifle_B"));
	if (SK_BODY.Succeeded())
	{
		WeaponBody->SetSkeletalMesh(SK_BODY.Object);
	}
	WeaponBody->SetWorldScale3D(FVector(1.0f, 1.0f, 1.0f));
	WeaponType = EWeaponType::AR;
	FirePos->SetRelativeLocation(FVector(0.0f, 75.0f, 13.0f));
	FirePos->SetRelativeRotation(FRotator(0, 90, 0));
	RPM = 600.0f;
}