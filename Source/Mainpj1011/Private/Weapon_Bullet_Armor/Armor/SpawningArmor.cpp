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
		Ranknum = GameInstance->RandonNumberRet(0, 4);// 랭크0~4 랜덤한숫자를 받아옴
		Brandnum = GameInstance->RandonNumberRet(0, 2);// 브랜드0~2 랜덤한 숫자를 받아옴
		Typenum = GameInstance->RandonNumberRet(0, 5);// 부위 0~5 랜덤한숫자 받아옴
	}
	BrandSet();
	TypeSet();
	FullName = BrandName + TypeName; //브렌드 + 부위  = 풀네임 ex) 알프스 + 장갑 = 알프스장갑
	ArmorProperty.ArmorName = FName(*FullName);
	RankParticleSet();

	UE_LOG(LogTexture, Error, TEXT("Fullname is %s"), *FullName);
	UE_LOG(LogTexture, Error, TEXT("WeaponPower is %f"), ArmorProperty.WeaponPower);
	UE_LOG(LogTexture, Error, TEXT("SkillPower is %f"), ArmorProperty.SkillPower);
	UE_LOG(LogTexture, Error, TEXT("Health is %f"), ArmorProperty.Health);

	UUserWidget* ArmorWidget = CreateWidget<UUserWidget>(GetWorld(), ArmorInfoWidget); //위젯생성
	WidgetComponent->SetWidget(ArmorWidget); // 위젯컴포넌트에 설정해준다.
	WidgetComponent->SetDrawSize(FVector2D(300, 100)); //위젯크기설정
	WidgetComponent->SetWorldLocation(GetActorLocation() + FVector(0, 0, 200.0f)); // 위치설정
	auto ArmorWidgetObj = Cast<UItemInfomationWidget>(WidgetComponent->GetUserWidgetObject());
	if (ArmorWidgetObj != nullptr)
	{
		ArmorWidgetObj->BindArmor(this);// 아이템정보위젯에 이 액터를 넘겨 묶어줌(브랜드에 따라서 위젯의 색깔을 바꿔주기 위해서 실행함)
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
	InteractionBox->OnComponentBeginOverlap.AddDynamic(this, &ASpawningArmor::OnComponentBeginOverlap);// 플래이어와 겹칠때 아이템 정보 위젯 보여줌
	InteractionBox->OnComponentEndOverlap.AddDynamic(this, &ASpawningArmor::OnOverlapEnd); //  플레이어와 겹침이 끝났을 때 아이템 정보 위젯 안보여줌
}

// Called every frame
void ASpawningArmor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);


	FRotator WidgetRot = UKismetMathLibrary::FindLookAtRotation(WidgetComponent->GetComponentLocation(), TempCharacter->Camera->GetComponentLocation());
	WidgetComponent->SetWorldRotation(WidgetRot);// 플레이어를 항상 바라보도록 회전값 설정
	if (IsEattenItem)// 먹은아이템은
	{
		Destroy(); // 삭제함
	}
}


void ASpawningArmor::ConstructParticle() // 등급에 따라 오라의 색깔 모양이 달라짐
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

void ASpawningArmor::RankParticleSet()// 랜덤하게 받은 수로 등급을 지정한다. 등급에 따라 방어구의 능력치가 달라진다.
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

void ASpawningArmor::BrandSet() // 랜덤하게 받은 수로 브랜드를 지정한다. 브랜드에 따라서 특화된 능력치가 달라진다.
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

void ASpawningArmor::TypeSet()// 랜덤하게 받은 수로 부위를 지정한다.
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
		WidgetComponent->SetVisibility(true);// 아이템 정보 위젯 보이기 true 
		IsEatableItem = true; // 아이템 습득할 수 있는 상태 true
		TempCharacter->TempArmorArray.Add(this);// 임시인벤토리에 추가 후 습득 또는 습득하지 않을 수 있다.
	}
}
void ASpawningArmor::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherComp->ComponentHasTag(FName("PLAYER")))
	{
		WidgetComponent->SetVisibility(false);// 아이템 정보 위젯 보이기 false
		IsEatableItem = false; // 아이템 습득할 수 있는 상태 false
		TempCharacter->TempArmorArray.Remove(this);// 임시인벤토리에 있는 이 액터 삭제
	}
}




