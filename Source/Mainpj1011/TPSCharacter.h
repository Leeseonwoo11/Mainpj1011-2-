// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "Weapon.h"
#include "BulletPoolComponent.h"
#include "GameFramework/Character.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISightTargetInterface.h"
#include "TPSCharacterStatComponent.h"
#include "InventoryComponent.h"
#include "TPSCharacter.generated.h"


class UAIPerceptionStimuliSourceComponent;

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
	UBulletPoolComponent* BulletPool;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	UTPSCharacterStatComponent* PlayerStatComp;

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
	AWeapon* WeaponSlot1; //1번무기
	AWeapon* WeaponSlot2; //2번무기
	AWeapon* WeaponSlot3; //3번무기
	UPROPERTY(VisibleAnywhere)
	AWeapon* CurWeapon = nullptr;  // 현재무기
	
	//연사유무
	UPROPERTY(BlueprintReadOnly)
	bool bAutomaticFire;

	//무기 슬롯 이넘
	enum class WeaponNum
	{
		Weapon1,
		Weapon2,
		Pistol,
	};
	//현재무기이넘값
	WeaponNum CurrentWeaponSlot = WeaponNum::Weapon1;


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
	UInventoryComponent* Inven;


};



