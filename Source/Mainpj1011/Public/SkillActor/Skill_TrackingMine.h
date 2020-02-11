// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SkillObject.h"
#include "AIModule/Classes/Navigation/PathFollowingComponent.h"
#include "Skill_TrackingMine.generated.h"

/**
 * 
 */

//추적지뢰는 스폰이되고 주변에 적이 없다면 플레이어를 따라다니다가 적을 감지하면 적주변으로 이동하고 폭발한다.
//스킬파워가 커지면 데미지가 증가한다.
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
	UPROPERTY(VisibleAnywhere)
	class UTPSSoundComponent* SoundComp;

	bool bDetectflag = false;
	bool bPlayerfallow = true;
	bool IsExplosion = false;
	EPathFollowingRequestResult::Type EnemyMoveTo;
	FVector TargetLocation;
	float Damage = 20.0f;

	virtual void OnActorBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);


};
