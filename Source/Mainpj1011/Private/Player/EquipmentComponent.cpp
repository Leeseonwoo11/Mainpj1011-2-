// Fill out your copyright notice in the Description page of Project Settings.


#include "EquipmentComponent.h"
#include "TPSCharacter.h"
#include "TPSCharacterStatComponent.h"
#include "Weapon_AR.h"
#include "Weapon_SR.h"
#include "Weapon_Pistol.h"

// Sets default values for this component's properties
UEquipmentComponent::UEquipmentComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UEquipmentComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UEquipmentComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UEquipmentComponent::WearingArmor(FArmorPropertyStruct NewArmor)
{
	switch (NewArmor.ArmorType)
	{
	case EArmorType::Head :
		EquipHead = NewArmor;
		break;
	case EArmorType::Chest :
		EquipChest = NewArmor;
		break;
	case EArmorType::Legs:
		EquipLegs = NewArmor;
		break;
	case EArmorType::Shoulders :
		EquipShoulders = NewArmor;
		break;
	case EArmorType::Hands :
		EquipHands = NewArmor;	
		break;
	case EArmorType::Foots :
		EquipFoots = NewArmor;
		break;
	}

	//착용이 끝나면 캐릭터에 장비스텟 적용
	ATPSCharacter* TempCharacter = Cast<ATPSCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (TempCharacter != nullptr)
	{
		TempCharacter->PlayerStatComp->SetEquipmentAbility();
	}
}

void UEquipmentComponent::MountingWeapon(FWeaponPropertyStruct NewWeapon)
{
	ATPSCharacter* TempCharacter = Cast<ATPSCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (TempCharacter != nullptr)
	{
		if (NewWeapon.WeaponType == EWeaponType::AR)
		{
			ARSlot = NewWeapon;
			if (TempCharacter->WeaponSlot1 != nullptr)
			{
				TempCharacter->WeaponSlot1->Destroy();
			}
			auto AR = GetWorld()->SpawnActor<AWeapon_AR>(FVector::ZeroVector, FRotator::ZeroRotator);
			AR->Damage += NewWeapon.AdditionalDamage;
			AR->RPM += NewWeapon.AdditionalRPM;
			TempCharacter->WeaponSlot1 = AR;
			if (TempCharacter->CurrentWeaponSlot == EWeaponNum::Weapon1)
			{
				AR->AttachToComponent(TempCharacter->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("hand_r_ARSocket"));
				TempCharacter->CurWeapon = AR;
			}
			else
			{
				AR->AttachToComponent(TempCharacter->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("Weapon1_Socket"));
			}
		}

		else if (NewWeapon.WeaponType == EWeaponType::SR)
		{
			SRSlot = NewWeapon;
			if (TempCharacter->WeaponSlot2 != nullptr)
			{
				TempCharacter->WeaponSlot2->Destroy();
			}
			auto SR = GetWorld()->SpawnActor<AWeapon_SR>(FVector::ZeroVector, FRotator::ZeroRotator);
			SR->Damage += NewWeapon.AdditionalDamage;
			SR->RPM += NewWeapon.AdditionalRPM;
			TempCharacter->WeaponSlot2 = SR;
			if (TempCharacter->CurrentWeaponSlot == EWeaponNum::Weapon2)
			{
				SR->AttachToComponent(TempCharacter->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("hand_r_SRSocket"));
				TempCharacter->CurWeapon = SR;
			}
			else
			{
				SR->AttachToComponent(TempCharacter->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("Weapon2_Socket"));
			}
		}

		else if (NewWeapon.WeaponType == EWeaponType::PT)
		{
			PistolSlot = NewWeapon;
			if (TempCharacter->WeaponSlot3 != nullptr)
			{
				TempCharacter->WeaponSlot3->Destroy();
			}
			auto Pistol = GetWorld()->SpawnActor<AWeapon_Pistol>(FVector::ZeroVector, FRotator::ZeroRotator);
			Pistol->Damage += NewWeapon.AdditionalDamage;
			Pistol->RPM += NewWeapon.AdditionalRPM;
			TempCharacter->WeaponSlot3 = Pistol;
			if (TempCharacter->CurrentWeaponSlot == EWeaponNum::Pistol)
			{
				Pistol->AttachToComponent(TempCharacter->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("hand_r_PistolSocket"));
				TempCharacter->CurWeapon = Pistol;
			}
			else
			{
				Pistol->AttachToComponent(TempCharacter->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("Pistol_Socket"));
			}
		}
	}
}


