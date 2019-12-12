// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawningWeapon.h"
#include "ItemInfomationWidget.h"
#include "TPSGameInstance.h"
#include "Kismet/KismetMathLibrary.h"
#include "TPSCharacter.h"


// Sets default values
ASpawningWeapon::ASpawningWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	InteractionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("InteractionBox"));
	InteractionBox->SetRelativeScale3D(FVector(5, 5, 5));
	InteractionBox->ComponentTags.Add(TEXT("SpawnArmor"));
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
		WeaponInfoWidget = UW_InfoWidget.Object;
		if (UW_InfoWidget.Object == nullptr)
		{
			UE_LOG(LogTexture, Error, TEXT("UW_InfoWidget is nullptr"));
		}
	}
}

void ASpawningWeapon::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	UTPSGameInstance* GameInstance = Cast<UTPSGameInstance>(GetGameInstance());
	if (GameInstance != nullptr)
	{
		Ranknum = GameInstance->RandonNumberRet(0, 4);
		Typenum = GameInstance->RandonNumberRet(0, 2);
	}
	TypeSet();
	WeaponProperty.WeaponName = FName(*FullName);
	RankParticleSet();

	UUserWidget* WeaponWidget = CreateWidget<UUserWidget>(GetWorld(), WeaponInfoWidget);
	WidgetComponent->SetWidget(WeaponWidget);
	WidgetComponent->SetDrawSize(FVector2D(300, 100));
	WidgetComponent->SetWorldLocation(GetActorLocation() + FVector(0, 0, 200.0f));
	auto WeaponWidgetObj = Cast<UItemInfomationWidget>(WidgetComponent->GetUserWidgetObject());
	if (WeaponWidgetObj != nullptr)
	{
		WeaponWidgetObj->BindWeapon(this);
	}

}

// Called when the game starts or when spawned
void ASpawningWeapon::BeginPlay()
{
	Super::BeginPlay();

	TempCharacter = Cast<ATPSCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (TempCharacter == nullptr)
	{
		UE_LOG(LogTexture, Error, TEXT("TempCharacter is nullptr"));
	}
	WidgetComponent->SetVisibility(false);
	InteractionBox->OnComponentBeginOverlap.AddDynamic(this, &ASpawningWeapon::OnComponentBeginOverlap);
	InteractionBox->OnComponentEndOverlap.AddDynamic(this, &ASpawningWeapon::OnOverlapEnd);
}

// Called every frame
void ASpawningWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FRotator WidgetRot = UKismetMathLibrary::FindLookAtRotation(WidgetComponent->GetComponentLocation(), TempCharacter->Camera->GetComponentLocation());
	WidgetComponent->SetWorldRotation(WidgetRot);
	if (IsEattenItem)
	{
		Destroy();
	}
}


void ASpawningWeapon::ConstructParticle()
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

void ASpawningWeapon::RankParticleSet()
{
	switch (Ranknum)
	{
	case 0:
		CurrentEffect = CommonEffect;
		WeaponProperty.Rank = EWeaponRank::Common;
		WeaponProperty.AdditionalDamage = 0.0f;
		WeaponProperty.AdditionalRPM = 0;
		break;
	case 1:
		CurrentEffect = UnCommonEffect;
		WeaponProperty.Rank = EWeaponRank::UnCommon;
		WeaponProperty.AdditionalDamage = 10.0f;
		WeaponProperty.AdditionalRPM = 75;
		break;
	case 2:
		CurrentEffect = RareEffect;
		WeaponProperty.Rank = EWeaponRank::Rare;
		WeaponProperty.AdditionalDamage = 20.0f;
		WeaponProperty.AdditionalRPM = 150;
		break;
	case 3:
		CurrentEffect = EpicEffect;
		WeaponProperty.Rank = EWeaponRank::Epic;
		WeaponProperty.AdditionalDamage = 30.0f;
		WeaponProperty.AdditionalRPM = 200;
		break;
	case 4:
		CurrentEffect = LegendaryEffect;
		WeaponProperty.Rank = EWeaponRank::Legendary;
		WeaponProperty.AdditionalDamage = 40.0f;
		WeaponProperty.AdditionalRPM =	300;
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

void ASpawningWeapon::TypeSet()
{
	switch (Typenum)
	{
	case 0:
		WeaponProperty.WeaponType = EWeaponType::PT;
		FullName = "Pistol";
		break;
	case 1:
		WeaponProperty.WeaponType = EWeaponType::AR;
		FullName = "AssaultRifle";
		break;
	case 2:
		WeaponProperty.WeaponType = EWeaponType::SR;
		FullName = "SniperRifle";
		break;
	default:
		break;
	}
}

void ASpawningWeapon::OnComponentBeginOverlap(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherComp->ComponentHasTag(FName("PLAYER")))
	{
		WidgetComponent->SetVisibility(true);
		IsEatableItem = true;
		TempCharacter->TempWeaponArray.Add(this);
	}
}

void ASpawningWeapon::OnOverlapEnd(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	if (OtherComp->ComponentHasTag(FName("PLAYER")))
	{
		WidgetComponent->SetVisibility(false);
		IsEatableItem = false;
		TempCharacter->TempWeaponArray.Remove(this);
	}
}

