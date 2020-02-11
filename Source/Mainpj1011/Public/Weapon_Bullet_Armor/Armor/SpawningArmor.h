// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "GameFramework/Actor.h"
#include "ArmorStruct.h"
#include "Components/WidgetComponent.h"
#include "SpawningArmor.generated.h"

//스폰되는 방어구의 액터이다. 

UCLASS()
class MAINPJ1011_API ASpawningArmor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawningArmor();

	virtual void PostInitializeComponents() override;

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
	int32 Brandnum;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	int32 Typenum;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FString BrandName;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FString TypeName;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	FString FullName;

	void ConstructParticle();
	void RankParticleSet();
	void BrandSet();
	void TypeSet();

	class ATPSCharacter* TempCharacter;
	UClass* ArmorInfoWidget;
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
