// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon_Pistol.h"

AWeapon_Pistol::AWeapon_Pistol()
{
	ConstructorHelpers::FObjectFinder<USkeletalMesh>SK_BODY(TEXT("/Game/MilitaryWeapDark/Weapons/Pistols_B"));
	if (SK_BODY.Succeeded())
	{
		WeaponBody->SetSkeletalMesh(SK_BODY.Object);
	}
	WeaponBody->SetWorldScale3D(FVector(1.2f, 1.2f, 1.2f));
	WeaponType = EWeaponType::PT;
	FirePos->SetRelativeLocation(FVector(0.0f, 30.0f, 10.0f));
	FirePos->SetRelativeRotation(FRotator(0, 90, 0));
}

void AWeapon_Pistol::BeginPlay()
{
}
