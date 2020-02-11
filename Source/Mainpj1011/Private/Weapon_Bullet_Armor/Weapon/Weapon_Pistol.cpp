// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon_Pistol.h"
#include "TableManager.h"

AWeapon_Pistol::AWeapon_Pistol()
{
	ConstructorHelpers::FObjectFinder<USkeletalMesh>SK_BODY(TEXT("/Game/MilitaryWeapDark/Weapons/Pistols_B"));
	if (SK_BODY.Succeeded())
	{
		WeaponBody->SetSkeletalMesh(SK_BODY.Object);
	}
	WeaponBody->SetWorldScale3D(FVector(1.2f, 1.2f, 1.2f));// 무기가 작아서 잘보이게 스케일을 키워줌
	WeaponType = EWeaponType::PT; //  무기의 타입 설정
	FirePos->SetRelativeLocation(FVector(0.0f, 30.0f, 10.0f)); // 무기 발사위치 재설정 (총구)
	FirePos->SetRelativeRotation(FRotator(0, 90, 0)); //  무기의 발사위치 회전값 재설정
}

void AWeapon_Pistol::BeginPlay()
{	
	TableManager* TableMgr = TableManager::GetInstance();
	if (TableMgr != nullptr)
	{
		Damage = TableMgr->GetWeaponDamage(WeaponType);
		RPM = TableMgr->GetWeaponRPM(WeaponType);
		AMMO = TableMgr->GetWeaponAMMO(WeaponType);
		Total_AMMO = TableMgr->GetTotal_AMMO(WeaponType);
		Balance = TableMgr->GetWeaponBalance(WeaponType);
		UE_LOG(LogTemp, Error, TEXT("Pistol RPM is %d"), RPM);
		UE_LOG(LogTemp, Error, TEXT("Pistol DMG is %f"), Damage);
		UE_LOG(LogTemp, Error, TEXT("Pistol AMMO is %d"), AMMO);
		UE_LOG(LogTemp, Error, TEXT("Pistol TotalAMMO is %d"), Total_AMMO);
	}
}
