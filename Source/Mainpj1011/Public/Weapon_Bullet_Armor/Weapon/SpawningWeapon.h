// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "GameFramework/Actor.h"
#include "WeaponStruct.h"
#include "Components/WidgetComponent.h"
#include "SpawningWeapon.generated.h"

UCLASS()
class MAINPJ1011_API ASpawningWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawningWeapon();

	virtual void PostInitializeComponents() override;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FWeaponPropertyStruct WeaponProperty; // 무기 구조체 이넘값들.

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UBoxComponent* InteractionBox;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UParticleSystemComponent* Particle;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UWidgetComponent* WidgetComponent;

	UObject* CommonEffect;
	UObject* UnCommonEffect;
	UObject* RareEffect;
	UObject* EpicEffect;
	UObject* LegendaryEffect;
	UObject* CurrentEffect;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 Ranknum;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 Typenum;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FString FullName;

	void ConstructParticle();
	void RankParticleSet();
	void TypeSet();

	class ATPSCharacter* TempCharacter;
	UClass* WeaponInfoWidget;

	UFUNCTION()
	void OnComponentBeginOverlap(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	//아이템 먹을수 있는지 체크 bool
	UPROPERTY()
	bool IsEatableItem = false;
	UPROPERTY()
	bool IsEattenItem = false;
};
