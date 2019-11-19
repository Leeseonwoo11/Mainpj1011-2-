// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "TableManager.h"

// Sets default values
AWeapon::AWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	WeaponBody = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WEAPONBODY"));
	FirePos = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FIREPOS"));
	FirePos->SetupAttachment(WeaponBody);
	WeaponType = EWeaponType::NoWeapon;
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

