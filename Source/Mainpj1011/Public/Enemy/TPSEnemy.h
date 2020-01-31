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
	AWeapon* CurWeapon;  // 현재무기

	//발사관련

	UFUNCTION(BlueprintCallable)
	void preFire();
	UFUNCTION(BlueprintCallable)
	void Fire();
	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	FVector TargetLoc;
	UPROPERTY(BlueprintReadWrite)
	FTimerHandle FireSpeedTimer1;

	//상태관련 변수
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
	
	//발사하기anim (애니메이션블루프린트에서 이 값들을 읽어서 모션을 바꿈)
	UFUNCTION(BlueprintCallable)
	void SetFalseFireState();
	UFUNCTION(BlueprintCallable)
	void SetTrueFireState();
	//웅크리기anim
	UFUNCTION(BlueprintCallable)
	void SetTrueCoverState();
	UFUNCTION(BlueprintCallable)
	void SetFalseCoverState();
	//조준하기anim
	UFUNCTION(BlueprintCallable)
	void SetTrueAimState();
	UFUNCTION(BlueprintCallable)
	void SetFalseAimState();
	//총알 발사 카운트
	int32 count = 0;
	//플레이어 포인터
	ATPSCharacter* TempCharacter;
	//행동변화 불값 변경 함수 (애니메이션에서 사용)
	UFUNCTION(BlueprintCallable)
	void CStartShooting();
	UFUNCTION(BlueprintCallable)
	void CStopShoothing();
	UFUNCTION(BlueprintCallable)
	void CEnterCover();
	UFUNCTION(BlueprintCallable)
	void COutCover();
	//겹침 체크 함수 총알,스킬 피격시 사용
	UFUNCTION()
	void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


	UPROPERTY(VisibleAnywhere)
	UWidgetComponent* EnemyHPWidget;
	UPROPERTY(VisibleAnywhere)
	UClass* Widget;
	//무기와 방어구 드랍 함수
	UFUNCTION()
	void SpawnWeapon();
	UFUNCTION()
	void SpawnArmor();

	bool bItemSpawn = true;
	//사운드
	class UTPSSoundComponent* SoundComp;
	UFUNCTION(BlueprintCallable)
	void PlayStepSound();
};
