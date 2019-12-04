// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "GameFramework/Actor.h"
#include "ArmorStruct.h"
#include "SpawningArmor.generated.h"

UCLASS()
class MAINPJ1011_API ASpawningArmor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawningArmor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FArmorPropertyStruct ArmorProperty;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UBoxComponent* InteractionBox;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UParticleSystemComponent* Particle;

	UObject* CommonEffect;
	UObject* UnCommonEffect;
	UObject* RareEffect;
	UObject* EpicEffect;
	UObject* LegendaryEffect;
	UObject* CurrentEffect;

	int32 Ranknum;
	int32 Brandnum;
	int32 Typenum;

	FString BrandName;
	FString TypeName;
	FString FullName;

	void ConstructParticle();
	void RankParticleSet();
	void BrandSet();
	void TypeSet();
};
