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
#include "TPSSoundComponent.h"



// Sets default values
ATPSEnemy::ATPSEnemy()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -88.0f), FRotator(0, -90, 0));
	GetCapsuleComponent()->ComponentTags.Add(TEXT("ENEMY"));
	
	//매시 지정
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>SK_BODY(TEXT("/Game/Mannequin/Character/Mesh/SK_Mannequin"));
	if (SK_BODY.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_BODY.Object);
	}
	GetMesh()->SetCollisionProfileName(FName("Ragdoll"));
	
	//애니메이션 지정
	static ConstructorHelpers::FClassFinder<UAnimInstance>ANIMBP_BODY(TEXT("/Game/MyNew/Animation/EnemyAnim/EnemyAnimation"));
	if (ANIMBP_BODY.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(ANIMBP_BODY.Class);
	}
	
	BulletPool = CreateDefaultSubobject<UBulletPoolComponent>(TEXT("BULLETPOOL")); // 총알풀 총알을 발사 할때 풀에서 꺼내 사용하고 사용이 끝난 총알은 비활성화한 후 재사용한다.

	StatComp = CreateDefaultSubobject<UTPSEnemyStatComponent>(TEXT("STATCOMP")); // Enmey의 스탯(HP)을 관리하는 컴포넌트

	AIControllerClass = ATPSAIController::StaticClass();

	//HP위젯 지정
	EnemyHPWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HPWIDGET"));
	EnemyHPWidget->SetupAttachment(GetMesh());
	EnemyHPWidget->SetRelativeLocation(FVector(0.0f, 0.0f, 180.0f));
	static ConstructorHelpers::FObjectFinder<UClass>UW_HPBar(TEXT("/Game/MyNew/HUD/EnemyHPBar.EnemyHPBar_C"));
	if (UW_HPBar.Succeeded())
	{
		Widget = UW_HPBar.Object;
	}
	// Enemy의 몸통 머터리얼 지정
	static ConstructorHelpers::FObjectFinder<UMaterial>M_BODY(TEXT("/Game/MyNew/Material/M_EnemyBody"));
	if(M_BODY.Succeeded())
	{ 
		GetMesh()->SetMaterial(0, M_BODY.Object);
	}
	//사운드 컴포넌트 발사 히트 발걸음 
	SoundComp = CreateDefaultSubobject<UTPSSoundComponent>(TEXT("SoundComponent"));
	SoundComp->ARShotSound->SetupAttachment(GetCapsuleComponent());
	SoundComp->HitSound->SetupAttachment(GetCapsuleComponent());
	SoundComp->StepSound->SetupAttachment(GetCapsuleComponent());
}

// Called when the game starts or when spawned
void ATPSEnemy::BeginPlay()
{
	Super::BeginPlay();
	// 적은 기본적으로 돌격소총을 무장함

	auto AssaultRifle = GetWorld()->SpawnActor<AWeapon_AR>(FVector::ZeroVector, FRotator::ZeroRotator); // 돌격소총 스폰
	CurWeapon = AssaultRifle;// 적의 현재무기 스폰한 돌격소총
	if (AssaultRifle != nullptr)
	{
		AssaultRifle->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("hand_r_ARSocket")); //  소캣에 붙여준다.
	}
	TempCharacter = Cast<ATPSCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0)); // 임시 플레이어캐릭터의 포인터
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &ATPSEnemy::OnComponentBeginOverlap);// 캡슐컴포넌트의 겹침시 이벤트 활성화
	bIsDead = false; // 죽음상태 false;
}

void ATPSEnemy::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	UUserWidget* HPBar = CreateWidget<UUserWidget>(GetWorld(), Widget); //생성자에서 미러 지정한 블루프린트 위젯을 생성
	EnemyHPWidget->SetWidget(HPBar); // 위젯컴포넌트에 생성한 위젯을 지정해준다.
	EnemyHPWidget->SetDrawSize(FVector2D(150.0f, 50.0f)); //위젯 사이즈 설정
	auto EnemyWidget = Cast<UEnemyHPBarWidget>(EnemyHPWidget->GetUserWidgetObject());
	if (EnemyWidget != nullptr)
	{
		EnemyWidget->BindEnemyHP(StatComp); // 위젯에 적AI의 스텟컴포넌트를 바인드 한다. (c++로 만든 EnemyHPBarWidget에 있는함수)
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
			TargetLoc = TempCharacter->GetMesh()->GetSocketLocation(FName("neck_01")); // Fire함수에서 총알을 발사할 위치 , 플레이어의 목 소캣위치에 총을 쏜다.
		}
	}
	FRotator WidgetRot = UKismetMathLibrary::FindLookAtRotation(EnemyHPWidget->GetComponentLocation(), TempCharacter->GetMesh()->GetComponentLocation());
	EnemyHPWidget->SetWorldRotation(WidgetRot); // 위젯은 항상 캐릭터가 볼수있도록 회전한다.
}

// Called to bind functionality to input
void ATPSEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
}

void ATPSEnemy::SetFalseFireState() //발사 멈춤상태
{
	bFireState = false;
	GetWorldTimerManager().ClearTimer(FireSpeedTimer1);
}

void ATPSEnemy::SetTrueFireState()// 발사상태
{
	bFireState = true;
	preFire();
}

void ATPSEnemy::preFire()//발사준비
{
	float Fireduration = 60.0f / (float)CurWeapon->RPM; //총의 발사속도에 맞게 발사주기 설정
	GetWorldTimerManager().SetTimer(FireSpeedTimer1, this, &ATPSEnemy::Fire, Fireduration, true); //  발사주기에 맞게 발사 타이머 설정
}

void ATPSEnemy::Fire() //발사함수 (1발)
{
	if (CurWeapon != nullptr)
	{
		auto TempBullet = BulletPool->GetBulletPtr();
		if (TempBullet != nullptr)
		{
			//발사사운드 실행
			SoundComp->ARShotSound->Play();
			//적 발견시 발사
			if (!Cast<ATPSAIController>(GetController())->CanSeePlayerAI)
			{
				FRotator BulletRot = GetActorRotation();
				FVector FireVector = GetActorForwardVector();
				BulletRot.Pitch = CurWeapon->FirePos->GetComponentRotation().Pitch + 3.0f; //총알 피치값만 재설정(총알이 비정상적으로 날아감 방지)
				TempBullet->Damage = CurWeapon->Damage; //총알 데미지 설정
				TempBullet->SetActorLocation(CurWeapon->FirePos->GetComponentLocation());// 총알 발사 초기위치 총의 총구로 설정함
				TempBullet->SetActorRotation(BulletRot);// 총알의 회전값 설정
				TempBullet->ProjectileMovement->Velocity = FireVector * 6000; //총알의 속도설정
				TempBullet->ProjectileMovement->Velocity.Z = (CurWeapon->FirePos->GetComponentLocation().Z - TargetLoc.Z); // 총알의 속도에 Z값은 커지지 않도록 재설정(이렇게 해야 총알이 맞는다.)
				TempBullet->SetActive(true); //총알 활성화
				TempBullet->BulletTrail->Activate(true); // 총알 파티클 활성화
			}
			//적 발견은 못햇지만 적방향으로 발사.(위협사격)
			else
			{
				FRotator BulletRot = GetActorRotation();
				FVector FireVector = (TargetLoc - CurWeapon->FirePos->GetComponentLocation()).GetSafeNormal();
				BulletRot.Pitch = CurWeapon->FirePos->GetComponentRotation().Pitch + 3.0f;
				TempBullet->Damage = CurWeapon->Damage;
				TempBullet->SetActorLocation(CurWeapon->FirePos->GetComponentLocation());
				TempBullet->SetActorRotation(BulletRot);
				TempBullet->ProjectileMovement->Velocity = FireVector * 151200; // 적의 총알의 속도는 너무빠르면 플레이어가 보이면 플레이어는 죽음 그래서 총알 속도가 더 적다.
				TempBullet->SetActive(true);
				TempBullet->BulletTrail->Activate(true);
				DrawDebugLine(GetWorld(), CurWeapon->FirePos->GetComponentLocation(), TargetLoc, FColor::Red, false, 1.0f);
			}
			UE_LOG(LogTemp, Error, TEXT("FIRE + %d"), count++);
		}
	}
}


void ATPSEnemy::SetTrueCoverState()// 엄페상태  true
{
	bCrouchState = true;
	GetCapsuleComponent()->SetRelativeScale3D(FVector(1.0f, 1.0f, 0.65f)); // 캡슐콜리젼의 높이를 줄인다.
	GetMesh()->SetRelativeScale3D(FVector(1.0f, 1.0f, 1.54f)); // 스켈레탈메시의 높이를 줄여준다.
}

void ATPSEnemy::SetFalseCoverState() //엄페상태false
{
	bCrouchState = false;
	GetCapsuleComponent()->SetRelativeScale3D(FVector(1.0f, 1.0f, 1.0f)); // 캡슐콜리젼 높이를 원래대로 복구
	GetMesh()->SetRelativeScale3D(FVector(1.0f, 1.0f, 1.0f)); //  스켈레탈메시의 높이를 원래대로 복구
}

void ATPSEnemy::SetTrueAimState() 
{
	bAimState = true;
}

void ATPSEnemy::SetFalseAimState() 
{
	bAimState = false;
}
//행동변화 불값 변경 함수 (애니메이션에서 사용)
void ATPSEnemy::CStartShooting()// 발사시작하기
{
	SetTrueFireState();
}
void ATPSEnemy::CStopShoothing() // 발사중지하기
{
	SetFalseFireState();
}
void ATPSEnemy::CEnterCover() // 엄페시작하기
{
	SetTrueCoverState();
}
void ATPSEnemy::COutCover() //엠페중지하기
{
	SetFalseCoverState();
}

void ATPSEnemy::OnComponentBeginOverlap(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherComp->ComponentHasTag(FName("BULLET")))//총알감지
	{
		//총알 피격시 소리
		SoundComp->HitSound->Play();
		//총알 피격시 데미지입기.
		StatComp->SetDamage((float)Cast<ABullet>(OtherComp->GetOwner())->Damage);
		ATPSAIController* AIController = Cast<ATPSAIController>(GetController());
		if (AIController != nullptr)
		{
			AIController->SetFocus(TempCharacter);
		}
	}	
	if (OtherComp->ComponentHasTag(FName("MINE")))//지뢰감지
	{
		ASkill_TrackingMine* Mine = Cast<ASkill_TrackingMine>(OtherComp->GetOwner());
		if(Mine!=nullptr)
		{
		UE_LOG(LogTexture, Error,TEXT("mine"));
		StatComp->SetDamage(Mine->Damage);//임시값:유도지뢰의 데미지
		ATPSAIController* AIController = Cast<ATPSAIController>(GetController());
		if (AIController != nullptr)
		{
			AIController->SetFocus(TempCharacter);
		}
		OtherComp->GetOwner()->SetLifeSpan(0.5f);
		}
	}

	if (StatComp->Health <= 0 && bIsDead==false) //적 캐릭터 사망유무체크
	{
		if (bItemSpawn) // 아이템 스폰가능 여부체크
		{
			UTPSGameInstance* GameInstance = Cast<UTPSGameInstance>(GetGameInstance());
			int32 RandomNum = GameInstance->RandonNumberRet(0, 9); //랜덤한 수를 뽑아온다.
			if (GameInstance != nullptr)
			{
				if (RandomNum > 4)// 4보다 크다면 방어구 스폰
				{
					bItemSpawn = false;
					SpawnArmor();
				}
				else // 아니라면 무기스폰
				{
					bItemSpawn = false;
					SpawnWeapon();
				}
			}
			// 킬스코어 증가
			GameInstance->AddKillScore(10);
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

void ATPSEnemy::SpawnWeapon() // 무기스폰 함수.
{
	FVector RootingLoc = GetActorLocation(); // 스폰할 위치
	RootingLoc.Z -=88.0f ; // 높이 재설정
	auto SpawnWeapon = GetWorld()->SpawnActor<ASpawningWeapon>(RootingLoc, GetActorRotation());//스폰
	UE_LOG(LogTexture, Error, TEXT("SpawnWeapon"));
}

void ATPSEnemy::SpawnArmor()
{
	FVector RootingLoc = GetActorLocation();
	RootingLoc.Z -= 88.0f;
	auto SpawnArmor = GetWorld()->SpawnActor<ASpawningArmor>(RootingLoc, GetActorRotation());
	UE_LOG(LogTexture, Error, TEXT("SpawnArmor"));
}

void ATPSEnemy::PlayStepSound()// 발소리 플레이 함수. 에님노티파이로 실행된다.
{
	SoundComp->StepSound->Play();
}



