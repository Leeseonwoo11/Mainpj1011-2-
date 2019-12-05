// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawningArmor.h"
#include "TPSGameInstance.h"

// Sets default values
ASpawningArmor::ASpawningArmor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	InteractionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractionBox"));
	Particle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle"));
	Particle->SetupAttachment(InteractionBox);
	ConstructParticle();
	InteractionBox->ComponentTags.Add(TEXT("SpawnArmor"));
}

// Called when the game starts or when spawned
void ASpawningArmor::BeginPlay()
{
	Super::BeginPlay();

	UTPSGameInstance* GameInstance = Cast<UTPSGameInstance>(GetGameInstance());
	if (GameInstance != nullptr)
	{
		Ranknum = GameInstance->RandonNumberRet(0,4);
		Brandnum = GameInstance->RandonNumberRet(0,2);
		Typenum = GameInstance->RandonNumberRet(0,5);
	}
	BrandSet();
	TypeSet();
	FullName = BrandName + TypeName;
	ArmorProperty.ArmorName = FName(*FullName);
	RankParticleSet();
	UE_LOG(LogTexture, Error, TEXT("Fullname is %s"), *FullName);
	UE_LOG(LogTexture, Error, TEXT("WeaponPower is %f"), ArmorProperty.WeaponPower);
	UE_LOG(LogTexture, Error, TEXT("SkillPower is %f"), ArmorProperty.SkillPower);
	UE_LOG(LogTexture, Error, TEXT("Health is %f"), ArmorProperty.Health);

}

// Called every frame
void ASpawningArmor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}


void ASpawningArmor::ConstructParticle()
{
	static ConstructorHelpers::FObjectFinder<UParticleSystem>PS_Common(TEXT("/Game/MyNew/Particle/ItemEffect/CommonEffect"));
	if (PS_Common.Succeeded())
	{
		CommonEffect = PS_Common.Object;
	}
	static ConstructorHelpers::FObjectFinder<UParticleSystem>PS_UnCommon(TEXT("/Game/MyNew/Particle/ItemEffect/UnCommonEffect"));
	if (PS_UnCommon.Succeeded())
	{
		UnCommonEffect = PS_UnCommon.Object;
	}
	static ConstructorHelpers::FObjectFinder<UParticleSystem>PS_Rare(TEXT("/Game/MyNew/Particle/ItemEffect/RareEffect"));
	if (PS_Rare.Succeeded())
	{
		RareEffect = PS_Rare.Object;
	}
	static ConstructorHelpers::FObjectFinder<UParticleSystem>PS_Epic(TEXT("/Game/MyNew/Particle/ItemEffect/EpicEffect"));
	if (PS_Epic.Succeeded())
	{
		EpicEffect = PS_Epic.Object;
	}
	static ConstructorHelpers::FObjectFinder<UParticleSystem>PS_Legendary(TEXT("/Game/MyNew/Particle/ItemEffect/LegendaryEffect"));
	if (PS_Legendary.Succeeded())
	{
		LegendaryEffect = PS_Legendary.Object;
	}
}

void ASpawningArmor::RankParticleSet()
{
	switch (Ranknum)
	{
	case 0:
		CurrentEffect = CommonEffect;
		ArmorProperty.Rank = ERank::Common;
		ArmorProperty.WeaponPower *= 1.0;
		ArmorProperty.SkillPower *= 1.0;
		ArmorProperty.Health *= 1.0;
		break;
	case 1:
		CurrentEffect = UnCommonEffect;
		ArmorProperty.Rank = ERank::UnCommon;
		ArmorProperty.WeaponPower *= 2.0;
		ArmorProperty.SkillPower *= 2.0;
		ArmorProperty.Health *= 2.0;
		break;
	case 2:
		CurrentEffect = RareEffect;
		ArmorProperty.Rank = ERank::Rare;
		ArmorProperty.WeaponPower *= 3.0;
		ArmorProperty.SkillPower *= 3.0;
		ArmorProperty.Health *= 3.0;
		break;
	case 3:
		CurrentEffect = EpicEffect;
		ArmorProperty.Rank = ERank::Epic;
		ArmorProperty.WeaponPower *= 4.0;
		ArmorProperty.SkillPower *= 4.0;
		ArmorProperty.Health *= 4.0;
		break;
	case 4:
		CurrentEffect = LegendaryEffect;
		ArmorProperty.Rank = ERank::Legendary;
		ArmorProperty.WeaponPower *= 5.0;
		ArmorProperty.SkillPower *= 5.0;
		ArmorProperty.Health *= 5.0;
		break;
	default:
		break;
	}
	UParticleSystem* RankParticle = Cast<UParticleSystem>(CurrentEffect);
	if (RankParticle != nullptr)
	{
		Particle->SetTemplate(RankParticle);
	}
}

void ASpawningArmor::BrandSet()
{
	switch (Brandnum)
	{
	case 0:
		ArmorProperty.Brand = EBrand::AlpsGroup;
		ArmorProperty.WeaponPower = 1.0f;
		ArmorProperty.SkillPower = 3.0f;
		ArmorProperty.Health = 1.0f;
		BrandName = "Alps";
		break;
	case 1:
		ArmorProperty.Brand = EBrand::GilagadGroup;
		ArmorProperty.WeaponPower = 1.0f;
		ArmorProperty.SkillPower = 1.0f;
		ArmorProperty.Health = 3.0f;
		BrandName = "Gilagad";
		break;
	case 2:
		ArmorProperty.Brand = EBrand::FenrirGroup;
		ArmorProperty.WeaponPower = 3.0f;
		ArmorProperty.SkillPower = 1.0f;
		ArmorProperty.Health = 1.0f;
		BrandName = "Fenrir";
		break;
	default:
		break;
	}
}

void ASpawningArmor::TypeSet()
{
	switch (Typenum)
	{
	case 0:
		ArmorProperty.ArmorType = EArmorType::Chest;
		TypeName = "Chestguard";
		break;
	case 1:
		ArmorProperty.ArmorType = EArmorType::Foots;
		TypeName = "Shoes";
		break;
	case 2:
		ArmorProperty.ArmorType = EArmorType::Hands;
		TypeName = "Gloves";
		break;
	case 3:
		ArmorProperty.ArmorType = EArmorType::Head;
		TypeName = "headpiece";
		break;
	case 4:
		ArmorProperty.ArmorType = EArmorType::Legs;
		TypeName = "Pants";
		break;
	case 5:
		ArmorProperty.ArmorType = EArmorType::Shoulders;
		TypeName = "Shoulderguards";
		break;
	default:
		break;
	}
}

