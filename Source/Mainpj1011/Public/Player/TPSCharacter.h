// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "GameFramework/Character.h"
#include "Perception/AISightTargetInterface.h"
#include "SpawningArmor.h"
#include "SpawningWeapon.h"
#include "TPSCharacter.generated.h"


class UAIPerceptionStimuliSourceComponent;

//���� ���� �̳�
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

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere)
	class UBulletPoolComponent* BulletPool;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	class UTPSCharacterStatComponent* PlayerStatComp;

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

	//��� ���ϸ��̼�
	UPROPERTY(BlueprintReadOnly)
	bool bDeadState = false;
	UFUNCTION()
	void SetTrueDeadState();

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
	class AWeapon* WeaponSlot1; //1������
	class AWeapon* WeaponSlot2; //2������
	class AWeapon* WeaponSlot3; //3������
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	class AWeapon* CurWeapon = nullptr;  // ���繫��
	
	//��������
	UPROPERTY(BlueprintReadOnly)
	bool bAutomaticFire;

	//���繫���̳Ѱ�
	EWeaponNum CurrentWeaponSlot = EWeaponNum::None;

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
	
	//�κ��丮������Ʈ
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UInventoryComponent* Inven;
	//�κ��丮 ������ �߰�
	UFUNCTION()
	void AddInventory();
	//�κ��丮�� �������� �ӽ÷� ���ִ� �����۹迭
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<ASpawningArmor*> TempArmorArray;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<ASpawningWeapon*> TempWeaponArray;

	//���������Ʈ ������ ��� ����������
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UEquipmentComponent* EquipmentComponent;

	//��ų ����, ��ų ����, ��ų ��Ÿ���Լ�
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
	
	//����
	UPROPERTY()
	class UTPSSoundComponent* SoundComp;

	UFUNCTION(BlueprintCallable)
	void StartReloadSound(); //���Գ�Ƽ���̷� �������ش�
	UFUNCTION(BlueprintCallable)
	void EndRelaodSound();//���Գ�Ƽ���̷� �������ش�
	UFUNCTION(BlueprintCallable)
	void ChangeWeaponSound();//���Գ�Ƽ���̷� �������ش�
	UFUNCTION(BlueprintCallable)
	void PlayStepSound();//���Գ�Ƽ���̷� �������ش�

	
};


