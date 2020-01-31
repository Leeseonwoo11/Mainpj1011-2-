// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSSoundComponent.h"

// Sets default values for this component's properties
UTPSSoundComponent::UTPSSoundComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	
	HitSound = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioHit"));
	WallHitSound = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioWallHit"));
	StepSound = CreateDefaultSubobject<UAudioComponent>(TEXT("AudipStep"));
	WeaponChangeSound = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioWeaponChange"));
	ARShotSound = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioARShot"));
	ARReloadSound1 = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioARReload1"));
	ARReloadSound2 = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioARReload2"));
	SRShotSound = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioSRShot"));
	SRReloadSound1 = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioSRReload1"));
	SRReloadSound2 = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioSRReload2"));
	PTShotSound = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioPTShot"));
	PTReloadSound1 = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioPTRload1"));
	PTReloadSound2 = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioPTRload2"));
	TrakingMineSound = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioTrakingMine"));
	PulseDetectorSound = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioPulse"));
	SupportSound = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioSupport"));
	
	HitSound->bAutoActivate = false;
	WallHitSound->bAutoActivate = false;
	StepSound->bAutoActivate = false;
	WeaponChangeSound->bAutoActivate = false;
	ARShotSound->bAutoActivate = false;
	ARReloadSound1->bAutoActivate = false;
	ARReloadSound2->bAutoActivate = false;
	SRShotSound->bAutoActivate = false;
	SRReloadSound1->bAutoActivate = false;
	SRReloadSound2->bAutoActivate = false;
	PTShotSound->bAutoActivate = false;
	PTReloadSound1->bAutoActivate = false;
	PTReloadSound2->bAutoActivate = false;
	TrakingMineSound->bAutoActivate = false;
	PulseDetectorSound->bAutoActivate = false;
	SupportSound->bAutoActivate = false;

	// 총알 피격 소리
	static ConstructorHelpers::FObjectFinder<USoundCue>SC_Hit(TEXT("/Game/MyNew/Sound/HitSound/SniperRifle_ImpactBody_Cue"));
	if (SC_Hit.Succeeded())
	{
		HitSound->SetSound(SC_Hit.Object);
	}
	// 벽 총알 맞는소리
	static ConstructorHelpers::FObjectFinder<USoundCue>SC_ImpactSound(TEXT("/Game/MyNew/Sound/BulletImpactSound/Rifle_ImpactSurface_Cue"));
	if (SC_ImpactSound.Succeeded())
	{
		WallHitSound->SetSound(SC_ImpactSound.Object);
	}
	// 발걸음소리
	static ConstructorHelpers::FObjectFinder<USoundCue>SC_StepSound(TEXT("/Game/MyNew/Sound/FootStepSound/FootStepSound_Cue"));
	if (SC_StepSound.Succeeded())
	{
		StepSound->SetSound(SC_StepSound.Object);
	}
	// 무기교체소리
	static ConstructorHelpers::FObjectFinder<USoundCue>SC_WeaponChange(TEXT("/Game/MyNew/Sound/WeaponChange/WeaponChange_Cue"));
	if (SC_WeaponChange.Succeeded())
	{
		WeaponChangeSound->SetSound(SC_WeaponChange.Object);
	}

	//각 화기 발사 소리
	static ConstructorHelpers::FObjectFinder<USoundCue>SC_PTShot(TEXT("/Game/MyNew/Sound/Pistol/PistolB_Fire_Cue"));
	if (SC_PTShot.Succeeded())
	{
		PTShotSound->SetSound(SC_PTShot.Object);
	}
	static ConstructorHelpers::FObjectFinder<USoundCue>SC_ARShot(TEXT("/Game/MyNew/Sound/Rifle/RifleB_FireEnd_Cue"));
	if (SC_ARShot.Succeeded())
	{
		ARShotSound->SetSound(SC_ARShot.Object);
	}
	static ConstructorHelpers::FObjectFinder<USoundCue>SC_SRShot(TEXT("/Game/MyNew/Sound/Sniper/SniperRifleB_Fire_Cue"));
	if (SC_SRShot.Succeeded())
	{
		SRShotSound->SetSound(SC_SRShot.Object);
	}
	//각 화기 재장전 시작 소리
	static ConstructorHelpers::FObjectFinder<USoundCue>SC_PTReload1(TEXT("/Game/MyNew/Sound/Pistol/Pistol_MagEject_Cue"));
	if (SC_PTReload1.Succeeded())
	{
		PTReloadSound1->SetSound(SC_PTReload1.Object);
	}
	static ConstructorHelpers::FObjectFinder<USoundCue>SC_ARReload1(TEXT("/Game/MyNew/Sound/Rifle/Rifle_Reload_Cue"));
	if (SC_ARReload1.Succeeded())
	{
		ARReloadSound1->SetSound(SC_ARReload1.Object);
	}
	static ConstructorHelpers::FObjectFinder<USoundCue>SC_SRReload1(TEXT("/Game/MyNew/Sound/Sniper/SniperRifle_Reload_Cue"));
	if (SC_SRReload1.Succeeded())
	{
		SRReloadSound1->SetSound(SC_SRReload1.Object);
	}
	//각 화기 재장전 끝 소리
	static ConstructorHelpers::FObjectFinder<USoundCue>SC_PTReload2(TEXT("/Game/MyNew/Sound/Pistol/Pistol_AmmoPickup_Cue"));
	if (SC_PTReload2.Succeeded())
	{
		PTReloadSound2->SetSound(SC_PTReload2.Object);
	}
	static ConstructorHelpers::FObjectFinder<USoundCue>SC_ARReload2(TEXT("/Game/MyNew/Sound/Rifle/Rifle_ReloadInsert_Cue"));
	if (SC_ARReload2.Succeeded())
	{
		ARReloadSound2->SetSound(SC_ARReload2.Object);
	}
	static ConstructorHelpers::FObjectFinder<USoundCue>SC_SRReload2(TEXT("/Game/MyNew/Sound/Sniper/SniperRifle_ReloadInsert_Cue"));
	if (SC_SRReload2.Succeeded())
	{
		SRReloadSound2->SetSound(SC_SRReload2.Object);
	}
	// 각각 스킬 소리
	static ConstructorHelpers::FObjectFinder<USoundCue>SC_TrakingMine(TEXT("/Game/MyNew/Sound/SkillSound/TrakingMine_Explosion_Cue"));
	if (SC_TrakingMine.Succeeded())
	{
		TrakingMineSound->SetSound(SC_TrakingMine.Object);
	}
	static ConstructorHelpers::FObjectFinder<USoundCue>SC_PulseScan(TEXT("/Game/MyNew/Sound/SkillSound/PulseScanSound_Cue"));
	if (SC_PulseScan.Succeeded())
	{
		PulseDetectorSound->SetSound(SC_PulseScan.Object);
	}
	static ConstructorHelpers::FObjectFinder<USoundCue>SC_Support(TEXT("/Game/MyNew/Sound/SkillSound/Support_Cue.Support_Cue"));
	if (SC_Support.Succeeded())
	{
		SupportSound->SetSound(SC_Support.Object);
	}
	// ...
}


// Called when the game starts
void UTPSSoundComponent::BeginPlay()
{
	Super::BeginPlay();
	// ...

}


// Called every frame
void UTPSSoundComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

