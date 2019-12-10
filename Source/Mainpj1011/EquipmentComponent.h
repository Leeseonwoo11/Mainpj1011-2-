// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SpawningArmor.h"
#include "EquipmentComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MAINPJ1011_API UEquipmentComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UEquipmentComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FArmorPropertyStruct EquipHead;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FArmorPropertyStruct EquipChest;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FArmorPropertyStruct EquipLegs;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FArmorPropertyStruct EquipShoulders;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FArmorPropertyStruct EquipHands;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FArmorPropertyStruct EquipFoots;

	UFUNCTION(BlueprintCallable)
	void WearingArmor(FArmorPropertyStruct NewArmor);

};