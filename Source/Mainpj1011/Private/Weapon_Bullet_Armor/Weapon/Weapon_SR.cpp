// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon_SR.h"
#include "TableManager.h"
#include "TPSCharacter.h"

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
void AWeapon_SR::BeginPlay()
{
	TableManager* TableMgr = TableManager::GetInstance();
	Damage = TableMgr->GetWeaponDamage(WeaponType);
	RPM = TableMgr->GetWeaponRPM(WeaponType);
	AMMO = TableMgr->GetWeaponAMMO(WeaponType);
	Total_AMMO = TableMgr->GetTotal_AMMO(WeaponType);
	Balance = TableMgr->GetWeaponBalance(WeaponType);
	UE_LOG(LogTemp, Error, TEXT("SR RPM is %d"), RPM);
	UE_LOG(LogTemp, Error, TEXT("SR DMG is %f"), Damage);
	UE_LOG(LogTemp, Error, TEXT("SR AMMO is %d"), AMMO);
	UE_LOG(LogTemp, Error, TEXT("SR Total AMMO is %d"), Total_AMMO);

}

void AWeapon_SR::SetScopeMode()
{
	ATPSCharacter* TempCharacter = Cast<ATPSCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	FirePos->SetWorldLocation(TempCharacter->Camera->GetComponentLocation());
}

void AWeapon_SR::SetAimMode()
{
	FirePos->SetRelativeLocation(FVector(0.0f, 80.0f, 14.0f));
}
