// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "Components/ActorComponent.h"
#include "ArmorStruct.h"
#include "InventoryComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MAINPJ1011_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UInventoryComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(EditAnywhere,BlueprintReadWrite)
	TArray<FArmorPropertyStruct> Inventory;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 InventorySize = 100;

	UFUNCTION()
	void AddInventroyItem(FArmorPropertyStruct Item);

	UFUNCTION()
	void RemoveInventoryItem(FArmorPropertyStruct Item);
};
