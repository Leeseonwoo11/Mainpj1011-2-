// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponStruct.h"

// Sets default values
AWeaponStruct::AWeaponStruct()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AWeaponStruct::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWeaponStruct::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool operator==(const FWeaponPropertyStruct& Property1, const FWeaponPropertyStruct& Property2)
{
	if (Property1.MeshRefText == Property2.MeshRefText)
	{
		if (Property1.Rank == Property2.Rank)
		{
			if (Property1.WeaponName == Property2.WeaponName)
			{
				if (Property1.WeaponType == Property2.WeaponType)
				{
					return true;
				}
			}
		}
	}
	else if (&Property1 == &Property1)
	{
		return true;
	}
	return false;
}
