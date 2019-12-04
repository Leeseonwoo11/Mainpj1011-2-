// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ArmorStruct.generated.h"

UENUM()
enum class EArmorType //������ Ÿ��
{
	Head,
	Chest,
	Shoulders,
	Legs,
	Hands,
	Foots,
};
UENUM()
enum class ERank // ������ ���
{
	Common, //���
	UnCommon, //���
	Rare, //�Ķ���
	Epic, //�����
	Legendary, //�����
};
UENUM()
enum class EBrand //�귣�� �̸�
{
	GilagadGroup, // ���Ưȭ �귣��
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

