// Fill out your copyright notice in the Description page of Project Settings.


#include "InventoryComponent.h"

// Sets default values for this component's properties
UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UInventoryComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UInventoryComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UInventoryComponent::AddInventroyArmor(FArmorPropertyStruct Armor)
{
	if (ArmorInventory.Num() <= ArmorInventorySize)
	{
		ArmorInventory.Add(Armor);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Inventory is full!"));
	}
}

void UInventoryComponent::RemoveInventoryArmor(FArmorPropertyStruct Armor)
{
	for (auto InvenItem : ArmorInventory)
	{
		if (InvenItem == Armor)
		{
			UE_LOG(LogTexture, Error, TEXT("Armor is Same"));
			ArmorInventory.RemoveSingle(Armor);
		}
	}
}

void UInventoryComponent::AddInventroyWeapon(FWeaponPropertyStruct Weapon)
{
	if (WeaponInventory.Num() <= WeaponInventorySize)
	{
		WeaponInventory.Add(Weapon);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Inventory is full!"));
	}
}

void UInventoryComponent::RemoveInventoryWeapon(FWeaponPropertyStruct Weapon)
{
	for (auto InvenItem : WeaponInventory)
	{
		if (InvenItem == Weapon)
		{
			UE_LOG(LogTexture, Error, TEXT("Armor is Same"));
			WeaponInventory.RemoveSingle(Weapon);
		}
	}
}

