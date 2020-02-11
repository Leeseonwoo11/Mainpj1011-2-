// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SkillObject.h"
#include "Skill_Support.generated.h"

/**
 * 
 */
// 서포터가 스폰이 되면 콜리젼에 감지된 플레이어는 콜리젼 내에서 타이머가 작동하는동안 HP를 회복시켜준다.
// 스킬파워가 높다면 힐량이 증가한다.
UCLASS()
class MAINPJ1011_API ASkill_Support : public ASkillObject
{
	GENERATED_BODY()
public:
	ASkill_Support();
protected:
	virtual void BeginPlay() override;
public:
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(VisibleAnywhere)
	UParticleSystemComponent* Aura;
	UPROPERTY(EditAnywhere, BlueprintReadwrite)
	FVector SkillRangeVector;
	float Range = 0.0f;
	
	FTimerHandle HealTimer;

	virtual void OnActorBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	UFUNCTION()
	void HealCharacter();

	UPROPERTY()
	class UTPSSoundComponent* SoundComp;

};
