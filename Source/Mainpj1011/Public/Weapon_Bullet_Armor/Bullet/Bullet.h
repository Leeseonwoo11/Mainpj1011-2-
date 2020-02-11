// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Bullet.generated.h"

// 총알 액터로 불릿풀에 미리 생성하고 따로 생성될 일은 없다.

UCLASS()
class MAINPJ1011_API ABullet : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABullet();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintReadWrite)
	UProjectileMovementComponent* ProjectileMovement; // 발사체 무브먼트
	UPROPERTY(VisibleAnywhere)
	USphereComponent* BulletBody;// 총알의 콜리젼
	UPROPERTY(VisibleAnywhere)
	UParticleSystemComponent* BulletTrail;// 총알의 파티클
	UPROPERTY(VisibleAnywhere)
	UParticleSystemComponent* BulletSpark;// 총알 피격시 스파크 파티클

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Damage; //  총알 데미지
	UPROPERTY(EditAnywhere)
	bool Active = false; //  활성화/비활성화 여부
	UFUNCTION()
	bool IsActive(); // 활성화여부 리턴
	UFUNCTION(BlueprintCallable)
	void SetActive(bool bval); // 활성화 설정
	UFUNCTION(BlueprintCallable)
	void SetVelocity(FVector val); //  속도설정

	UFUNCTION(BlueprintCallable)
	void OnActorBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	FTimerHandle DestoryTime; // 비활성화 타이머
	void SetDeActive();// 비활성화 설정

	class UTPSSoundComponent* BulletImpactSoundComp;
};
