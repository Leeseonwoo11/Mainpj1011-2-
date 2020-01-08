// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSEnemy.h"
#include "Weapon_AR.h"
#include "DrawDebugHelpers.h"
#include "TPSAIController.h"
#include "Kismet/KismetMathLibrary.h"
#include "EnemyHPBarWidget.h"
#include "AIModule/Classes/BrainComponent.h"
#include "Skill_TrackingMine.h"
#include "SpawningArmor.h"
#include "SpawningWeapon.h"
#include "TPSGameInstance.h"
#include "Perception/AISenseConfig_Sight.h"



// Sets default values
ATPSEnemy::ATPSEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -88.0f), FRotator(0, -90, 0));
	GetCapsuleComponent()->ComponentTags.Add(TEXT("ENEMY"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>SK_BODY(TEXT("/Game/Mannequin/Character/Mesh/SK_Mannequin"));
	if (SK_BODY.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_BODY.Object);
	}
	GetMesh()->SetCollisionProfileName(FName("Ragdoll"));
	
	static ConstructorHelpers::FClassFinder<UAnimInstance>ANIMBP_BODY(TEXT("/Game/MyNew/Animation/EnemyAnim/EnemyAnimation"));
	if (ANIMBP_BODY.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(ANIMBP_BODY.Class);
	}
	
	BulletPool = CreateDefaultSubobject<UBulletPoolComponent>(TEXT("BULLETPOOL"));

	StatComp = CreateDefaultSubobject<UTPSEnemyStatComponent>(TEXT("STATCOMP"));

	AIControllerClass = ATPSAIController::StaticClass();

	EnemyHPWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPWIDGET"));
	EnemyHPWidget->SetupAttachment(GetMesh());
	EnemyHPWidget->SetRelativeLocation(FVector(0.0f, 0.0f, 180.0f));
	static ConstructorHelpers::FObjectFinder<UClass>UW_HPBar(TEXT("/Game/MyNew/HUD/EnemyHPBar.EnemyHPBar_C"));
	if (UW_HPBar.Succeeded())
	{
		Widget = UW_HPBar.Object;
	}
	static ConstructorHelpers::FObjectFinder<UMaterial>M_BODY(TEXT("/Game/MyNew/Material/M_EnemyBody"));
	if(M_BODY.Succeeded())
	{ 
		GetMesh()->SetMaterial(0, M_BODY.Object);
	}
}

// Called when the game starts or when spawned
void ATPSEnemy::BeginPlay()
{
	Super::BeginPlay();

	auto AssaultRifle = GetWorld()->SpawnActor<AWeapon_AR>(FVector::ZeroVector, FRotator::ZeroRotator);
	CurWeapon = AssaultRifle;
	if (AssaultRifle != nullptr)
	{
		AssaultRifle->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("hand_r_ARSocket"));
	}
	TempCharacter = Cast<ATPSCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &ATPSEnemy::OnComponentBeginOverlap);
	bIsDead = false;
}

void ATPSEnemy::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	UUserWidget* HPBar = CreateWidget<UUserWidget>(GetWorld(), Widget);
	EnemyHPWidget->SetWidget(HPBar);
	EnemyHPWidget->SetDrawSize(FVector2D(150.0f, 50.0f));
	auto EnemyWidget = Cast<UEnemyHPBarWidget>(EnemyHPWidget->GetUserWidgetObject());
	if (EnemyWidget != nullptr)
	{
		EnemyWidget->BindEnemyHP(StatComp);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("EnemyWidget is nullptr"));
	}
}

// Called every frame
void ATPSEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//�Ѿ� �߻� ����~ 
	ATPSAIController* TPSAIController = Cast<ATPSAIController>(GetController());
	if (TPSAIController != nullptr)
	{
		if (TPSAIController->CanSeePlayerAI)
		{
			TargetLoc = TempCharacter->GetMesh()->GetSocketLocation(FName("neck_01")); // Fire�Լ����� �Ѿ��� �߻��� ��ġ
		}
	}
	FRotator WidgetRot = UKismetMathLibrary::FindLookAtRotation(EnemyHPWidget->GetComponentLocation(), TempCharacter->GetMesh()->GetComponentLocation());
	EnemyHPWidget->SetWorldRotation(WidgetRot); // ���� ĳ���Ͱ� �����ְ� ������
}

// Called to bind functionality to input
void ATPSEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
}

void ATPSEnemy::SetFalseFireState()
{
	bFireState = false;
	GetWorldTimerManager().ClearTimer(FireSpeedTimer1);
}

void ATPSEnemy::SetTrueFireState()
{
	bFireState = true;
	preFire();
}

void ATPSEnemy::preFire()
{
	float Fireduration = 60.0f / (float)CurWeapon->RPM;
	GetWorldTimerManager().SetTimer(FireSpeedTimer1, this, &ATPSEnemy::Fire, Fireduration, true);
}

void ATPSEnemy::Fire()
{
	if (CurWeapon != nullptr)
	{
		auto TempBullet = BulletPool->GetBulletPtr();
		if (TempBullet != nullptr)
		{
			if (!Cast<ATPSAIController>(GetController())->CanSeePlayerAI)
			{
				FRotator BulletRot = GetActorRotation();
				FVector FireVector = GetActorForwardVector();
				BulletRot.Pitch = CurWeapon->FirePos->GetComponentRotation().Pitch + 3.0f;
				TempBullet->Damage = CurWeapon->Damage;
				TempBullet->SetActorLocation(CurWeapon->FirePos->GetComponentLocation());
				TempBullet->SetActorRotation(BulletRot);
				TempBullet->ProjectileMovement->Velocity = FireVector * 6000;
				TempBullet->ProjectileMovement->Velocity.Z = (CurWeapon->FirePos->GetComponentLocation().Z - TargetLoc.Z);
				TempBullet->SetActive(true);
				TempBullet->BulletTrail->Activate(true);
			}
			else
			{
				FRotator BulletRot = GetActorRotation();
				FVector FireVector = (TargetLoc - CurWeapon->FirePos->GetComponentLocation()).GetSafeNormal();
				BulletRot.Pitch = CurWeapon->FirePos->GetComponentRotation().Pitch + 3.0f;
				TempBullet->Damage = CurWeapon->Damage;
				TempBullet->SetActorLocation(CurWeapon->FirePos->GetComponentLocation());
				TempBullet->SetActorRotation(BulletRot);
				TempBullet->ProjectileMovement->Velocity = FireVector * 151200; // ���� �Ѿ��� �ӵ��� �ʹ������� �÷��̾ ���̸� �÷��̾�� ���� �׷��� �Ѿ� �ӵ��� �� ����.
				TempBullet->SetActive(true);
				TempBullet->BulletTrail->Activate(true);
				DrawDebugLine(GetWorld(), CurWeapon->FirePos->GetComponentLocation(), TargetLoc, FColor::Red, false, 1.0f);
			}
			UE_LOG(LogTemp, Error, TEXT("FIRE + %d"), count++);
		}
	}
}


void ATPSEnemy::SetTrueCoverState()
{
	bCrouchState = true;
	GetCapsuleComponent()->SetRelativeScale3D(FVector(1.0f, 1.0f, 0.65f));
	GetMesh()->SetRelativeScale3D(FVector(1.0f, 1.0f, 1.54f));
}

void ATPSEnemy::SetFalseCoverState()
{
	bCrouchState = false;
	GetCapsuleComponent()->SetRelativeScale3D(FVector(1.0f, 1.0f, 1.0f));
	GetMesh()->SetRelativeScale3D(FVector(1.0f, 1.0f, 1.0f));
}

void ATPSEnemy::SetTrueAimState()
{
	bAimState = true;
}

void ATPSEnemy::SetFalseAimState()
{
	bAimState = false;
}

void ATPSEnemy::CStartShooting()
{
	SetTrueFireState();
}

void ATPSEnemy::CStopShoothing()
{
	SetFalseFireState();
}

void ATPSEnemy::CEnterCover()
{
	SetTrueCoverState();
}
void ATPSEnemy::COutCover()
{
	SetFalseCoverState();
}

void ATPSEnemy::OnComponentBeginOverlap(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherComp->ComponentHasTag(FName("BULLET")))
	{
		StatComp->SetDamage((float)Cast<ABullet>(OtherComp->GetOwner())->Damage);
		ATPSAIController* AIController = Cast<ATPSAIController>(GetController());
		if (AIController != nullptr)
		{
			AIController->SetFocus(TempCharacter);
		}
	}	
	if (OtherComp->ComponentHasTag(FName("MINE")))
	{
		ASkill_TrackingMine* Mine = Cast<ASkill_TrackingMine>(OtherComp->GetOwner());
		if(Mine!=nullptr)
		{
		UE_LOG(LogTexture, Error,TEXT("mine"));
		StatComp->SetDamage(Mine->Damage);//�ӽð�:���������� ������
		ATPSAIController* AIController = Cast<ATPSAIController>(GetController());
		if (AIController != nullptr)
		{
			AIController->SetFocus(TempCharacter);
		}
		OtherComp->GetOwner()->SetLifeSpan(0.5f);
		}
	}

	if (StatComp->Health <= 0 && bIsDead==false) //�� ĳ���� �������üũ
	{
		if (bItemSpawn)
		{
			UTPSGameInstance* GameInstance = Cast<UTPSGameInstance>(GetGameInstance());
			int32 RandomNum = GameInstance->RandonNumberRet(0, 9);
			if (GameInstance != nullptr)
			{
				if (RandomNum > 4)
				{
					bItemSpawn = false;
					SpawnArmor();
				}
				else
				{
					bItemSpawn = false;
					SpawnWeapon();
				}
			}
		}
		GetMesh()->SetSimulatePhysics(true);
		ATPSAIController* TPSAIController = Cast<ATPSAIController>(GetController());
		if (TPSAIController != nullptr)
		{
			UBrainComponent* Brain = TPSAIController->GetBrainComponent();
			if (Brain != nullptr)
			{
				Brain->StopLogic(TEXT("DIE"));
			}
			GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
			BulletPool->DestroyBulletPool();
			CurWeapon->Destroy();
			SetLifeSpan(1.5f);
			bIsDead = true;
		}
	}
}

void ATPSEnemy::SpawnWeapon()
{
	FVector RootingLoc = GetActorLocation();
	RootingLoc.Z -=88.0f ;
	auto SpawnWeapon = GetWorld()->SpawnActor<ASpawningWeapon>(RootingLoc, GetActorRotation());
	UE_LOG(LogTexture, Error, TEXT("SpawnWeapon"));
}

void ATPSEnemy::SpawnArmor()
{
	FVector RootingLoc = GetActorLocation();
	RootingLoc.Z -= 88.0f;
	auto SpawnArmor = GetWorld()->SpawnActor<ASpawningArmor>(RootingLoc, GetActorRotation());
	UE_LOG(LogTexture, Error, TEXT("SpawnArmor"));
}




