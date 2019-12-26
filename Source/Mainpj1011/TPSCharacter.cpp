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
	if (DirectionToMove.SizeSquared() > 0.0f) //ĳ���� �����̱�
	{
		GetController()->SetControlRotation(FRotationMatrix::MakeFromX(DirectionToMove).Rotator()); //ĳ������ ȸ��
		AddMovementInput(DirectionToMove); //ĳ���� ������ �յ��¿�
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

void ATPSCharacter::Up(float NewAxisValue) //���� ������ �� ��
{
	if ((Controller != NULL) && (NewAxisValue != 0.0f))
	{
		const FRotator Rotation = GetActorRotation();
		const FVector Direction = FRotationMatrix(Rotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, NewAxisValue);
	}
}
void ATPSCharacter::Down(float NewAxisValue) //���� ������ �� ��
{
	if ((Controller != NULL) && (NewAxisValue != 0.0f))
	{
		const FRotator Rotation = GetActorRotation();	
		const FVector Direction = FRotationMatrix(Rotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, NewAxisValue);
	}
	
}
void ATPSCharacter::LeftRight(float NewAxisValue) //�¿� ������ �� ��
{
	if(NewAxisValue != 0.0f)
	{
		// find out which way is right
		const FRotator Rotation = GetActorRotation();
		const FVector Direction = FRotationMatrix(Rotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Direction, NewAxisValue);
	}
}

//���ػ��¼���
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
	if (bAimRight) //�����ʿ����̴� ���ʿ������� �ٲ۴�.
	{
		bAimRight = false;
		SpringArm->TargetArmLength = 100.0f;
		SpringArm->SetRelativeRotation(FRotator(-5.0f, 10.0f, 0.0f));
		SpringArm->SetRelativeLocation(FVector(0.0f, -65.0f, 85.0f));
		
	}
	else
	{
		bAimRight = true; //���ʿ����̴� �����ʿ������� �ٲ۴�.
		SpringArm->TargetArmLength = 100.0f;
		SpringArm->SetRelativeRotation(FRotator(-5.0f, 0.0f, 0.0f));
		SpringArm->SetRelativeLocation(FVector(0.0f, 45.0f, 85.0f));
	}
}
//�߻���� ����
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
	else if (bAimState) // �����ϴٰ� �����ϸ� ���ֻ��°� Ǯ���� ���ػ��·� �Ѿ�µ� �̶� ����Ű�� ������ ������ �߻簡 �ȵȴ�. �� ��Ȳ�� �����ϱ�����
	{					// �߰��� else if���̴�.
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

void ATPSCharacter::preFire() //�߻����� �غ�
{
	bAutoFireRelaodFlag = true; //�ڵ������÷��� 
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
					GetWorldTimerManager().SetTimer(FireCheckTimer, this, &ATPSCharacter::FireCheck, (60.0f / (float)CurWeapon->RPM), false, (60.0f / (float)CurWeapon->RPM)); //RPM�� �ʰ��ϴ� �������
				}
				else//�߻��Ϸ��µ� �Ѿ��� ���ٸ� �������ض�.
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
					if (CurWeapon->AMMO > 0) // ���� �ѿ� �Ѿ��� �ִٸ� �߻�
					{

						FireCheckFlag = false;
						bFireState = true;
						Fire();
						GetWorldTimerManager().SetTimer(FireCheckTimer, this, &ATPSCharacter::FireCheck, (60.0f / (float)CurWeapon->RPM), false, (60.0f / (float)CurWeapon->RPM)); //RPM�� �ʰ��ϴ� �������
					}
					else //�߻��Ϸ��µ� �Ѿ��� ���ٸ� �������ض�.
					{
						preReload();
					}
				}
			}
			else if (WeaponSlot1->WeaponType == EWeaponType::AR)
			{
				if (CurWeapon->AMMO > 0) // ���� �ѿ� �Ѿ��� �ִٸ� �߻�
				{
					float Fireduration = 60.0f / (float)WeaponSlot1->RPM;
					bFireState = true;
					GetWorldTimerManager().SetTimer(FireSpeedTimer, this, &ATPSCharacter::Fire, Fireduration, true);
				}
				else //�߻��Ϸ��µ� �Ѿ��� ���ٸ� �������ض�.
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
					if (CurWeapon->AMMO > 0) // ���� �ѿ� �Ѿ��� �ִٸ� �߻�
					{
						FireCheckFlag = false;
						bFireState = true;
						Fire();
						GetWorldTimerManager().SetTimer(FireCheckTimer, this, &ATPSCharacter::FireCheck, (60.0f / (float)CurWeapon->RPM), false, (60.0f / (float)CurWeapon->RPM)); //RPM�� �ʰ��ϴ� �������
					}
					else //�߻��Ϸ��µ� �Ѿ��� ���ٸ� �������ض�.
					{
						preReload();
					}
				}
			}
			else if (WeaponSlot2->WeaponType == EWeaponType::AR)
			{
				if (CurWeapon->AMMO > 0) // ���� �ѿ� �Ѿ��� �ִٸ� �߻�
				{
					float Fireduration = 60.0f / (float)WeaponSlot2->RPM;
					bFireState = true;
					GetWorldTimerManager().SetTimer(FireSpeedTimer, this, &ATPSCharacter::Fire, Fireduration, true);
				}
				else //�߻��Ϸ��µ� �Ѿ��� ���ٸ� �������ض�.
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
	//---------------------------�߻� �����̼��� ã�� ����Ʈ���̽�
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
	//-----------------------------�߻�
	if (CurWeapon != nullptr)
	{
		auto TempBullet = BulletPool->GetBulletPtr();
		if (TempBullet != nullptr)
		{
			UE_LOG(LogTemp, Error, TEXT("fire Loc is  x = %f,y = %f, z = %f"), CurWeapon->FirePos->GetComponentLocation().X, CurWeapon->FirePos->GetComponentLocation().Y, CurWeapon->FirePos->GetComponentLocation().Z);

			FVector FireVector = (TargetLoc - CurWeapon->FirePos->GetComponentLocation()).GetSafeNormal();
			FRotator BulletRot = GetActorRotation();
			BulletRot.Pitch = CurWeapon->FirePos->GetComponentRotation().Pitch + 3.0f;

			TempBullet->Damage = CurWeapon->Damage+ PlayerStatComp->PlayerAttackPower; //������ ������ + �÷��̾� �����Ŀ�
			TempBullet->SetActorLocation(CurWeapon->FirePos->GetComponentLocation());
			TempBullet->SetActorRotation(BulletRot);
			TempBullet->ProjectileMovement->Velocity = FireVector * 12000;
			TempBullet->SetActive(true);
			TempBullet->BulletTrail->Activate(true);

			//DrawDebugLine(GetWorld(), CurWeapon->FirePos->GetComponentLocation(), TargetLoc, FColor::Green, false, 1.0f);
			CurWeapon->AMMO -= 1; // �߻��Ҷ����� ���� ���� �Ѿ��� 1���ش�.
			UE_LOG(LogTemp, Error, TEXT("Remain Ammo  = %d"), CurWeapon->AMMO);
		}
	}
}

void ATPSCharacter::FireCheck()
{
	FireCheckFlag = true;
}


//���ֻ��� ����
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
//���ⱳü���� ����
void ATPSCharacter::SetTrueChangeWeaponState()
{
	bChangeWeaponState = true;
}

void ATPSCharacter::SetFalseChangeWeaponState()
{
	bChangeWeaponState = false;
}
//���ⱳü
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
			//1������ ����
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
			//2������ �������� �̵�
			if (WeaponSlot2 != nullptr)
				WeaponSlot2->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("Weapon2_Socket"));
			//���� �������� �̵�
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
			//2������ ����
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
			//���� �������� �̵�
			if (WeaponSlot3 != nullptr)
				WeaponSlot3->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("Pistol_Socket"));
			//1������ �������� �̵�
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
			//���� ����
			if (WeaponSlot3 != nullptr)
				WeaponSlot3->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("hand_r_PistolSocket"));
			//2������ �������� �̵�
			if (WeaponSlot2 != nullptr)
				WeaponSlot2->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("Weapon2_Socket"));
			//1������ �������� �̵�
			if (WeaponSlot1 != nullptr)
				WeaponSlot1->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("Weapon1_Socket"));
		}
	}
}
void ATPSCharacter::preReload() // ������ ������ ���� ź���� �ִ� ���� źâ�뷮�� �޶� ������ Ÿ���� �� �� ��������
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
			if (CurWeapon->Total_AMMO >= CurWeapon->AMMO) // �� ������ �ִ� ��źâ���� ���ų� ���ٸ�
			{
				if (CurWeapon->AMMO <= 0) //��ź�� ������
				{
					IsReload = true;
					CurWeapon->AMMO = TableMgr->GetWeaponAMMO(CurWeaponType);
					CurWeapon->Total_AMMO -= TableMgr->GetWeaponAMMO(CurWeaponType);
				}
				else if (CurWeapon->AMMO == TableMgr->GetWeaponAMMO(CurWeaponType))// �Ѿ��� �Ҹ�����ʰ� �״�� ������
				{
					//�ƹ��͵� ���Ѵ�.
				}
				else // ��ź�� ������
				{
					IsReload = true;
					int32 RemainAmmo = CurWeapon->AMMO; //��ź����
					CurWeapon->AMMO = TableMgr->GetWeaponAMMO(CurWeaponType); //ź ä��
					CurWeapon->Total_AMMO -= TableMgr->GetWeaponAMMO(CurWeaponType); // �� ź�࿡�� ��źâ��
					CurWeapon->Total_AMMO += RemainAmmo; // ��ź�࿡ ��ź ä��
				}
			}
			else if ((CurWeapon->Total_AMMO <= TableMgr->GetWeaponAMMO(CurWeaponType)) && (CurWeapon->Total_AMMO > 0)) // ��ü ���� ź���� ��źâ�� �ȵ����� 0�� �ƴ϶��
			{
				IsReload = true;
				CurWeapon->AMMO = CurWeapon->Total_AMMO;
				CurWeapon->Total_AMMO = 0;
			}
			else // ��ü ���� ź���� ���ٸ�
			{
				//�ƹ��͵� ���Ѵ�.
				//��� �� ���ǿ� �������� ���Ѵ�.
			}
		}
	}
}
//��������� ����
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
	if (bAimState) //���ػ���
	{
		if (bCovered) //�����ε� �������
		{
			GetCharacterMovement()->MaxWalkSpeed = 200;
		}
		else
		{
			GetCharacterMovement()->MaxWalkSpeed = 400;
		}
	}
	else if (bCovered)//�������
	{
		GetCharacterMovement()->MaxWalkSpeed = 200;
	}
	else if (bSprintState) //���ֻ���
	{
		GetCharacterMovement()->MaxWalkSpeed = 1400;
	}
	else //�Ϲݻ���
	{
		GetCharacterMovement()->MaxWalkSpeed = 1000;
	}
}


//���ӿ����� �����Լ�
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
	for (auto TempArmor : TempArmorArray) // TempArmorArray�� SpawningArmor���Ϳ��� Add�� Remove�� ����ȴ�.
	{										//ĳ���Ϳ� ��ġ�� �߰��ǰ� ��ħ�̳����� ���ŵȴ�.
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
	for (int32 i = 0; i < InputComponent->GetNumActionBindings(); i++) //Q�� ���ε� �Ǿ��ִ� ��ų�� �ִٸ� �ʱ�ȭ ���ش�.
	{
		FInputActionBinding Binding = InputComponent->GetActionBinding(i);
		if (Binding.GetActionName() == Skill_Q_Binding.GetActionName())
		{
			InputComponent->RemoveActionBinding(i);
		}
	}

	if (num == 1) //�޽�
	{
		Skill_Q = num;
		Skill_Q_Binding.ActionDelegate.BindDelegate(this, &ATPSCharacter::SpawnPulseDetector);
		InputComponent->AddActionBinding(Skill_Q_Binding);
		if (Skill_E == 1) // Q�� ����� ��ų�� ���� ��ų�� E���� ����� �ִٸ� E�� ����� ��ų�� �����ؾ� �Ѵ�.
		{
			Skill_E = 0;
			SetSkill_E(0); // E�� ���ε�� ��ų�� �����ϱ� ���ؼ� �ƹ��ǹ̾��� 0�� �־� ��ų�� �Լ��� �ҷ��ش�.
		}
	}
	else if (num == 2) // ��������
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
	else if (num == 3) //������
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
	for (int32 i = 0; i < InputComponent->GetNumActionBindings(); i++) // E ��ų ���ε� �ʱ�ȭ
	{
		FInputActionBinding Binding = InputComponent->GetActionBinding(i);
		if (Binding.GetActionName() == Skill_E_Binding.GetActionName())
		{
			InputComponent->RemoveActionBinding(i);
		}
	}

	if (num == 1)// 1���� �޽�
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
	else if (num == 2) //2���� ��������
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
	else if (num == 3) //3���� ������
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





