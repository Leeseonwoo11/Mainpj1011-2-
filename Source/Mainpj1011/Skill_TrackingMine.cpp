// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill_TrackingMine.h"
#include "TPSEnemy.h"
#include "AIController.h"
#include "Particles/ParticleSystem.h"
#include "DrawDebugHelpers.h"
#include "TPSCharacter.h"
#include "TPSCharacterStatComponent.h"

ASkill_TrackingMine::ASkill_TrackingMine()
{
	PrimaryActorTick.bCanEverTick = true;

	GetCapsuleComponent()->SetCollisionProfileName(FName("SkillObject"));

	SkillRange->SetWorldScale3D(FVector(1.0f, 1.0f, 1.0f));
	SkillRange->SetCollisionProfileName(FName("Bullet"));
	SkillRange->ComponentTags.Add(TEXT("MINE"));

	DetectRange = CreateDefaultSubobject<USphereComponent>(TEXT("DetectRange"));
	DetectRange->SetWorldScale3D(FVector(50.0f, 50.0f, 50.0f));
	DetectRange->SetupAttachment(GetCapsuleComponent());
	DetectRange->SetCollisionProfileName(FName("Pulse"));

	GetCharacterMovement()->MaxWalkSpeed = 1200.0f;
	static ConstructorHelpers::FObjectFinder<UStaticMesh>SM_BODY(TEXT("/Game/StarterContent/Shapes/Shape_Sphere"));
	if (SM_BODY.Succeeded())
	{
		Body->SetStaticMesh(SM_BODY.Object);
	}
	Body->SetWorldLocation(FVector(0.0f, 0.0f, -80.0f));
	Body->SetWorldScale3D(FVector(0.3f, 0.3f, 0.3f));
	Body->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Body->SetupAttachment(GetCapsuleComponent());
	static ConstructorHelpers::FObjectFinder<UMaterial>M_BODY(TEXT("/Engine/VREditor/LaserPointer/LaserPointerMaterial"));
	if (M_BODY.Succeeded())
	{
		Body->SetMaterial(0, M_BODY.Object);
	}
	Plane = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SkillRangePlane"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh>SM_PLANE(TEXT("/Game/StarterContent/Architecture/SM_AssetPlatform"));
	if (SM_PLANE.Succeeded())
	{
		Plane->SetStaticMesh(SM_PLANE.Object);
	}
	Plane->SetupAttachment(Body);
	Plane->SetWorldScale3D(FVector(0.1f, 0.1f, 0.25f));
	Plane->SetWorldLocation(FVector(0.0f, 0.0f, -88.0f));
	static ConstructorHelpers::FObjectFinder<UParticleSystem>PS_EXP(TEXT("/Game/MyNew/Particle/SkillEffect/MineEffect"));
	if (PS_EXP.Succeeded())
	{
		ParticleFX1 = PS_EXP.Object;
	}
	static ConstructorHelpers::FObjectFinder<UMaterial>M_PLANE(TEXT("/Engine/MapTemplates/Materials/BasicAsset01"));
	if(M_PLANE.Succeeded())
	{
		Plane->SetMaterial(0, M_PLANE.Object);
	}
	SkillType = ESkillType::TrackingMines;
}

void ASkill_TrackingMine::BeginPlay()
{
	Super::BeginPlay();

	SpawnDefaultController();
	DetectRange->OnComponentBeginOverlap.AddDynamic(this, &ASkill_TrackingMine::OnActorBeginOverlap);
	ATPSCharacter* TempCharacter = Cast<ATPSCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (TempCharacter != nullptr)
	{
		Damage+=TempCharacter->PlayerStatComp->PlayerSkillPower;
	}

}

void ASkill_TrackingMine::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bDetectflag)
	{
		DetectRange->SetWorldScale3D(FVector(50.0f, 50.0f, 50.0f));
		bDetectflag = false;
	}
	else
	{
		DetectRange->SetWorldScale3D(FVector(1.0f, 1.0f, 1.0f));
		bDetectflag = true;
	}
	AAIController* TempAIController = Cast<AAIController>(GetController());
	if (bPlayerfallow)
	{
		if (TempAIController != nullptr)
		{
			TempAIController->MoveToLocation(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)->GetActorLocation(), 100.0f, false);
		}
		else
		{
			UE_LOG(LogTexture, Error, TEXT("AIController is nullptr"));
		}
	}

	if (bPlayerfallow == false)
	{
		if (IsExplosion == false)
		{
			DrawDebugLine(GetWorld(), GetMesh()->GetComponentLocation(), TargetLocation, FColor::Red);

			if (EnemyMoveTo == EPathFollowingRequestResult::AlreadyAtGoal)
			{
				UE_LOG(LogTexture, Error, TEXT(" doldollee Move "));
				Plane->SetWorldScale3D(FVector(3.0f, 3.0f, 0.25f));
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ParticleFX1, GetActorLocation());
				IsExplosion = true;
				SkillRange->SetWorldScale3D(FVector(10.0f, 10.0f, 10.0f));
			}
		}
	}
}

void ASkill_TrackingMine::OnActorBeginOverlap(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherComp->ComponentHasTag(TEXT("ENEMY")))
	{
		AAIController* TempAiController = Cast<AAIController>(GetController());
		UE_LOG(LogTexture, Error, TEXT("Detect Enemy"));
		if (TempAiController != nullptr)
		{
			bPlayerfallow = false;
			UE_LOG(LogTexture, Error, TEXT("TempAiController not null"));
			TargetLocation = OtherComp->GetOwner()->GetActorLocation();
			EnemyMoveTo = TempAiController->MoveToActor(OtherComp->GetOwner(), 50.0f);
		}
	}
	UE_LOG(LogTexture, Error, TEXT("No Detect Enemy"));
}
