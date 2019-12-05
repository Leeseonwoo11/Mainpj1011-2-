// Fill out your copyright notice in the Description page of Project Settings.


#include "ArmorStruct.h"

// Sets default values
AArmorStruct::AArmorStruct()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AArmorStruct::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AArmorStruct::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool operator==(const FArmorPropertyStruct & Property1, const FArmorPropertyStruct & Property2)
{
	if (Property1.ArmorName == Property2.ArmorName)
	{
		if (Property1.ArmorType == Property2.ArmorType)
		{
			if (Property1.Brand == Property2.Brand)
			{
				if (Property1.Rank == Property2.Rank)
				{
					if ((Property1.SkillPower - Property2.SkillPower) < 0.01f && (Property1.SkillPower - Property2.SkillPower) > -0.01f)
					{
						if((Property1.WeaponPower - Property2.WeaponPower) < 0.01f && (Property1.WeaponPower - Property2.WeaponPower) > -0.01f)
						{
							if ((Property1.Health - Property2.Health) < 0.01f && (Property1.Health - Property2.Health) > -0.01f)
							{
								return true;
							}
						}
					}
				}
			}
		}
	}
	return false;
}
