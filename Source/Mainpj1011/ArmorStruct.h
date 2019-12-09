// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ArmorStruct.generated.h"

UENUM(BlueprintType)
enum class EArmorType : uint8 //������ Ÿ��
{
	Head,
	Chest,
	Shoulders,
	Legs,
	Hands,
	Foots,
};
UENUM(BlueprintType)
enum class ERank : uint8 // ������ ���
{
	Common, //���
	UnCommon, //���
	Rare, //�Ķ���
	Epic, //�����
	Legendary, //�����
};
UENUM(BlueprintType)
enum class EBrand : uint8 //�귣�� �̸�
{
	GilagardGroup, // ���Ưȭ �귣��
	FenrirGroup, // �����Ŀ�Ưȭ �귣��
	AlpsGroup, //��ų�Ŀ� Ưȭ �귣��
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
	EArmorType ArmorType;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	FName ArmorName;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float WeaponPower;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SkillPower;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)	
	float Health;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString MeshRefText;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ERank Rank;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EBrand Brand;
};

bool operator==(const FArmorPropertyStruct& Property1, const FArmorPropertyStruct& Property2);
