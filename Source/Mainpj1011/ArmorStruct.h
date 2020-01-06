// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ArmorStruct.generated.h"

UENUM(BlueprintType)
enum class EArmorType : uint8 //아이템 타입
{
	Head,
	Chest,
	Shoulders,
	Legs,
	Hands,
	Foots,
	None,
};
UENUM(BlueprintType)
enum class EArmorRank : uint8 // 아이템 등급
{
	Common, //흰색
	UnCommon, //녹색
	Rare, //파란색
	Epic, //보라색
	Legendary, //노란색
	None,
};
UENUM(BlueprintType)
enum class EBrand : uint8 //브랜드 이름
{
	GilagardGroup, // 방어특화 브랜드
	FenrirGroup, // 무기파워특화 브랜드
	AlpsGroup, //스킬파워 특화 브랜드
	None,
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
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	EArmorType ArmorType = EArmorType::None;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FName ArmorName  = FName();
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float WeaponPower = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SkillPower = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)	
	float Health = 0.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString MeshRefText = "";
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EArmorRank Rank = EArmorRank::None;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EBrand Brand = EBrand::None;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float KeepWarm = 0.0f;
};

bool operator==(const FArmorPropertyStruct& Property1, const FArmorPropertyStruct& Property2);
