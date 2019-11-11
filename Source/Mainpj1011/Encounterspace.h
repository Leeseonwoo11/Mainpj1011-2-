// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "GameFramework/Actor.h"
#include "TPSEnemy.h"
#include "Encounterspace.generated.h"

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
