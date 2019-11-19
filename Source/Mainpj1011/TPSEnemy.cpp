// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSEnemy.h"
#include "Weapon_AR.h"
#include "DrawDebugHelpers.h"
#include "TPSAIController.h"
#include "Kismet/KismetMathLibrary.h"
#include "EnemyHPBarWidget.h"
#include "AIModule/Classes/BrainComponent.h"
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
	EnemyHPWidget->SetupAttachment(GetCapsuleComponent());
	EnemyHPWidget->SetRelativeLocation(FVector(0.0f, 0.0f, 180.0f));
	static ConstructorHelpers::FObjectFinder<UClass>UW_HPBar(TEXT("/Game/MyNew/HUD/EnemyHPBar.EnemyHPBar_C"));
	if (UW_HPBar.Succeeded())
	{
		Widget = UW_HPBar.Object;
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

	//총알 발사 지점~ 
	ATPSAIController* TPSAIController = Cast<ATPSAIController>(GetController());
	if (TPSAIController != nullptr)
	{
		if (TPSAIController->CanSeePlayerAI)
		{
			TargetLoc = TempCharacter->GetMesh()->GetSocketLocation(FName("neck_01")); // Fire함수에서 총알을 발사할 위치
		}
	}
	FRotator WidgetRot = UKismetMathLibrary::FindLookAtRotation(EnemyHPWidget->GetComponentLocation(), TempCharacter->GetMesh()->GetComponentLocation());
	EnemyHPWidget->SetWorldRotation(WidgetRot); // 위젯 캐릭터가 볼수있게 돌리기
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
				TempBullet->ProjectileMovement->Velocity = FireVector * 12000;
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
	if (StatComp->Health <= 0)
	{
		GetMesh()->SetSimulatePhysics(true);
		
		UBrainComponent* Brain = Cast<ATPSAIController>(GetController())->GetBrainComponent();
		if (Brain != nullptr)
		{
			Brain->StopLogic(TEXT("DIE"));
		}
		GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::Type::NoCollision);
		BulletPool->DestroyBulletPool();
		CurWeapon->Destroy();
		SetLifeSpan(1.5f);
	}
}




