// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponStruct.generated.h"

UENUM(BlueprintType)
enum class EWeaponRank : uint8 // 아이템 등급
{
	Common, //흰색
	UnCommon, //녹색
	Rare, //파란색
	Epic, //보라색
	Legendary, //노란색
	None,
};
UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	NoWeapon,
	PT,
	AR,
	SR,
};

UCLASS()
class MAINPJ1011_API AWeaponStruct : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeaponStruct();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};


USTRUCT(Atomic, BlueprintType)
struct FWeaponPropertyStruct
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EWeaponType WeaponType = EWeaponType::NoWeapon;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName WeaponName = FName();
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString MeshRefText = "";
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EWeaponRank Rank = EWeaponRank::None;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float AdditionalDamage = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 AdditionalRPM = 0;
};

bool operator==(const FWeaponPropertyStruct& Property1, const FWeaponPropertyStruct& Property2);


