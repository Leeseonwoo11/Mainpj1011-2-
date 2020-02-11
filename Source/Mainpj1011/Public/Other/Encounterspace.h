// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "GameFramework/Actor.h"
#include "TPSEnemy.h"
#include "Encounterspace.generated.h"

// 적 하나가 이 영역에서 플레이어를 감지하면 다른 적들도 플레이어를 감지할 수 있도록 설정해주는 영역 액터

DECLARE_DELEGATE_OneParam(FEncounterDelegate,FVector)

UCLASS()
class MAINPJ1011_API AEncounterspace : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEncounterspace();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY()
	UBoxComponent* EncountSpace;

	UPROPERTY()
	FVector TargetLocation;

	UFUNCTION()
	void CallOutTarget(FVector TargetLoc);

	FEncounterDelegate EncounterDelegate;


};
