// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon_AR.h"
#include "TableManager.h"

AWeapon_AR::AWeapon_AR()
{
	ConstructorHelpers::FObjectFinder<USkeletalMesh>SK_BODY(TEXT("/Game/MilitaryWeapDark/Weapons/Assault_Rifle_B"));
	if (SK_BODY.Succeeded())
	{
		WeaponBody->SetSkeletalMesh(SK_BODY.Object);
	}

	WeaponBody->SetWorldScale3D(FVector(1.0f, 1.0f, 1.0f)); // 무기 메시의 스케일 1,1,1
	WeaponType = EWeaponType::AR; // 무기의 타입설정
	FirePos->SetRelativeLocation(FVector(0.0f, 75.0f, 13.0f));// 총알발사위치 재설정
	FirePos->SetRelativeRotation(FRotator(0, 90, 0)); //  총알 발사 위치 회전값 재설정
}

void AWeapon_AR::BeginPlay()
{
	Super::BeginPlay();
	
	TableManager* TableMgr = TableManager::GetInstance(); // 테이블매니져에서 무기 각각의 속성값을 받아서 지정해준다.
	Damage = TableMgr->GetWeaponDamage(WeaponType);
	RPM = TableMgr->GetWeaponRPM(WeaponType);
	AMMO = TableMgr->GetWeaponAMMO(WeaponType);
	Total_AMMO = TableMgr->GetTotal_AMMO(WeaponType);
	Balance = TableMgr->GetWeaponBalance(WeaponType);
	UE_LOG(LogTemp, Error, TEXT("AR RPM is %d"), RPM);
	UE_LOG(LogTemp, Error, TEXT("AR DMG is %f"), Damage);
	UE_LOG(LogTemp, Error, TEXT("AR AMMO is %d"), AMMO);
	UE_LOG(LogTemp, Error, TEXT("AR Total AMMO is %d"), Total_AMMO);

}