// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

UENUM()
enum class EWeaponType
{
	NoWeapon,
	PT,
	AR,
	SMG,
	LMG,
	SG,
	SR,
};

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
	USceneComponent* FirePos;
	UPROPERTY(EditAnywhere)
	int32 Damage;
	UPROPERTY(EditAnywhere)
	float RPM = 1;


	EWeaponType WeaponType;
};
