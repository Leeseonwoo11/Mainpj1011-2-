// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSCharacter.h"
#include "Weapon_Pistol.h"
#include "Weapon_AR.h"
#include "Weapon_SR.h"
#include "DrawDebugHelpers.h"
#include "Perception/AISense_Sight.h"


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
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
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
}

// Called when the game starts or when spawned
void ATPSCharacter::BeginPlay()
{
	Super::BeginPlay();
	Tags.Add(TEXT("PLAYER"));
	GetMesh()->SetGenerateOverlapEvents(true);

	auto Pistol = GetWorld()->SpawnActor<AWeapon_Pistol>(FVector::ZeroVector, FRotator::ZeroRotator);
	WeaponSlot3 = Pistol;
	if (Pistol != nullptr)
	{
		Pistol->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("Pistol_Socket"));
	}

	auto AssaultRifle = GetWorld()->SpawnActor<AWeapon_AR>(FVector::ZeroVector, FRotator::ZeroRotator);
	WeaponSlot1 = AssaultRifle;
	if (AssaultRifle != nullptr)
	{
		AssaultRifle->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("Weapon1_Socket"));
	}
	auto SniperRifle = GetWorld()->SpawnActor<AWeapon_SR>(FVector::ZeroVector, FRotator::ZeroRotator);
	WeaponSlot2 = SniperRifle;
	if (SniperRifle != nullptr)
	{
		SniperRifle->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("Weapon2_Socket"));
	}
	SetWeapon3();
	SetFalseCoverState();
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

}
void ATPSCharacter::SetCameraOption()
{
	SpringArm->SetupAttachment(GetCapsuleComponent());

	SpringArm->TargetArmLength = 200.0f;
	SpringArm->SetRelativeRotation(FRotator(-5.0f, 0.0f, 0.0f));
	SpringArm->SetRelativeLocation(FVector(0.0f, 45.0f, 85.0f));
	SpringArm->bUsePawnControlRotation = true;

	Camera->SetupAttachment(SpringArm);
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
		bAimRight = true;
		SpringArm->TargetArmLength = 100.0f;
		SpringArm->SetRelativeRotation(FRotator(-5.0f, 0.0f, 0.0f));
		SpringArm->SetRelativeLocation(FVector(0.0f, 45.0f, 85.0f));
	}
}
//�߻���� ����
void ATPSCharacter::SetTrueFireState()
{
	if (!bSprintState)
	{
		bFireState = true;
		preFire();
	}
	else if(bAimState)
	{
		bFireState = true;
		preFire();
	}
}
void ATPSCharacter::preFire() //�߻����� �غ�
{
	switch (CurrentWeaponSlot)
	{
	case WeaponNum::Pistol:
		CurWeapon = WeaponSlot3;
		Fire();
		break;
	case WeaponNum::Weapon1:
		CurWeapon = WeaponSlot1;
		if (WeaponSlot1->WeaponType == EWeaponType::SR)
		{
			Fire();
		}
		else if (WeaponSlot1->WeaponType == EWeaponType::AR)
		{
			float Fireduration = 60.0f/ (float)WeaponSlot1->RPM;
			GetWorldTimerManager().SetTimer(FireSpeedTimer,this,&ATPSCharacter::Fire,Fireduration, true);
		}
		break;
	case WeaponNum::Weapon2:
		CurWeapon = WeaponSlot2;
		if (WeaponSlot2->WeaponType == EWeaponType::SR)
		{
			Fire();
		}
		else if (WeaponSlot2->WeaponType == EWeaponType::AR)
		{
			float Fireduration = 60.0f / (float)WeaponSlot2->RPM;
			GetWorldTimerManager().SetTimer(FireSpeedTimer, this, &ATPSCharacter::Fire, Fireduration, true);
		}
		break;
	}
}

void ATPSCharacter::Fire()
{
	//---------------------------�߻� �����̼��� ã�� ����Ʈ���̽�
	FHitResult OutResult;
	FVector Start = Camera->GetComponentLocation();
	FVector CamForwardVector = Camera->GetForwardVector();
	bool Result = GetWorld()->LineTraceSingleByChannel(OutResult, Start, CamForwardVector*15000.0f + Start, ECC_GameTraceChannel1);
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

			TempBullet->Damage = CurWeapon->Damage;
			TempBullet->SetActorLocation(CurWeapon->FirePos->GetComponentLocation());
			TempBullet->SetActorRotation(BulletRot);
			TempBullet->ProjectileMovement->Velocity = FireVector * 12000;
			TempBullet->SetActive(true);
			TempBullet->BulletTrail->Activate(true);

			//DrawDebugLine(GetWorld(), CurWeapon->FirePos->GetComponentLocation(), TargetLoc, FColor::Green, false, 1.0f);
			UE_LOG(LogTemp, Error, TEXT("Character fire count is  = %d"), count++);

		}
	}
}

void ATPSCharacter::SetFalseFireState()
{
	GetWorldTimerManager().ClearTimer(FireSpeedTimer);
	bFireState = false;
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

	if (CurrentWeaponSlot != WeaponNum::Weapon1)
	{
		CurrentWeaponSlot = WeaponNum::Weapon1;
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

void ATPSCharacter::SetWeapon2()
{
	GetWorldTimerManager().ClearTimer(FireSpeedTimer);

	if (CurrentWeaponSlot != WeaponNum::Weapon2)
	{
		CurrentWeaponSlot = WeaponNum::Weapon2;
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
		if(WeaponSlot1 != nullptr)
		WeaponSlot1->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("Weapon1_Socket"));

	}
}

void ATPSCharacter::SetWeapon3()
{
	GetWorldTimerManager().ClearTimer(FireSpeedTimer);

	if (CurrentWeaponSlot != WeaponNum::Pistol)
	{
		CurrentWeaponSlot = WeaponNum::Pistol;
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
		//	UE_LOG(LogTemp, Warning, TEXT("TRUE"));
			return true;
		}
	}
	const bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, ObserverLocation, GetActorLocation(), ECC_GameTraceChannel5);
	NumberOfLoSChecksPerformed++;
	if (bHit == true)
	{
		if (HitResult.Actor.IsValid() && HitResult.Actor->IsOwnedBy(this))
		{
			OutSeenLocation = GetActorLocation();
			OutSightStrength = 1;
			//UE_LOG(LogTemp, Warning, TEXT("TRUE"));
			return true;
		}
	}
	//UE_LOG(LogTemp, Warning, TEXT("FALSE"));
	OutSightStrength = 0;
	return false;
}

