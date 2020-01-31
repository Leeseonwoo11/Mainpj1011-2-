// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawningArmor.h"
#include "ItemInfomationWidget.h"
#include "TPSGameInstance.h"
#include "TPSCharacter.h"
#include "Kismet/KismetMathLibrary.h"


// Sets default values
ASpawningArmor::ASpawningArmor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	InteractionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractionBox"));
	InteractionBox->SetRelativeScale3D(FVector(5, 5, 5));
	InteractionBox->ComponentTags.Add(TEXT("SpawnArmor"));
	InteractionBox->SetCollisionProfileName(TEXT("Item"));
	RootComponent = InteractionBox;

	Particle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle"));
	Particle->SetupAttachment(InteractionBox);
	ConstructParticle();
	Particle->SetRelativeScale3D(FVector(0.2, 0.2, 0.2));

	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("WidgetComponent"));
	WidgetComponent->SetupAttachment(InteractionBox);
	WidgetComponent->SetRelativeScale3D(FVector(0.2, 0.2, 0.2));


	static ConstructorHelpers::FObjectFinder<UClass>UW_InfoWidget(TEXT("/Game/MyNew/UI/ItemUI/ItemUI.ItemUI_C"));
	if (UW_InfoWidget.Succeeded())
	{
		ArmorInfoWidget = UW_InfoWidget.Object;
		if (UW_InfoWidget.Object == nullptr)
		{
			UE_LOG(LogTexture, Error, TEXT("UW_InfoWidget is nullptr"));
		}
	}

}
void ASpawningArmor::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	UTPSGameInstance* GameInstance = Cast<UTPSGameInstance>(GetGameInstance());
	if (GameInstance != nullptr)
	{
		Ranknum = GameInstance->RandonNumberRet(0, 4);
		Brandnum = GameInstance->RandonNumberRet(0, 2);
		Typenum = GameInstance->RandonNumberRet(0, 5);
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

	UUserWidget* ArmorWidget = CreateWidget<UUserWidget>(GetWorld(), ArmorInfoWidget);
	WidgetComponent->SetWidget(ArmorWidget);
	WidgetComponent->SetDrawSize(FVector2D(300, 100));
	WidgetComponent->SetWorldLocation(GetActorLocation() + FVector(0, 0, 200.0f));
	auto ArmorWidgetObj = Cast<UItemInfomationWidget>(WidgetComponent->GetUserWidgetObject());
	if (ArmorWidgetObj != nullptr)
	{
		ArmorWidgetObj->BindArmor(this);
	}
}

// Called when the game starts or when spawned
void ASpawningArmor::BeginPlay()
{
	Super::BeginPlay();

	TempCharacter = Cast<ATPSCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (TempCharacter == nullptr)
	{
		UE_LOG(LogTexture, Error, TEXT("TempCharacter is nullptr"));
	}
	WidgetComponent->SetVisibility(false);
	InteractionBox->OnComponentBeginOverlap.AddDynamic(this, &ASpawningArmor::OnComponentBeginOverlap);
	InteractionBox->OnComponentEndOverlap.AddDynamic(this, &ASpawningArmor::OnOverlapEnd);
}

// Called every frame
void ASpawningArmor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	FRotator WidgetRot = UKismetMathLibrary::FindLookAtRotation(WidgetComponent->GetComponentLocation(), TempCharacter->Camera->GetComponentLocation());
	WidgetComponent->SetWorldRotation(WidgetRot);
	if (IsEattenItem)
	{
		Destroy();
	}
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
		ArmorProperty.Rank = EArmorRank::Common;
		ArmorProperty.WeaponPower *= 1.0;
		ArmorProperty.SkillPower *= 1.0;
		ArmorProperty.Health *= 1.0;
		ArmorProperty.KeepWarm = 1.0f;//보온효과 +1
		break;
	case 1:
		CurrentEffect = UnCommonEffect;
		ArmorProperty.Rank = EArmorRank::UnCommon;
		ArmorProperty.WeaponPower *= 2.0;
		ArmorProperty.SkillPower *= 2.0;
		ArmorProperty.Health *= 2.0;
		ArmorProperty.KeepWarm = 3.0f;//보온효과 +3
		break;
	case 2:
		CurrentEffect = RareEffect;
		ArmorProperty.Rank = EArmorRank::Rare;
		ArmorProperty.WeaponPower *= 3.0;
		ArmorProperty.SkillPower *= 3.0;
		ArmorProperty.Health *= 4.0;
		ArmorProperty.KeepWarm = 5.0f;//보온효과 +5

		break;
	case 3:
		CurrentEffect = EpicEffect;
		ArmorProperty.Rank = EArmorRank::Epic;
		ArmorProperty.WeaponPower *= 4.0;
		ArmorProperty.SkillPower *= 4.0;
		ArmorProperty.Health *= 4.0;
		ArmorProperty.KeepWarm = 7.0f;//보온효과 +7

		break;
	case 4:
		CurrentEffect = LegendaryEffect;
		ArmorProperty.Rank = EArmorRank::Legendary;
		ArmorProperty.WeaponPower *= 5.0;
		ArmorProperty.SkillPower *= 5.0;
		ArmorProperty.Health *= 5.0;
		ArmorProperty.KeepWarm = 9.0f; //보온효과 +9
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
		ArmorProperty.Brand = EBrand::GilagardGroup;
		ArmorProperty.WeaponPower = 1.0f;
		ArmorProperty.SkillPower = 1.0f;
		ArmorProperty.Health = 3.0f;
		BrandName = "Gilagard";
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

void ASpawningArmor::OnComponentBeginOverlap(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherComp->ComponentHasTag(FName("PLAYER")))
	{
		WidgetComponent->SetVisibility(true);
		IsEatableItem = true;
		TempCharacter->TempArmorArray.Add(this);
	}
}
void ASpawningArmor::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherComp->ComponentHasTag(FName("PLAYER")))
	{
		WidgetComponent->SetVisibility(false);
		IsEatableItem = false;
		TempCharacter->TempArmorArray.Remove(this);
	}
}




