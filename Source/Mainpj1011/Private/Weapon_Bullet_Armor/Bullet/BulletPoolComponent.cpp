// Fill out your copyright notice in the Description page of Project Settings.


#include "BulletPoolComponent.h"

// Sets default values for this component's properties
UBulletPoolComponent::UBulletPoolComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	// ...
}


// Called when the game starts
void UBulletPoolComponent::BeginPlay()
{
	Super::BeginPlay();
	GenerateBulletPool();
}


// Called every frame
void UBulletPoolComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UBulletPoolComponent::GenerateBulletPool() 
{
	UWorld* const World = GetWorld();
	if (World)
	{
		for (int32 i = 0; i < PoolSize; i++)
		{
			ABullet* TempBullet = GetWorld()->SpawnActor<ABullet>(FVector::ZeroVector, FRotator::ZeroRotator); // 총알 생성
			if (TempBullet != nullptr)
			{
					TempBullet->SetActive(false); // 총알 비활성화
					Bulletpool.Push(TempBullet); // 풀 배열에 추가
			}
			
		}
	}
}

ABullet * UBulletPoolComponent::GetBulletPtr()
{
	for (ABullet* PoolingBullet : Bulletpool) //풀 배열
	{
		if (!PoolingBullet->IsActive())// 비활성화된 총알이라면
		{
			return PoolingBullet; // 리턴해준다.
		}
	}
	return nullptr;// 비활성화된 총알이 풀에 없다면 널값을 리턴해준다.
}

void UBulletPoolComponent::DestroyBulletPool()// 풀삭제
{
	for (ABullet* PoolingBullet : Bulletpool)
	{
		PoolingBullet->Destroy();// 생성해 놓은 총알 삭제
		PoolingBullet = nullptr; // 널로 바꿔줌
	}
}
