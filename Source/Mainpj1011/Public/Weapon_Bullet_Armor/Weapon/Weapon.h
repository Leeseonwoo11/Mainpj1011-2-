// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponStruct.h"
#include "Weapon.generated.h"

// 무기의 메시,총알수,파티클 등이 변수로 저장되어있다. 무기들은 이 액터를 상속받아서 사용한다.

UCLASS()
class MAINPJ1011_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USkeletalMeshComponent* WeaponBody;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* FirePos;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UParticleSystemComponent* MuzzleFlash;
	UPROPERTY(EditAnywhere)
	int32 AMMO = 1;
	UPROPERTY(EditAnywhere)
	int32 Total_AMMO = 1;
	UPROPERTY(EditAnywhere)
	int32 RPM = 1;
	UPROPERTY(EditAnywhere)
	float Damage = 1.0f;
	UPROPERTY(EditAnywhere)
	int32 Balance = 1;

	EWeaponType WeaponType;
};
