// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "Bullet.h"
#include "Components/ActorComponent.h"
#include "BulletPoolComponent.generated.h"


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
	int32 PoolSize = 20;

	UPROPERTY(VisibleAnywhere)
	TArray<ABullet*> Bulletpool;
	UFUNCTION()
	void GenerateBulletPool();
	UFUNCTION()
	ABullet* GetBulletPtr();
	UFUNCTION(BlueprintCallable)
	void DestroyBulletPool();
};
