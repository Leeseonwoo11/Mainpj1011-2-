// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "GameFramework/Character.h"
#include "Weapon.h"
#include "BulletPoolComponent.h"
#include "TPSCharacter.h"
#include "TPSEnemyStatComponent.h"
#include "Components/WidgetComponent.h"
#include "TPSEnemy.generated.h"


UCLASS()
class MAINPJ1011_API ATPSEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATPSEnemy();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	virtual void PostInitializeComponents() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	UBulletPoolComponent* BulletPool;

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	UTPSEnemyStatComponent* StatComp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	AWeapon* CurWeapon;  // ���繫��

	//�߻����

	UFUNCTION(BlueprintCallable)
	void preFire();
	UFUNCTION(BlueprintCallable)
	void Fire();
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	FVector TargetLoc;
	UPROPERTY(BlueprintReadWrite)
	FTimerHandle FireSpeedTimer1;

	//���°��� ����
	UPROPERTY(BlueprintReadWrite)
	bool bAimState = false;
	UPROPERTY(BlueprintReadWrite)
	bool bSprintState = false;
	UPROPERTY(BlueprintReadWrite)
	bool bFireState = false;
	UPROPERTY(BlueprintReadWrite)
	bool bCrouchState = false;
	UPROPERTY(BlueprintReadWrite)
	bool bIsDead = false;
	
	//�߻��ϱ�anim (�ִϸ��̼Ǻ������Ʈ���� �� ������ �о ����� �ٲ�)
	UFUNCTION(BlueprintCallable)
	void SetFalseFireState();
	UFUNCTION(BlueprintCallable)
	void SetTrueFireState();
	//��ũ����anim
	UFUNCTION(BlueprintCallable)
	void SetTrueCoverState();
	UFUNCTION(BlueprintCallable)
	void SetFalseCoverState();
	//�����ϱ�anim
	UFUNCTION(BlueprintCallable)
	void SetTrueAimState();
	UFUNCTION(BlueprintCallable)
	void SetFalseAimState();
	//�Ѿ� �߻� ī��Ʈ
	int32 count = 0;
	//�÷��̾� ������
	ATPSCharacter* TempCharacter;
	//�ൿ��ȭ �Ұ� ���� �Լ� (�ִϸ��̼ǿ��� ���)
	UFUNCTION(BlueprintCallable)
	void CStartShooting();
	UFUNCTION(BlueprintCallable)
	void CStopShoothing();
	UFUNCTION(BlueprintCallable)
	void CEnterCover();
	UFUNCTION(BlueprintCallable)
	void COutCover();
	//��ħ üũ �Լ� �Ѿ�,��ų �ǰݽ� ���
	UFUNCTION()
	void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


	UPROPERTY(VisibleAnywhere)
	UWidgetComponent* EnemyHPWidget;
	UPROPERTY(VisibleAnywhere)
	UClass* Widget;
	//����� �� ��� �Լ�
	UFUNCTION()
	void SpawnWeapon();
	UFUNCTION()
	void SpawnArmor();

	bool bItemSpawn = true;
	//����
	class UTPSSoundComponent* SoundComp;
	UFUNCTION(BlueprintCallable)
	void PlayStepSound();
};
