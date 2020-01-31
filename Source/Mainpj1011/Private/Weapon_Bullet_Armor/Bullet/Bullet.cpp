// Fill out your copyright notice in the Description page of Project Settings.


#include "Bullet.h"
#include "TPSSoundComponent.h"
#include "Sound/SoundCue.h"

// Sets default values
ABullet::ABullet()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	BulletBody = CreateDefaultSubobject<USphereComponent>(TEXT("BULLETBODY"));
	BulletBody->ComponentTags.Add(TEXT("BULLET"));
	BulletBody->SetCollisionProfileName(FName("Bullet"));
	BulletBody->SetSphereRadius(0.5f);
	RootComponent = BulletBody;

	BulletTrail = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("BULLETTRAIL"));
	BulletTrail->SetupAttachment(RootComponent);
	BulletTrail->SetRelativeLocation(FVector(40, 0, 0));
	ConstructorHelpers::FObjectFinder<UParticleSystem>PS_TRAIL(TEXT("/Game/MilitaryWeapDark/FX/P_AssaultRifle_Tracer_01"));
	if(PS_TRAIL.Succeeded())
	{
		BulletTrail->SetTemplate(PS_TRAIL.Object);
	}

	BulletSpark = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("BULLETSPARK"));
	BulletSpark->SetRelativeRotation(FRotator(0.0f, 180.0f, 0.0f));
	BulletSpark->SetupAttachment(RootComponent);
	BulletSpark->SetWorldScale3D(FVector(0.5f, 0.5f, 0.5f));

	ConstructorHelpers::FObjectFinder<UParticleSystem>PS_SPARK(TEXT("/Game/MilitaryWeapDark/FX/P_Impact_Metal_Large_01"));
	if (PS_SPARK.Succeeded())
	{
		BulletSpark->SetTemplate(PS_SPARK.Object);
	}

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("PROJECTILE_MOVEMENT"));
	ProjectileMovement->ProjectileGravityScale = 0.0f;
	ProjectileMovement->InitialSpeed = 5000.0f;
	ProjectileMovement->MaxSpeed = 5000.0f;
	ProjectileMovement->Velocity = FVector(0, 0, 0);

	BulletImpactSoundComp = CreateDefaultSubobject<UTPSSoundComponent>(TEXT("BulletImpactSound"));
	BulletImpactSoundComp->WallHitSound->SetupAttachment(BulletBody);
}

// Called when the game starts or when spawned
void ABullet::BeginPlay()
{
	Super::BeginPlay();
	BulletBody->OnComponentBeginOverlap.AddDynamic(this, &ABullet::OnActorBeginOverlap);
}

// Called every frame
void ABullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

bool ABullet::IsActive()
{
	return Active;
}

void ABullet::SetActive(bool bval)
{
	Active = bval;
	SetActorHiddenInGame(!bval);
	if (!bval)
	{
		this->SetActorLocation(FVector(0, 0, 0));
	}
}


void ABullet::SetVelocity(FVector val)
{
	ProjectileMovement->Velocity = val;
}

void ABullet::OnActorBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherComp->ComponentHasTag(FName("BULLET")))
	{
		this->SetVelocity(FVector(0.0f, 0.0f, 0.0f));
		if (this->IsActive())
		{
			BulletSpark->Activate(true);
		}
		GetWorldTimerManager().SetTimer(DestoryTime, this, &ABullet::SetDeActive,0.01, false);
		if (OtherComp->ComponentHasTag(FName("Wall"))) //벽에 총알이 맞으면 총알 튕기는소리? 를 플레이한다.
		{
			BulletImpactSoundComp->WallHitSound->Play();
		}
	}

}

void ABullet::SetDeActive()
{
	this->SetActive(false);
	GetWorldTimerManager().ClearTimer(DestoryTime);
}
