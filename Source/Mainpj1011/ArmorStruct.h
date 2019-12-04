// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ArmorStruct.generated.h"

UENUM()
enum class EArmorType //아이템 타입
{
	Head,
	Chest,
	Shoulders,
	Legs,
	Hands,
	Foots,
};
UENUM()
enum class ERank // 아이템 등급
{
	Common, //흰색
	UnCommon, //녹색
	Rare, //파란색
	Epic, //보라색
	Legendary, //노란색
};
UENUM()
enum class EBrand //브랜드 이름
{
	GilagadGroup, // 방어특화 브랜드
	FenrirGroup, // 무기파워특화 브랜드
	AlpsGroup, //스킬파워 특화 브랜드
};

UCLASS()
class MAINPJ1011_API AArmorStruct : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AArmorStruct();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};

USTRUCT(Atomic,BlueprintType)
struct FArmorPropertyStruct
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere)
	EArmorType ArmorType;
	UPROPERTY(EditAnywhere)
	FName ArmorName;
	UPROPERTY(EditAnywhere)
	float WeaponPower;
	UPROPERTY(EditAnywhere)
	float SkillPower;
	UPROPERTY(EditAnywhere)
	float Health;
	UPROPERTY(EditAnywhere)
	FString MeshRefText;
	UPROPERTY(EditAnywhere)
	ERank Rank;
	UPROPERTY(EditAnywhere)
	EBrand Brand;
};

