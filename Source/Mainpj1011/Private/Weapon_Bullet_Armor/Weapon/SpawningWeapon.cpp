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
		Ranknum = GameInstance->RandonNumberRet(0, 4); // 랭크 0~4 랜덤한 수 설정
		Typenum = GameInstance->RandonNumberRet(0, 2); // 부위 0~2 랜덤한 수 설정
	}
	TypeSet(); //타입설정
	WeaponProperty.WeaponName = FName(*FullName);// 이름설정
	RankParticleSet();// 등급설정

	UUserWidget* WeaponWidget = CreateWidget<UUserWidget>(GetWorld(), WeaponInfoWidget); // 위젯 생성
	WidgetComponent->SetWidget(WeaponWidget); //  위젯컴포넌트에 설정
	WidgetComponent->SetDrawSize(FVector2D(300, 100)); // 그리는 사이즈 설정
	WidgetComponent->SetWorldLocation(GetActorLocation() + FVector(0, 0, 200.0f)); // 위치 설정
	auto WeaponWidgetObj = Cast<UItemInfomationWidget>(WidgetComponent->GetUserWidgetObject()); // 
	if (WeaponWidgetObj != nullptr)
	{
		WeaponWidgetObj->BindWeapon(this); // 아이템정보위젯에 스폰무기를 보내줘서 등급을 위젯의 색깔로 표시하기 위해서 묶어줌 
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
	WidgetComponent->SetVisibility(false); // 위젯 보이기 false
	InteractionBox->OnComponentBeginOverlap.AddDynamic(this, &ASpawningWeapon::OnComponentBeginOverlap); //.상호작용박스 beginoverlap
	InteractionBox->OnComponentEndOverlap.AddDynamic(this, &ASpawningWeapon::OnOverlapEnd); //  상호작용박스 endoverlap
}

// Called every frame
void ASpawningWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	FRotator WidgetRot = UKismetMathLibrary::FindLookAtRotation(WidgetComponent->GetComponentLocation(), TempCharacter->Camera->GetComponentLocation()); //플레이어가 항상 볼 수 있도록 하는 위젯 회전값
	WidgetComponent->SetWorldRotation(WidgetRot);// 회전시키기
	if (IsEattenItem)// 먹은아이템은
	{
		Destroy();// 삭제함
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

