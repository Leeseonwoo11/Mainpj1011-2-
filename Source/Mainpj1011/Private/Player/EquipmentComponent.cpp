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

	//������ ������ ĳ���Ϳ� ����� ����
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
		if (NewWeapon.WeaponType == EWeaponType::AR)// ���ݼ����̶��
		{
			ARSlot = NewWeapon;
			if (TempCharacter->WeaponSlot1 != nullptr)// ĳ������ ���⽽��1�� ���Ⱑ �ִٸ�
			{
				TempCharacter->WeaponSlot1->Destroy(); // ���⽽�� 1�� ���⸦ ���ش�.
			}
			auto AR = GetWorld()->SpawnActor<AWeapon_AR>(FVector::ZeroVector, FRotator::ZeroRotator);// ���ݼ����� �����Ѵ�.
			AR->Damage += NewWeapon.AdditionalDamage;// ������ ����
			AR->RPM += NewWeapon.AdditionalRPM; // �߻�ӵ� ����
			TempCharacter->WeaponSlot1 = AR; // ������ ���⸦ �������ش�.
			if (TempCharacter->CurrentWeaponSlot == EWeaponNum::Weapon1)// ���繫�⽽���� 1�������̶��
			{
				AR->AttachToComponent(TempCharacter->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("hand_r_ARSocket"));// ������ ���⸦ �� ��Ĺ�� ������Ų��.
				TempCharacter->CurWeapon = AR; // ���繫�⸦ ���ݼ������� �ٲ۴�.
			}
			else //���⽽���� 1���� �ƴ϶��
			{
				AR->AttachToComponent(TempCharacter->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("Weapon1_Socket")); // ���ݼ��� ������Ĺ�� ������Ų��.
			}
		}

		else if (NewWeapon.WeaponType == EWeaponType::SR) // �������۶������̶��
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

		else if (NewWeapon.WeaponType == EWeaponType::PT) // �ǽ����̶��
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


