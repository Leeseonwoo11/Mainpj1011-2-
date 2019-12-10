// Fill out your copyright notice in the Description page of Project Settings.


#include "EquipmentComponent.h"
#include "TPSCharacter.h"
#include "TPSCharacterStatComponent.h"

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
