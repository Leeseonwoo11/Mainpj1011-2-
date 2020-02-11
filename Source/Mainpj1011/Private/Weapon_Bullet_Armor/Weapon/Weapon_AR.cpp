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

	WeaponBody->SetWorldScale3D(FVector(1.0f, 1.0f, 1.0f)); // ���� �޽��� ������ 1,1,1
	WeaponType = EWeaponType::AR; // ������ Ÿ�Լ���
	FirePos->SetRelativeLocation(FVector(0.0f, 75.0f, 13.0f));// �Ѿ˹߻���ġ �缳��
	FirePos->SetRelativeRotation(FRotator(0, 90, 0)); //  �Ѿ� �߻� ��ġ ȸ���� �缳��
}

void AWeapon_AR::BeginPlay()
{
	Super::BeginPlay();
	
	TableManager* TableMgr = TableManager::GetInstance(); // ���̺�Ŵ������� ���� ������ �Ӽ����� �޾Ƽ� �������ش�.
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