// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SkillObject.h"
#include "AIModule/Classes/Navigation/PathFollowingComponent.h"
#include "Skill_TrackingMine.generated.h"

/**
 * 
 */
UCLASS()
class MAINPJ1011_API ASkill_TrackingMine : public ASkillObject
{
	GENERATED_BODY()
protected:
	virtual void BeginPlay() override;

public:	
	ASkill_TrackingMine();

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere)
	USphereComponent* DetectRange;
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* Plane;
	UPROPERTY(VisibleAnywhere)
	UParticleSystem* ParticleFX1;

	bool bDetectflag = false;
	bool bPlayerfallow = true;
	bool IsExplosion = false;
	EPathFollowingRequestResult::Type EnemyMoveTo;
	FVector TargetLocation;
	float Damage = 20.0f;

	virtual void OnActorBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


};
