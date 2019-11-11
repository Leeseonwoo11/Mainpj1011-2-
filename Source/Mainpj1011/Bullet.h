// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Bullet.generated.h"

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
	UProjectileMovementComponent* ProjectileMovement;
	UPROPERTY(VisibleAnywhere)
	USphereComponent* BulletBody;
	UPROPERTY(VisibleAnywhere)
	UParticleSystemComponent* BulletTrail;
	UPROPERTY(VisibleAnywhere)
	UParticleSystemComponent* BulletSpark;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Damage;
	UPROPERTY(EditAnywhere)
	bool Active = false;
	UFUNCTION()
	bool IsActive();
	UFUNCTION(BlueprintCallable)
	void SetActive(bool bval);
	UFUNCTION(BlueprintCallable)
	void SetVelocity(FVector val);

	UFUNCTION(BlueprintCallable)
	void OnActorBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	FTimerHandle DestoryTime;
	void SetDeActive();

};
