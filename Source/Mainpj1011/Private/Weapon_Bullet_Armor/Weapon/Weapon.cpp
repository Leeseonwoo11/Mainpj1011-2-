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
	MuzzleFlash = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleComponent"));
	MuzzleFlash->SetupAttachment(FirePos);
	static ConstructorHelpers::FObjectFinder<UParticleSystem>PS_FLASH(TEXT("/Game/MyNew/Particle/FireEffect/MuzzleFlash"));
	if (PS_FLASH.Succeeded())
	{
		MuzzleFlash->SetTemplate(PS_FLASH.Object);
	}
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	MuzzleFlash->SetActive(false);
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

