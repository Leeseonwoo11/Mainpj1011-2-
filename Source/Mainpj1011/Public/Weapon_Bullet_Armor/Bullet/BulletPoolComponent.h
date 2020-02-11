// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "Bullet.h"
#include "Components/ActorComponent.h"
#include "BulletPoolComponent.generated.h"

// 총알오브젝트 풀

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
	int32 PoolSize = 20; // 총알 풀 사이즈 

	UPROPERTY(VisibleAnywhere)
	TArray<ABullet*> Bulletpool;
	UFUNCTION()
	void GenerateBulletPool();// 총알 풀 생성함수
	UFUNCTION()
	ABullet* GetBulletPtr(); // 풀에서 한개의 총알 포인터 리턴
	UFUNCTION(BlueprintCallable)
	void DestroyBulletPool(); //  풀 삭제
};
