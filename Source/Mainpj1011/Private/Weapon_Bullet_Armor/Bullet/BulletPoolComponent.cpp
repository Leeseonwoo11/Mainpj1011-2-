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
			ABullet* TempBullet = GetWorld()->SpawnActor<ABullet>(FVector::ZeroVector, FRotator::ZeroRotator); // �Ѿ� ����
			if (TempBullet != nullptr)
			{
					TempBullet->SetActive(false); // �Ѿ� ��Ȱ��ȭ
					Bulletpool.Push(TempBullet); // Ǯ �迭�� �߰�
			}
			
		}
	}
}

ABullet * UBulletPoolComponent::GetBulletPtr()
{
	for (ABullet* PoolingBullet : Bulletpool) //Ǯ �迭
	{
		if (!PoolingBullet->IsActive())// ��Ȱ��ȭ�� �Ѿ��̶��
		{
			return PoolingBullet; // �������ش�.
		}
	}
	return nullptr;// ��Ȱ��ȭ�� �Ѿ��� Ǯ�� ���ٸ� �ΰ��� �������ش�.
}

void UBulletPoolComponent::DestroyBulletPool()// Ǯ����
{
	for (ABullet* PoolingBullet : Bulletpool)
	{
		PoolingBullet->Destroy();// ������ ���� �Ѿ� ����
		PoolingBullet = nullptr; // �η� �ٲ���
	}
}
