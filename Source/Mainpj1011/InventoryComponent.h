// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "Components/ActorComponent.h"
#include "ArmorStruct.h"
#include "WeaponStruct.h"
#include "InventoryComponent.generated.h"


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
	int32 ArmorInventorySize = 100;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 WeaponInventorySize = 100;


	UFUNCTION()
	void AddInventroyArmor(FArmorPropertyStruct Armor);
	UFUNCTION()
	void RemoveInventoryArmor(FArmorPropertyStruct Armor);

	UFUNCTION()
	void AddInventroyWeapon(FWeaponPropertyStruct Weapon);
	UFUNCTION()
	void RemoveInventoryWeapon(FWeaponPropertyStruct Weapon);
};
