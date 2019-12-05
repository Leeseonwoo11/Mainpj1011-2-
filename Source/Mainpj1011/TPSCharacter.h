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

	//ī�޶�ɼ�
	void SetCameraOption();
	
	void Up(float NewAxisValue);
	void Down(float NewAxisValue);
	void LeftRight(float NewAxisValue);

	FVector DirectionToMove = FVector::ZeroVector;

	//����,���ؾִϸ��̼�
	UPROPERTY(BlueprintReadOnly)
	bool bAimState = false;
	void SetTrueAimState();
	void SetFalseAimState();
	void ChangeAimLocation();
	bool bAimRight = true; //ĳ�����ǿ��������� ���ؼ� ����

	//�߻�,�߻�ִϸ��̼�,�߻���ġ,�߻�ӵ� Ÿ�̸�
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
	FTimerHandle FireCheckTimer; // ���簡 �Ұ����� �ѱⰡ Ŭ���ϴ� �ӵ����� �߻�Ǵ°��� �����ϱ� ���� Ÿ�̸�.;
	UFUNCTION()
	void FireCheck(); // ���̾� üũ Ÿ�̸ӿ� ���ؼ� ���ư��� ������ ��������Լ�;
	bool FireCheckFlag = true;

	//���� ���ϸ��̼�
	UPROPERTY(BlueprintReadOnly)
	bool bSprintState = false;
	void SetTrueSprintState();
	void SetFalseSprintState();
	void DownKeyPress(); //�Ʒ�Ű ���ȴ�(�Ʒ�Ű�� ������ ���ָ� ���ϰ� �ϱ����ؼ�)
	void DownKeyRelease(); // �Ʒ�Ű �ȴ��ȴ�.
	bool IsDown = false; 

	//���ⱳü���ϸ��̼�
	UPROPERTY(BlueprintReadOnly)
	bool bChangeWeaponState = false;
	UFUNCTION(BlueprintCallable)
	void SetFalseChangeWeaponState();
	void SetTrueChangeWeaponState();

	//���ⱳü
	UPROPERTY(BlueprintReadOnly)
	bool bWeapon1 = false;
	void SetWeapon1();
	UPROPERTY(BlueprintReadOnly)
	bool bWeapon2 = false;
	void SetWeapon2();
	UPROPERTY(BlueprintReadOnly)
	bool bWeapon3 = false;
	void SetWeapon3();

	//���� ����
	UFUNCTION()
	void preReload();
	UPROPERTY(BlueprintReadWrite)
	bool IsReload;
	UFUNCTION()
	void Reload(EWeaponType CurWeaponType);
	bool bAutoFireRelaodFlag = true;

	//�������� �ҷ����� ����
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	int32 CurAMMO; //����ź�� (�������� ���)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 TotalAMMO;// ��ź�� (�������� ���)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float PlayerHPRatio; // HP����(�������� ���)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float CurWeaponDamage = 0.0f;


	//������
	UPROPERTY(BlueprintReadOnly)
	bool bCovered = false;
	void SetCover();
	UFUNCTION()
	void SetFalseCoverState();
	UFUNCTION()
	void SetTrueCoverState();

	//ĳ���ӵ�����
	void CharacterSpeedOption();

	//���ӿ�����
	UFUNCTION(BlueprintCallable)
	FRotator GetAimOffsets() const;

	//���⽽��
	AWeapon* WeaponSlot1; //1������
	AWeapon* WeaponSlot2; //2������
	AWeapon* WeaponSlot3; //3������
	UPROPERTY(VisibleAnywhere)
	AWeapon* CurWeapon = nullptr;  // ���繫��
	
	//��������
	UPROPERTY(BlueprintReadOnly)
	bool bAutomaticFire;

	//���� ���� �̳�
	enum class WeaponNum
	{
		Weapon1,
		Weapon2,
		Pistol,
	};
	//���繫���̳Ѱ�
	WeaponNum CurrentWeaponSlot = WeaponNum::Weapon1;


	// �浹üũ
	UFUNCTION()
	void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	

	//���� ĳ���͸� �����ֵ��� ���� (AIPerception)
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	UAIPerceptionStimuliSourceComponent* PerceptionSource;
	virtual bool CanBeSeenFrom(const FVector & ObserverLocation,
			FVector & OutSeenLocation,
			int32 & NumberOfLoSChecksPerformed,
			float & OutSightStrength,
			const AActor * IgnoreActor = nullptr)const;
	//��������Ʈ
	
	//�κ��丮������Ʈ
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UInventoryComponent* Inven;


};



