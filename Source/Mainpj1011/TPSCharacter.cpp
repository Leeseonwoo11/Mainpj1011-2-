// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSCharacter.h"
#include "Weapon.h"
#include "Weapon_Pistol.h"
#include "Weapon_AR.h"
#include "Weapon_SR.h"
#include "DrawDebugHelpers.h"
#include "BulletPoolComponent.h"
#include "TPSCharacterStatComponent.h"
#include "EquipmentComponent.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISense_Sight.h"
#include "InventoryComponent.h"
#include "TableManager.h"
#include "SkillObject.h"
#include "Skill_Pulse.h"
#include "Skill_Support.h"
#include "Skill_TrackingMine.h"
#include "AIController.h"


// Sets default values
ATPSCharacter::ATPSCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	RootComponent = GetCapsuleComponent();
	GetMesh()->SetRelativeLocationAndRotation(FVector(0, 0, -88.0f), FRotator(0, -90, 0));
	GetCapsuleComponent()->ComponentTags.Add(TEXT("PLAYER"));
	static ConstructorHelpers::FObjectFinder<USkeletalMesh>SK_BODY(TEXT("/Game/Mannequin/Character/Mesh/SK_Mannequin"));
	if (SK_BODY.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_BODY.Object);
	}
	
	PrimaryActorTick.bCanEverTick = true;
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(GetCapsuleComponent());
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm);
	SetCameraOption();


	static ConstructorHelpers::FClassFinder<UAnimInstance>ANIMBP_BODY(TEXT("/Game/MyNew/Animation/CharacterAnim/CharacterAnimation"));
	if (ANIMBP_BODY.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(ANIMBP_BODY.Class);
	}

	BulletPool = CreateDefaultSubobject<UBulletPoolComponent>(TEXT("BULLETPOOL"));

	PerceptionSource = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(TEXT("PerceptionSource"));
	PerceptionSource->RegisterForSense(UAISense_Sight::StaticClass());
	PerceptionSource->RegisterWithPerceptionSystem();

	PlayerStatComp = CreateDefaultSubobject<UTPSCharacterStatComponent>(TEXT("PlayerStatComp"));
	Inven = CreateDefaultSubobject<UInventoryComponent>(TEXT("Inventory"));
	EquipmentComponent = CreateDefaultSubobject<UEquipmentComponent>(TEXT("Equipment"));
}

// Called when the game starts or when spawned
void ATPSCharacter::BeginPlay()
{
	Super::BeginPlay();
	Tags.Add(TEXT("PLAYER"));
	GetMesh()->SetGenerateOverlapEvents(true);

	SetWeapon3();
	SetFalseCoverState();
	GetCapsuleComponent()->OnComponentBeginOverlap.AddDynamic(this, &ATPSCharacter::OnComponentBeginOverlap);
}

// Called every frame
void ATPSCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (DirectionToMove.SizeSquared() > 0.0f) //캐릭터 움직이기
	{
		GetController()->SetControlRotation(FRotationMatrix::MakeFromX(DirectionToMove).Rotator()); //캐릭터의 회전
		AddMovementInput(DirectionToMove); //캐릭터 움직임 앞뒤좌우
	}
	CharacterSpeedOption();
	if (CurWeapon != nullptr)
	{
		if (CurWeapon->WeaponType == EWeaponType::AR)
			bAutomaticFire = true;
		else if (CurWeapon->WeaponType == EWeaponType::SR)
			bAutomaticFire = false;
		else if (CurWeapon->WeaponType == EWeaponType::PT)
			bAutomaticFire = false;

		CurAMMO = CurWeapon->AMMO;
		TotalAMMO = CurWeapon->Total_AMMO;
		CurWeaponDamage = CurWeapon->Damage;
	}
	//UE_LOG(LogTemp, Error, TEXT("Player HP = %f"), PlayerStatComp->PlayerHealth);
	if (CurWeapon != nullptr)
	{
		if (CurWeapon->AMMO <= 0)
		{
			GetWorldTimerManager().ClearTimer(FireSpeedTimer);
			bFireState = false;
		}
	}
	if (PlayerStatComp != nullptr)
	{
		PlayerHPRatio = PlayerStatComp->GetHPRatio();
	}
}

// Called to bind functionality to input
void ATPSCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ATPSCharacter::Up);
	PlayerInputComponent->BindAxis("MoveBackward", this, &ATPSCharacter::Down);
	PlayerInputComponent->BindAxis("MoveRight", this, &ATPSCharacter::LeftRight);
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAction("Aim", IE_Pressed,this, &ATPSCharacter::SetTrueAimState);
	PlayerInputComponent->BindAction("Aim", IE_Released, this, &ATPSCharacter::SetFalseAimState);
	PlayerInputComponent->BindAction("Fire", IE_Pressed, this, &ATPSCharacter::SetTrueFireState);
	PlayerInputComponent->BindAction("Fire", IE_Released, this, &ATPSCharacter::SetFalseFireState);
	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ATPSCharacter::SetTrueSprintState);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ATPSCharacter::SetFalseSprintState);
	PlayerInputComponent->BindAction("Weapon1", IE_Pressed, this, &ATPSCharacter::SetWeapon1);
	PlayerInputComponent->BindAction("Weapon2", IE_Pressed, this, &ATPSCharacter::SetWeapon2);
	PlayerInputComponent->BindAction("Weapon3", IE_Pressed, this, &ATPSCharacter::SetWeapon3);
	PlayerInputComponent->BindAction("Covered", IE_Pressed, this, &ATPSCharacter::SetCover);
	PlayerInputComponent->BindAction("DownKey", IE_Pressed, this, &ATPSCharacter::DownKeyPress);
	PlayerInputComponent->BindAction("DownKey", IE_Released, this, &ATPSCharacter::DownKeyRelease);
	PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &ATPSCharacter::preReload); 
	PlayerInputComponent->BindAction("Interaction", IE_Pressed, this, &ATPSCharacter::AddInventory);


}
void ATPSCharacter::SetCameraOption()
{
	SpringArm->TargetArmLength = 200.0f;
	SpringArm->SetRelativeRotation(FRotator(-5.0f, 0.0f, 0.0f));
	SpringArm->SetRelativeLocation(FVector(0.0f, 45.0f, 85.0f));
	SpringArm->bUsePawnControlRotation = true;

	GetCharacterMovement()->RotationRate = FRotator(0.0f, 720.0f, 0.0f);
	GetCharacterMovement()->bOrientRotationToMovement = false;
}

void ATPSCharacter::Up(float NewAxisValue) //상하 움직임 값 셋
{
	if ((Controller != NULL) && (NewAxisValue != 0.0f))
	{
		const FRotator Rotation = GetActorRotation();
		const FVector Direction = FRotationMatrix(Rotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, NewAxisValue);
	}
}
void ATPSCharacter::Down(float NewAxisValue) //상하 움직임 값 셋
{
	if ((Controller != NULL) && (NewAxisValue != 0.0f))
	{
		const FRotator Rotation = GetActorRotation();	
		const FVector Direction = FRotationMatrix(Rotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, NewAxisValue);
	}
	
}
void ATPSCharacter::LeftRight(float NewAxisValue) //좌우 움직임 값 셋
{
	if(NewAxisValue != 0.0f)
	{
		// find out which way is right
		const FRotator Rotation = GetActorRotation();
		const FVector Direction = FRotationMatrix(Rotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, NewAxisValue);
	}
}

//조준상태설정
void ATPSCharacter::SetTrueAimState()
{
	bAimState = true;
	bAimRight = true;
	SpringArm->TargetArmLength = 100.0f;
}
void ATPSCharacter::SetFalseAimState()
{
	SetCameraOption();
	bAimState = false;
}
void ATPSCharacter::ChangeAimLocation()
{
	if (bAimRight) //오른쪽에임이니 왼쪽에임으로 바꾼다.
	{
		bAimRight = false;
		SpringArm->TargetArmLength = 100.0f;
		SpringArm->SetRelativeRotation(FRotator(-5.0f, 10.0f, 0.0f));
		SpringArm->SetRelativeLocation(FVector(0.0f, -65.0f, 85.0f));
		
	}
	else
	{
		bAimRight = true; //왼쪽에임이니 오른쪽에임으로 바꾼다.
		SpringArm->TargetArmLength = 100.0f;
		SpringArm->SetRelativeRotation(FRotator(-5.0f, 0.0f, 0.0f));
		SpringArm->SetRelativeLocation(FVector(0.0f, 45.0f, 85.0f));
	}
}
//발사상태 설정
void ATPSCharacter::SetTrueFireState()
{

	if (CurWeapon != nullptr)
	{
		if (CurWeapon->AMMO <= 0)
		{
			if (bAutoFireRelaodFlag)
			{
				bAutoFireRelaodFlag = false;
				preReload();
			}
		}
	}
	if (!bSprintState)
	{
		if (!IsReload) 
		{
			preFire();
		}
	}
	else if (bAimState) // 질주하다가 조준하면 질주상태가 풀리고 조준상태로 넘어가는데 이때 질주키를 누르고 있으면 발사가 안된다. 이 상황을 방지하기위해
	{					// 추가한 else if문이다.
		if (!IsReload)
		{
			preFire();
		}
	}

}
void ATPSCharacter::SetFalseFireState()
{
	GetWorldTimerManager().ClearTimer(FireSpeedTimer);
	bFireState = false;
}

void ATPSCharacter::preFire() //발사전에 준비
{
	bAutoFireRelaodFlag = true; //자동장전플래그 
	if (CurWeapon != nullptr)
	{
		switch (CurrentWeaponSlot)
		{
		case EWeaponNum::Pistol:
			if (FireCheckFlag == true)
			{
				if (CurWeapon->AMMO > 0)
				{
					FireCheckFlag = false;
					bFireState = true;
					Fire();
					GetWorldTimerManager().SetTimer(FireCheckTimer, this, &ATPSCharacter::FireCheck, (60.0f / (float)CurWeapon->RPM), false, (60.0f / (float)CurWeapon->RPM)); //RPM을 초과하는 연사방지
				}
				else//발사하려는데 총알이 없다면 재장전해라.
				{
					preReload();
				}
			}
			break;
		case EWeaponNum::Weapon1:
			if (WeaponSlot1->WeaponType == EWeaponType::SR)
			{
				if (FireCheckFlag == true)
				{
					if (CurWeapon->AMMO > 0) // 현재 총에 총알이 있다면 발사
					{

						FireCheckFlag = false;
						bFireState = true;
						Fire();
						GetWorldTimerManager().SetTimer(FireCheckTimer, this, &ATPSCharacter::FireCheck, (60.0f / (float)CurWeapon->RPM), false, (60.0f / (float)CurWeapon->RPM)); //RPM을 초과하는 연사방지
					}
					else //발사하려는데 총알이 없다면 재장전해라.
					{
						preReload();
					}
				}
			}
			else if (WeaponSlot1->WeaponType == EWeaponType::AR)
			{
				if (CurWeapon->AMMO > 0) // 현재 총에 총알이 있다면 발사
				{
					float Fireduration = 60.0f / (float)WeaponSlot1->RPM;
					bFireState = true;
					GetWorldTimerManager().SetTimer(FireSpeedTimer, this, &ATPSCharacter::Fire, Fireduration, true);
				}
				else //발사하려는데 총알이 없다면 재장전해라.
				{
					preReload();
				}
			}
			break;
		case EWeaponNum::Weapon2:
			if (WeaponSlot2->WeaponType == EWeaponType::SR)
			{
				if (FireCheckFlag == true)
				{
					if (CurWeapon->AMMO > 0) // 현재 총에 총알이 있다면 발사
					{
						FireCheckFlag = false;
						bFireState = true;
						Fire();
						GetWorldTimerManager().SetTimer(FireCheckTimer, this, &ATPSCharacter::FireCheck, (60.0f / (float)CurWeapon->RPM), false, (60.0f / (float)CurWeapon->RPM)); //RPM을 초과하는 연사방지
					}
					else //발사하려는데 총알이 없다면 재장전해라.
					{
						preReload();
					}
				}
			}
			else if (WeaponSlot2->WeaponType == EWeaponType::AR)
			{
				if (CurWeapon->AMMO > 0) // 현재 총에 총알이 있다면 발사
				{
					float Fireduration = 60.0f / (float)WeaponSlot2->RPM;
					bFireState = true;
					GetWorldTimerManager().SetTimer(FireSpeedTimer, this, &ATPSCharacter::Fire, Fireduration, true);
				}
				else //발사하려는데 총알이 없다면 재장전해라.
				{
					preReload();
				}
			}
			break;
		}
	}
}

void ATPSCharacter::Fire()
{
	//---------------------------발사 로케이션을 찾는 라인트레이싱
	FHitResult OutResult;
	FVector Start = Camera->GetComponentLocation();
	FVector CamForwardVector = Camera->GetForwardVector();
	bool Result = GetWorld()->LineTraceSingleByChannel(OutResult, Start, CamForwardVector*15000.0f + Start, ECC_GameTraceChannel1);
	UGameplayStatics::GetPlayerController(GetWorld(),0)->AddPitchInput(-0.1*CurWeapon->Balance);
	if (Result)
	{
		TargetLoc = OutResult.Location;
		UE_LOG(LogTemp, Error, TEXT("TX = %f, TY = %f, TZ = %f"), TargetLoc.X, TargetLoc.Y, TargetLoc.Z);
	}
	//-----------------------------발사
	if (CurWeapon != nullptr)
	{
		auto TempBullet = BulletPool->GetBulletPtr();
		if (TempBullet != nullptr)
		{
			UE_LOG(LogTemp, Error, TEXT("fire Loc is  x = %f,y = %f, z = %f"), CurWeapon->FirePos->GetComponentLocation().X, CurWeapon->FirePos->GetComponentLocation().Y, CurWeapon->FirePos->GetComponentLocation().Z);

			FVector FireVector = (TargetLoc - CurWeapon->FirePos->GetComponentLocation()).GetSafeNormal();
			FRotator BulletRot = GetActorRotation();
			BulletRot.Pitch = CurWeapon->FirePos->GetComponentRotation().Pitch + 3.0f;

			TempBullet->Damage = CurWeapon->Damage+ PlayerStatComp->PlayerAttackPower; //무기의 데미지 + 플레이어 공격파워
			TempBullet->SetActorLocation(CurWeapon->FirePos->GetComponentLocation());
			TempBullet->SetActorRotation(BulletRot);
			TempBullet->ProjectileMovement->Velocity = FireVector * 12000;
			TempBullet->SetActive(true);
			TempBullet->BulletTrail->Activate(true);

			//DrawDebugLine(GetWorld(), CurWeapon->FirePos->GetComponentLocation(), TargetLoc, FColor::Green, false, 1.0f);
			CurWeapon->AMMO -= 1; // 발사할때마다 현재 총의 총알을 1빼준다.
			UE_LOG(LogTemp, Error, TEXT("Remain Ammo  = %d"), CurWeapon->AMMO);
		}
	}
}

void ATPSCharacter::FireCheck()
{
	FireCheckFlag = true;
}


//질주상태 설정
void ATPSCharacter::SetTrueSprintState()
{
	if (!bAimState && !bFireState)
	{
		if (!IsDown)
		{
			bSprintState = true;
			SetFalseCoverState();
		}
	}
	if (bAimState)
	{
		ChangeAimLocation();
	}
}
void ATPSCharacter::SetFalseSprintState()
{
	bSprintState = false;
}
void ATPSCharacter::DownKeyPress()
{
	IsDown = true;
	bSprintState = false;
}
void ATPSCharacter::DownKeyRelease()
{
	IsDown = false;
}
//무기교체상태 설정
void ATPSCharacter::SetTrueChangeWeaponState()
{
	bChangeWeaponState = true;
}

void ATPSCharacter::SetFalseChangeWeaponState()
{
	bChangeWeaponState = false;
}
//무기교체
void ATPSCharacter::SetWeapon1()
{
	GetWorldTimerManager().ClearTimer(FireSpeedTimer);
	if (WeaponSlot1 != nullptr)
	{
		if (CurrentWeaponSlot != EWeaponNum::Weapon1)
		{
			CurrentWeaponSlot = EWeaponNum::Weapon1;
			CurWeapon = WeaponSlot1;
			bWeapon1 = true;
			bWeapon2 = false;
			bWeapon3 = false;
			SetTrueChangeWeaponState();
			//1번무기 장착
			if (WeaponSlot1 != nullptr)
			{
				if (WeaponSlot1->WeaponType == EWeaponType::AR)
				{
					WeaponSlot1->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("hand_r_ARSocket"));
				}
				else if (WeaponSlot1->WeaponType == EWeaponType::SR)
				{
					WeaponSlot1->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("hand_r_Socket"));
				}
			}
			//2번무기 슬롯으로 이동
			if (WeaponSlot2 != nullptr)
				WeaponSlot2->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("Weapon2_Socket"));
			//권총 슬롯으로 이동
			if (WeaponSlot3 != nullptr)
				WeaponSlot3->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("Pistol_Socket"));
		}
	}
}

void ATPSCharacter::SetWeapon2()
{
	GetWorldTimerManager().ClearTimer(FireSpeedTimer);
	if (WeaponSlot2 != nullptr)
	{
		if (CurrentWeaponSlot != EWeaponNum::Weapon2)
		{
			CurrentWeaponSlot = EWeaponNum::Weapon2;
			CurWeapon = WeaponSlot2;
			bWeapon1 = false;
			bWeapon2 = true;
			bWeapon3 = false;
			SetTrueChangeWeaponState();
			//2번무기 장착
			if (WeaponSlot2 != nullptr)
			{
				if (WeaponSlot2->WeaponType == EWeaponType::AR)
				{
					WeaponSlot2->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("hand_r_ARSocket"));
				}
				else if (WeaponSlot2->WeaponType == EWeaponType::SR)
				{
					WeaponSlot2->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("hand_r_SRSocket"));
				}
			}
			//권총 슬롯으로 이동
			if (WeaponSlot3 != nullptr)
				WeaponSlot3->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("Pistol_Socket"));
			//1번무기 슬롯으로 이동
			if (WeaponSlot1 != nullptr)
				WeaponSlot1->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("Weapon1_Socket"));
		}
	}
}

void ATPSCharacter::SetWeapon3()
{
	GetWorldTimerManager().ClearTimer(FireSpeedTimer);

	if (WeaponSlot3 != nullptr)
	{
		if (CurrentWeaponSlot != EWeaponNum::Pistol)
		{
			CurrentWeaponSlot = EWeaponNum::Pistol;
			CurWeapon = WeaponSlot3;
			bWeapon1 = false;
			bWeapon2 = false;
			bWeapon3 = true;
			SetTrueChangeWeaponState();
			//권총 장착
			if (WeaponSlot3 != nullptr)
				WeaponSlot3->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("hand_r_PistolSocket"));
			//2번무기 슬롯으로 이동
			if (WeaponSlot2 != nullptr)
				WeaponSlot2->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("Weapon2_Socket"));
			//1번무기 슬롯으로 이동
			if (WeaponSlot1 != nullptr)
				WeaponSlot1->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("Weapon1_Socket"));
		}
	}
}
void ATPSCharacter::preReload() // 무기의 종류에 따라서 탄약의 최대 수와 탄창용량이 달라 무기의 타입을 비교 후 재장전함
{

	if (CurWeapon != nullptr)
	{
		switch (CurWeapon->WeaponType)
		{
		case EWeaponType::PT:
			Reload(EWeaponType::PT);
			break;

		case EWeaponType::AR:
			Reload(EWeaponType::AR);
			break;

		case EWeaponType::SR:
			Reload(EWeaponType::SR);
			break;

		default:
			break;
		}

		UE_LOG(LogTexture, Error, TEXT("Current Weapon AMMO is %d"), CurWeapon->AMMO);
		UE_LOG(LogTexture, Error, TEXT("Current Weapon Total AMMO is %d"), CurWeapon->Total_AMMO);
	}
}
void ATPSCharacter::Reload(EWeaponType CurWeaponType)
{
	TableManager* TableMgr = TableManager::GetInstance();
	if (TableMgr != nullptr)
	{
		if (CurWeapon->Total_AMMO > 0)
		{
			if (CurWeapon->Total_AMMO >= CurWeapon->AMMO) // 총 가지고 있는 한탄창보다 많거나 같다면
			{
				if (CurWeapon->AMMO <= 0) //잔탄이 없을때
				{
					IsReload = true;
					CurWeapon->AMMO = TableMgr->GetWeaponAMMO(CurWeaponType);
					CurWeapon->Total_AMMO -= TableMgr->GetWeaponAMMO(CurWeaponType);
				}
				else if (CurWeapon->AMMO == TableMgr->GetWeaponAMMO(CurWeaponType))// 총알이 소모되지않고 그대로 있을때
				{
					//아무것도 안한다.
				}
				else // 잔탄이 있을때
				{
					IsReload = true;
					int32 RemainAmmo = CurWeapon->AMMO; //잔탄저장
					CurWeapon->AMMO = TableMgr->GetWeaponAMMO(CurWeaponType); //탄 채움
					CurWeapon->Total_AMMO -= TableMgr->GetWeaponAMMO(CurWeaponType); // 총 탄약에서 한탄창뺌
					CurWeapon->Total_AMMO += RemainAmmo; // 총탄약에 잔탄 채움
				}
			}
			else if ((CurWeapon->Total_AMMO <= TableMgr->GetWeaponAMMO(CurWeaponType)) && (CurWeapon->Total_AMMO > 0)) // 전체 남은 탄약이 한탄창도 안되지만 0은 아니라면
			{
				IsReload = true;
				CurWeapon->AMMO = CurWeapon->Total_AMMO;
				CurWeapon->Total_AMMO = 0;
			}
			else // 전체 남은 탄약이 없다면
			{
				//아무것도 안한다.
				//사실 이 조건에 들어오지도 못한다.
			}
		}
	}
}
//은엄폐상태 설정
void ATPSCharacter::SetCover()
{
	if (bCovered == false)
		SetTrueCoverState();
	else
		SetFalseCoverState();
}

void ATPSCharacter::SetFalseCoverState()
{
	bCovered = false;
	//SpringArm->SetRelativeLocation(FVector(0.0f, 45.0f, 85.0f));
	GetCapsuleComponent()->SetRelativeScale3D(FVector(1.0f, 1.0f, 1.0f));
	GetMesh()->SetRelativeScale3D(FVector(1.0f, 1.0f, 1.0f));
}

void ATPSCharacter::SetTrueCoverState()
{
	bCovered = true;
	SetFalseSprintState();
	//SpringArm->SetRelativeLocation(FVector(0.0f, 45.0f, 35.0f));
	GetCapsuleComponent()->SetRelativeScale3D(FVector(1.0f, 1.0f, 0.65f));
	GetMesh()->SetRelativeScale3D(FVector(1.0f, 1.0f, 1.54f));
}

void ATPSCharacter::CharacterSpeedOption()
{
	if (bAimState) //조준상태
	{
		if (bCovered) //조준인데 엄폐상태
		{
			GetCharacterMovement()->MaxWalkSpeed = 200;
		}
		else
		{
			GetCharacterMovement()->MaxWalkSpeed = 400;
		}
	}
	else if (bCovered)//엄폐상태
	{
		GetCharacterMovement()->MaxWalkSpeed = 200;
	}
	else if (bSprintState) //질주상태
	{
		GetCharacterMovement()->MaxWalkSpeed = 1400;
	}
	else //일반상태
	{
		GetCharacterMovement()->MaxWalkSpeed = 1000;
	}
}


//에임오프셋 리턴함수
FRotator ATPSCharacter::GetAimOffsets() const
{
	const FVector AimDirWS = GetBaseAimRotation().Vector();
	const FVector AimDirLS = ActorToWorld().InverseTransformVectorNoScale(AimDirWS);
	const FRotator AimRotLS = AimDirLS.Rotation();

	return AimRotLS;
}

void ATPSCharacter::OnComponentBeginOverlap(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherComp->ComponentHasTag(FName("BULLET")))
	{
		PlayerStatComp->SetDamage((float)Cast<ABullet>(OtherComp->GetOwner())->Damage);
	}
}

bool ATPSCharacter::CanBeSeenFrom(const FVector & ObserverLocation, FVector & OutSeenLocation, int32 & NumberOfLoSChecksPerformed, float & OutSightStrength, const AActor * IgnoreActor) const
{
	FHitResult HitResult;
	FVector SocketLocation = GetMesh()->GetSocketLocation("neck_01");
	const bool bHitSocket = GetWorld()->LineTraceSingleByChannel(HitResult, ObserverLocation, SocketLocation, ECC_GameTraceChannel5);
	NumberOfLoSChecksPerformed++;
	if (bHitSocket == true)
	{
		if (HitResult.Actor.IsValid() && HitResult.Actor->IsOwnedBy(this))
		{
			OutSeenLocation = SocketLocation;
			OutSightStrength = 1;
			UE_LOG(LogTemp, Warning, TEXT("TRUE"));
			return true;
		}
	}

	UE_LOG(LogTemp, Warning, TEXT("FALSE"));
	OutSightStrength = 0;
	return false;
}

void ATPSCharacter::AddInventory()
{
	for (auto TempArmor : TempArmorArray) // TempArmorArray는 SpawningArmor액터에서 Add와 Remove가 실행된다.
	{										//캐릭터와 겹치면 추가되고 겹침이끝나면 제거된다.
		if (TempArmor->IsEatableItem)
		{

			if (Inven->AddInventroyArmor(TempArmor->ArmorProperty))
			{
				TempArmor->IsEattenItem = true;
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("NoAddInven"));
		}
	}
	for (auto TempWeapon : TempWeaponArray)
	{
		if (TempWeapon->IsEatableItem)
		{
			if (Inven->AddInventroyWeapon(TempWeapon->WeaponProperty))
			{
				TempWeapon->IsEattenItem = true;
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("NoAddInven"));
		}
	}
}

void ATPSCharacter::SetSkill_Q(int32 num)
{
	for (int32 i = 0; i < InputComponent->GetNumActionBindings(); i++) //Q에 바인드 되어있던 스킬이 있다면 초기화 해준다.
	{
		FInputActionBinding Binding = InputComponent->GetActionBinding(i);
		if (Binding.GetActionName() == Skill_Q_Binding.GetActionName())
		{
			InputComponent->RemoveActionBinding(i);
		}
	}

	if (num == 1) //펄스
	{
		Skill_Q = num;
		Skill_Q_Binding.ActionDelegate.BindDelegate(this, &ATPSCharacter::SpawnPulseDetector);
		InputComponent->AddActionBinding(Skill_Q_Binding);
		if (Skill_E == 1) // Q에 저장된 스킬과 같은 스킬이 E에도 저장되 있다면 E에 저장된 스킬을 삭제해야 한다.
		{
			Skill_E = 0;
			SetSkill_E(0); // E에 바인드된 스킬을 삭제하기 위해서 아무의미없는 0을 넣어 스킬셋 함수를 불러준다.
		}
	}
	else if (num == 2) // 유도지뢰
	{
		Skill_Q = num;
		Skill_Q_Binding.ActionDelegate.BindDelegate(this, &ATPSCharacter::SpawnTrackingMine);
		InputComponent->AddActionBinding(Skill_Q_Binding);
		if (Skill_E == 2)
		{
			Skill_E = 0;
			SetSkill_E(0);
		}
	}
	else if (num == 3) //지원소
	{
		Skill_Q = num;
		Skill_Q_Binding.ActionDelegate.BindDelegate(this, &ATPSCharacter::SpawnSupport);
		InputComponent->AddActionBinding(Skill_Q_Binding);
		if (Skill_E == 3)
		{
			Skill_E = 0;
			SetSkill_E(0);
		}
	}
}

void ATPSCharacter::SetSkill_E(int32 num)
{
	for (int32 i = 0; i < InputComponent->GetNumActionBindings(); i++) // E 스킬 바인드 초기화
	{
		FInputActionBinding Binding = InputComponent->GetActionBinding(i);
		if (Binding.GetActionName() == Skill_E_Binding.GetActionName())
		{
			InputComponent->RemoveActionBinding(i);
		}
	}

	if (num == 1)// 1번은 펄스
	{
		Skill_E = num;
		Skill_E_Binding.ActionDelegate.BindDelegate(this, &ATPSCharacter::SpawnPulseDetector);
		InputComponent->AddActionBinding(Skill_E_Binding);
		if (Skill_Q == 1)
		{
			Skill_Q = 0;
			SetSkill_Q(0);
		}
	}
	else if (num == 2) //2번은 유도지뢰
	{
		Skill_E = num;
		Skill_E_Binding.ActionDelegate.BindDelegate(this, &ATPSCharacter::SpawnTrackingMine);
		InputComponent->AddActionBinding(Skill_E_Binding);
		if (Skill_Q == 2)
		{
			Skill_Q = 0;
			SetSkill_Q(0);
		}
	}
	else if (num == 3) //3번은 지원소
	{
		Skill_E = num;
		Skill_E_Binding.ActionDelegate.BindDelegate(this, &ATPSCharacter::SpawnSupport);
		InputComponent->AddActionBinding(Skill_E_Binding);
		if (Skill_Q == 3)
		{
			Skill_Q = 0;
			SetSkill_Q(0);
		}
	}
}

void ATPSCharacter::SpawnTrackingMine()
{
	FActorSpawnParameters Spawnparam;
	Spawnparam.bNoFail = true;
	if (bMineCooldown)
	{
		bMineCooldown = false;
		auto TrackingMine = GetWorld()->SpawnActor<ASkill_TrackingMine>(GetActorLocation(), FRotator::ZeroRotator, Spawnparam);
		MineCoolTime = TrackingMine->Cooltime;
		MineTimeValue = 0.0f;
		GetWorldTimerManager().SetTimer(MineTimer,this, &ATPSCharacter::MineCooltimeFunc, 0.1f,true);
	}
}

void ATPSCharacter::SpawnSupport()
{
	FActorSpawnParameters Spawnparam;
	Spawnparam.bNoFail = true;
	if (bSupportCooldown)
	{
		bSupportCooldown = false;
		auto Support = GetWorld()->SpawnActor<ASkill_Support>(GetActorLocation(), FRotator::ZeroRotator, Spawnparam);
		SupportCoolTime = Support->Cooltime;
		SupportTimeValue = 0.0f;
		GetWorldTimerManager().SetTimer(SupportTimer, this, &ATPSCharacter::SupportCooltimeFunc, 0.1f, true);
	}
}

void ATPSCharacter::SpawnPulseDetector()
{
	FActorSpawnParameters Spawnparam;
	Spawnparam.bNoFail = true;
	if (bPulseCooldown)
	{
		bPulseCooldown = false;
		auto PulseDetector = GetWorld()->SpawnActor<ASkill_Pulse>(GetActorLocation(), FRotator::ZeroRotator, Spawnparam);
		PulseCoolTime = PulseDetector->Cooltime;
		PulseTimeValue = 0.0f;
		GetWorldTimerManager().SetTimer(PulseTimer, this, &ATPSCharacter::PulseCooltimeFunc, 0.1f, true);
	}
}

void ATPSCharacter::NoSkill()
{
	UE_LOG(LogTemp, Error, TEXT("NoSkill"));
}

void ATPSCharacter::PulseCooltimeFunc()
{
	PulseTimeValue += 0.1f;
	if (PulseCoolTime <= PulseTimeValue)
	{
		bPulseCooldown = true;
		GetWorldTimerManager().ClearTimer(PulseTimer);
	}
}

void ATPSCharacter::MineCooltimeFunc()
{
	MineTimeValue+=0.1f;
	if (MineCoolTime <= MineTimeValue)
	{
		bMineCooldown = true;
		GetWorldTimerManager().ClearTimer(MineTimer);
	}
}

void ATPSCharacter::SupportCooltimeFunc()
{
	SupportTimeValue += 0.1f;
	if (SupportCoolTime <= SupportTimeValue)
	{
		bSupportCooldown = true;
		GetWorldTimerManager().ClearTimer(SupportTimer);
	}
}





