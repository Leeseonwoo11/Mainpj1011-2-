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
		if (NewWeapon.WeaponType == EWeaponType::AR)// 돌격소총이라면
		{
			ARSlot = NewWeapon;
			if (TempCharacter->WeaponSlot1 != nullptr)// 캐릭터의 무기슬롯1에 무기가 있다면
			{
				TempCharacter->WeaponSlot1->Destroy(); // 무기슬롯 1의 무기를 없앤다.
			}
			auto AR = GetWorld()->SpawnActor<AWeapon_AR>(FVector::ZeroVector, FRotator::ZeroRotator);// 돌격소총을 스폰한다.
			AR->Damage += NewWeapon.AdditionalDamage;// 데미지 설정
			AR->RPM += NewWeapon.AdditionalRPM; // 발사속도 설정
			TempCharacter->WeaponSlot1 = AR; // 스폰한 무기를 지정해준다.
			if (TempCharacter->CurrentWeaponSlot == EWeaponNum::Weapon1)// 현재무기슬롯이 1번슬롯이라면
			{
				AR->AttachToComponent(TempCharacter->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("hand_r_ARSocket"));// 스폰한 무기를 손 소캣에 장착시킨다.
				TempCharacter->CurWeapon = AR; // 현재무기를 돌격소총으로 바꾼다.
			}
			else //무기슬롯이 1번이 아니라면
			{
				AR->AttachToComponent(TempCharacter->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("Weapon1_Socket")); // 돌격소총 수납소캣에 장착시킨다.
			}
		}

		else if (NewWeapon.WeaponType == EWeaponType::SR) // 스나이퍼라이플이라면
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

		else if (NewWeapon.WeaponType == EWeaponType::PT) // 피스톨이라면
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


