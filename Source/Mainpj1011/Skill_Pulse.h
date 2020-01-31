// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "SkillObject.h"
#include "Skill_Pulse.generated.h"

/**
 * 
 */
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
