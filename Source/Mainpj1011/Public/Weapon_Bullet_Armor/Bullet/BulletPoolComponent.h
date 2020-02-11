// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "Bullet.h"
#include "Components/ActorComponent.h"
#include "BulletPoolComponent.generated.h"

// �Ѿ˿�����Ʈ Ǯ

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MAINPJ1011_API UBulletPoolComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UBulletPoolComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	int32 PoolSize = 20; // �Ѿ� Ǯ ������ 

	UPROPERTY(VisibleAnywhere)
	TArray<ABullet*> Bulletpool;
	UFUNCTION()
	void GenerateBulletPool();// �Ѿ� Ǯ �����Լ�
	UFUNCTION()
	ABullet* GetBulletPtr(); // Ǯ���� �Ѱ��� �Ѿ� ������ ����
	UFUNCTION(BlueprintCallable)
	void DestroyBulletPool(); //  Ǯ ����
};
