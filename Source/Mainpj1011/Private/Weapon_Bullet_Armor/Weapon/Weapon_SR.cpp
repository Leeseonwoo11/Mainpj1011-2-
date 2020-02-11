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
	WeaponType = EWeaponType::SR; //  무기타입설정
	FirePos->SetRelativeLocation(FVector(0.0f, 80.0f, 14.0f));// 총알 발사 위치 재설정
	FirePos->SetRelativeRotation(FRotator(0, 90, 0)); // 총알 발사 위치 회전값 재설정
}
void AWeapon_SR::BeginPlay()
{
	TableManager* TableMgr = TableManager::GetInstance(); // 무기테이블에서 해당하는 값들 불러와 설정
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

void AWeapon_SR::SetScopeMode()// 스나이퍼의 경우 스코프가 있어서 스코프 사격시 총알 발시위치는 총구에서 카메라위치로 수정된다.
{
	ATPSCharacter* TempCharacter = Cast<ATPSCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	FirePos->SetWorldLocation(TempCharacter->Camera->GetComponentLocation());
}

void AWeapon_SR::SetAimMode() // 총알발사위치를 원래 총구위치로 바꿔준다.
{
	FirePos->SetRelativeLocation(FVector(0.0f, 80.0f, 14.0f));
}
