// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Bullet.generated.h"

// �Ѿ� ���ͷ� �Ҹ�Ǯ�� �̸� �����ϰ� ���� ������ ���� ����.

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
	UProjectileMovementComponent* ProjectileMovement; // �߻�ü �����Ʈ
	UPROPERTY(VisibleAnywhere)
	USphereComponent* BulletBody;// �Ѿ��� �ݸ���
	UPROPERTY(VisibleAnywhere)
	UParticleSystemComponent* BulletTrail;// �Ѿ��� ��ƼŬ
	UPROPERTY(VisibleAnywhere)
	UParticleSystemComponent* BulletSpark;// �Ѿ� �ǰݽ� ����ũ ��ƼŬ

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Damage; //  �Ѿ� ������
	UPROPERTY(EditAnywhere)
	bool Active = false; //  Ȱ��ȭ/��Ȱ��ȭ ����
	UFUNCTION()
	bool IsActive(); // Ȱ��ȭ���� ����
	UFUNCTION(BlueprintCallable)
	void SetActive(bool bval); // Ȱ��ȭ ����
	UFUNCTION(BlueprintCallable)
	void SetVelocity(FVector val); //  �ӵ�����

	UFUNCTION(BlueprintCallable)
	void OnActorBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	FTimerHandle DestoryTime; // ��Ȱ��ȭ Ÿ�̸�
	void SetDeActive();// ��Ȱ��ȭ ����

	class UTPSSoundComponent* BulletImpactSoundComp;
};
