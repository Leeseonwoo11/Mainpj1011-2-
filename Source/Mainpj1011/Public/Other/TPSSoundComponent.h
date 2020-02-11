// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "Components/ActorComponent.h"
#include "Sound/SoundCue.h"
#include "TPSSoundComponent.generated.h"

// 게임플레이 할 때 필요한 사운드를 모두 담아놓은 사운드 컴포넌트이다. 총소리 재장전소리 무기교체서리 발걸음소리 등등

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MAINPJ1011_API UTPSSoundComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTPSSoundComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// 사운드
	UPROPERTY(VisibleAnywhere)
	UAudioComponent* HitSound;
	UPROPERTY(VisibleAnywhere)
	UAudioComponent* WallHitSound;
	UPROPERTY(VisibleAnywhere)
	UAudioComponent* StepSound;
	UPROPERTY(VisibleAnywhere)
	UAudioComponent* WeaponChangeSound;
	UPROPERTY(VisibleAnywhere)
	UAudioComponent* ARShotSound;
	UPROPERTY(VisibleAnywhere)
	UAudioComponent* ARReloadSound1;
	UPROPERTY(VisibleAnywhere)
	UAudioComponent* ARReloadSound2;
	UPROPERTY(VisibleAnywhere)
	UAudioComponent* SRShotSound;
	UPROPERTY(VisibleAnywhere)
	UAudioComponent* SRReloadSound1;
	UPROPERTY(VisibleAnywhere)
	UAudioComponent* SRReloadSound2;
	UPROPERTY(VisibleAnywhere)
	UAudioComponent* PTShotSound;
	UPROPERTY(VisibleAnywhere)
	UAudioComponent* PTReloadSound1;
	UPROPERTY(VisibleAnywhere)
	UAudioComponent* PTReloadSound2;
	UPROPERTY(VisibleAnywhere)
	UAudioComponent* TrakingMineSound;
	UPROPERTY(VisibleAnywhere)
	UAudioComponent* PulseDetectorSound;
	UPROPERTY(VisibleAnywhere)
	UAudioComponent* SupportSound;
		
};
