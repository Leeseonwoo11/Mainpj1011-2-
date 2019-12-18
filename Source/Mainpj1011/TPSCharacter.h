// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "GameFramework/Character.h"
#include "Perception/AISightTargetInterface.h"
#include "SpawningArmor.h"
#include "SpawningWeapon.h"
#include "TPSCharacter.generated.h"


class UAIPerceptionStimuliSourceComponent;

//무기 슬롯 이넘
UENUM()
enum class EWeaponNum
{
	Weapon1,
	Weapon2,
	Pistol,
	None,
};

UCLASS()
class MAINPJ1011_API ATPSCharacter : public ACharacter,public IAISightTargetInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATPSCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArm;

	UPROPERTY(EditAnywhere)
	UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere)
	class UBulletPoolComponent* BulletPool;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	class UTPSCharacterStatComponent* PlayerStatComp;

	//카메라옵션
	void SetCameraOption();
	void Up(float NewAxisValue);
	void Down(float NewAxisValue);
	void LeftRight(float NewAxisValue);
	FVector DirectionToMove = FVector::ZeroVector;

	//조준,조준애니메이션
	UPROPERTY(BlueprintReadOnly)
	bool bAimState = false;
	void SetTrueAimState();
	void SetFalseAimState();
	void ChangeAimLocation();
	bool bAimRight = true; //캐릭터의오른쪽으로 조준선 잡음

	//발사,발사애니메이션,발사위치,발사속도 타이머
	UPROPERTY(BlueprintReadOnly)
	bool bFireState = false;
	UFUNCTION(BlueprintCallable)
	void SetFalseFireState();
	void SetTrueFireState();
	UFUNCTION()
	void preFire();
	UFUNCTION()
	void Fire();
	UPROPERTY()
	FVector TargetLoc;
	UPROPERTY()
	FTimerHandle FireSpeedTimer;
	UPROPERTY()
	FTimerHandle FireCheckTimer; // 연사가 불가능한 총기가 클릭하는 속도따라 발사되는것을 방지하기 위한 타이머.;
	UFUNCTION()
	void FireCheck(); // 파이어 체크 타이머에 의해서 돌아가는 과도한 연사방지함수;
	bool FireCheckFlag = true;

	//질주 에니메이션
	UPROPERTY(BlueprintReadOnly)
	bool bSprintState = false;
	void SetTrueSprintState();
	void SetFalseSprintState();
	void DownKeyPress(); //아래키 눌렸다(아래키가 눌리면 질주를 못하게 하기위해서)
	void DownKeyRelease(); // 아래키 안눌렸다.
	bool IsDown = false; 

	//무기교체에니메이션
	UPROPERTY(BlueprintReadOnly)
	bool bChangeWeaponState = false;
	UFUNCTION(BlueprintCallable)
	void SetFalseChangeWeaponState();
	void SetTrueChangeWeaponState();

	//무기교체
	UPROPERTY(BlueprintReadOnly)
	bool bWeapon1 = false;
	void SetWeapon1();
	UPROPERTY(BlueprintReadOnly)
	bool bWeapon2 = false;
	void SetWeapon2();
	UPROPERTY(BlueprintReadOnly)
	bool bWeapon3 = false;
	void SetWeapon3();

	//무기 장전
	UFUNCTION()
	void preReload();
	UPROPERTY(BlueprintReadWrite)
	bool IsReload;
	UFUNCTION()
	void Reload(EWeaponType CurWeaponType);
	bool bAutoFireRelaodFlag = true;

	//위젯에서 불러쓰는 값들
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	int32 CurAMMO; //현재탄약 (위젯에서 사용)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 TotalAMMO;// 총탄약 (위젯에서 사용)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float PlayerHPRatio; // HP비율(위젯에서 사용)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float CurWeaponDamage = 0.0f;


	//은엄페
	UPROPERTY(BlueprintReadOnly)
	bool bCovered = false;
	void SetCover();
	UFUNCTION()
	void SetFalseCoverState();
	UFUNCTION()
	void SetTrueCoverState();

	//캐릭속도조절
	void CharacterSpeedOption();

	//에임오프셋
	UFUNCTION(BlueprintCallable)
	FRotator GetAimOffsets() const;

	//무기슬롯
	class AWeapon* WeaponSlot1; //1번무기
	class AWeapon* WeaponSlot2; //2번무기
	class AWeapon* WeaponSlot3; //3번무기
	UPROPERTY(VisibleAnywhere)
	class AWeapon* CurWeapon = nullptr;  // 현재무기
	
	//연사유무
	UPROPERTY(BlueprintReadOnly)
	bool bAutomaticFire;

	//현재무기이넘값
	EWeaponNum CurrentWeaponSlot = EWeaponNum::None;

	// 충돌체크
	UFUNCTION()
	void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	//적이 캐릭터를 볼수있도록 해줌 (AIPerception)
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	UAIPerceptionStimuliSourceComponent* PerceptionSource;
	virtual bool CanBeSeenFrom(const FVector & ObserverLocation,
			FVector & OutSeenLocation,
			int32 & NumberOfLoSChecksPerformed,
			float & OutSightStrength,
			const AActor * IgnoreActor = nullptr)const;
	//델리게이트
	
	//인벤토리컴포넌트
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UInventoryComponent* Inven;
	//인벤토리 아이템 추가
	UFUNCTION()
	void AddInventory();
	//인벤토리에 들어가기전에 임시로 들어가있는 아이템배열
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<ASpawningArmor*> TempArmorArray;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<ASpawningWeapon*> TempWeaponArray;

	//장비컴포넌트 장착한 장비를 가지고있음
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UEquipmentComponent* EquipmentComponent;

	//스킬 시전 
	FInputActionBinding Skill_Q_Binding = FInputActionBinding("Skill_Q",IE_Pressed);
	FInputActionBinding Skill_E_Binding = FInputActionBinding("Skill_E", IE_Pressed);
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Skill_Q;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Skill_E;
	UFUNCTION(BlueprintCallable)
	void SetSkill_Q(int32 num);
	UFUNCTION(BlueprintCallable)
	void SetSkill_E(int32 num);
	UFUNCTION()
	void SpawnTrackingMine();
	UFUNCTION()
	void SpawnSupport();
	UFUNCTION()
	void SpawnPulseDetector();
	UFUNCTION()
	void NoSkill();
	FTimerHandle PulseTimer;
	FTimerHandle MineTimer;
	FTimerHandle SupportTimer;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bPulseCooldown = true;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bMineCooldown = true;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool bSupportCooldown = true;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float PulseTimeValue = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MineTimeValue = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float SupportTimeValue = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float PulseCoolTime = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float MineCoolTime = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float SupportCoolTime = 1.0f;
	UFUNCTION()
	void PulseCooltimeFunc();	
	UFUNCTION()
	void MineCooltimeFunc();
	UFUNCTION()
	void SupportCooltimeFunc();
};



