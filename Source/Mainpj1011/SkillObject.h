// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "GameFramework/Character.h"
#include "SkillObject.generated.h"


UENUM()
enum class ESkillType
{
	NoSkillObj,
	PulseDetector,
	StickyBoom,
	Support,
	TrackingMines,
};

UCLASS()
class MAINPJ1011_API ASkillObject : public ACharacter
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASkillObject();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* Body;
	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	USphereComponent* SkillRange;
	UPROPERTY()
	float SkillPower;
	UPROPERTY()
	float Cooltime;
	UPROPERTY()
	ESkillType SkillType;
	UFUNCTION()
	virtual void OnActorBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
