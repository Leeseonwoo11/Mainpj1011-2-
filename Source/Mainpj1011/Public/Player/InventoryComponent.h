// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "Components/ActorComponent.h"
#include "ArmorStruct.h"
#include "WeaponStruct.h"
#include "InventoryComponent.generated.h"

// 가지고있는 모든 아이템을 관리하는 컴포넌트이다.

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MAINPJ1011_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TArray<FArmorPropertyStruct> ArmorInventory;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FWeaponPropertyStruct> WeaponInventory;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ArmorInventorySize = 12;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 WeaponInventorySize = 6;


	UFUNCTION()
	bool AddInventroyArmor(FArmorPropertyStruct Armor); // 방어구 인벤토리 추가함수, 같은등급 같은부위 같은브랜드의 아이템이라면 추가되지 않는다.
	UFUNCTION(BlueprintCallable)
	void RemoveInventoryArmor(FArmorPropertyStruct Armor); // 방어구 인벤토리 삭제 함수

	UFUNCTION()
	bool AddInventroyWeapon(FWeaponPropertyStruct Weapon); //  무기 인벤토리 추가함수 같은무기 같은등급의 아이템이라면 추가되지않는다.
	UFUNCTION(BlueprintCallable)
	void RemoveInventoryWeapon(FWeaponPropertyStruct Weapon);// 무기 인벤토리 삭제함수
};
