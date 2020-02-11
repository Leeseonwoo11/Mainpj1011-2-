// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "SkillObject.h"
#include "Skill_Pulse.generated.h"

/**
 * 
 */

// 펄스스킬액터가 스폰이 모든 적들은 일시적으로 랜더 커스텀뎁스를 켜주어 벽 뒤에서도 감지할 수 있도록 바꿔준다.
// 시간이 지나면 원래대로 돌아간다.
UCLASS()
class MAINPJ1011_API ASkill_Pulse : public ASkillObject
{
	GENERATED_BODY()
public:
	ASkill_Pulse();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	void StartSearching();
	void EndSearching();
	virtual void OnActorBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	
	FTimerHandle PulseTimer;
	FTimerHandle DebugSphereTimer;
	float Radius = 100;
	void DebugSphere();

	UPROPERTY(VisibleAnywhere)
	class UTPSSoundComponent* SoundComp;
};
