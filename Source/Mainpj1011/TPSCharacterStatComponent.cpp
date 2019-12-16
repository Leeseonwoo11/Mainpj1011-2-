// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSCharacterStatComponent.h"
#include "EquipmentComponent.h"
#include "TPSCharacter.h"

// Sets default values for this component's properties
UTPSCharacterStatComponent::UTPSCharacterStatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	// ...
}


// Called when the game starts
void UTPSCharacterStatComponent::BeginPlay()
{
	Super::BeginPlay();
	TempTPSCharacter = Cast<ATPSCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(),0));
}


// Called every frame
void UTPSCharacterStatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	// ...

}

void UTPSCharacterStatComponent::SetDamage(float Damage)
{
	if (Damage > 0)
	{
		if (PlayerHealth > 0)
		{
			PlayerHealth -= Damage;
		}
	}
}

float UTPSCharacterStatComponent::GetHPRatio()
{
	return PlayerHealth / 100.0f;
}

void UTPSCharacterStatComponent::SetEquipmentAbility()
{
	if (TempTPSCharacter != nullptr)
	{
		auto EquipComp = TempTPSCharacter->EquipmentComponent;
		PlayerAttackPower = EquipComp->EquipHead.WeaponPower+ EquipComp->EquipChest.WeaponPower+ EquipComp->EquipFoots.WeaponPower+ EquipComp->EquipHands.WeaponPower+ EquipComp->EquipLegs.WeaponPower+ EquipComp->EquipShoulders.WeaponPower;
		PlayerSkillPower = EquipComp->EquipHead.SkillPower + EquipComp->EquipChest.SkillPower + EquipComp->EquipFoots.SkillPower + EquipComp->EquipHands.SkillPower + EquipComp->EquipLegs.SkillPower + EquipComp->EquipShoulders.SkillPower;
		PlayerHealth = EquipComp->EquipHead.Health + EquipComp->EquipChest.Health + EquipComp->EquipFoots.Health + EquipComp->EquipHands.Health + EquipComp->EquipLegs.Health + EquipComp->EquipShoulders.Health+ 100; // 100은 기본값
	}
}


