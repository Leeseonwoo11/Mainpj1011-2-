// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SpawningArmor.h"
#include "TPSCharacterStatComponent.generated.h"


//DECLARE_MULTICAST_DELEGATE(FCharHPUpdateDeleate);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MAINPJ1011_API UTPSCharacterStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTPSCharacterStatComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float PlayerMaxHealth = 100.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float PlayerHealth = PlayerMaxHealth;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float PlayerAttackPower = 0.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float PlayerSkillPower = 0.0f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float PlayerWindChillMin = -43.0f;   // 최저체감온도 (생존모드에서 사용 도달하면 죽음)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float PlayerWindChillMax = 20.0f;   // 최고체감온도 (생존모드에서 사용)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float PlayerWindChill = 0.0f;   // 현재체감온도 (생존모드에서 사용)
	float ColdDamage = 1.0f;
	float WarmProperty = 1.0f;
	UFUNCTION()
	void SetDamage(float Damage);
	UFUNCTION()
	void SetColdDamage();
	UFUNCTION()
	void SetWarm();
	UFUNCTION(BlueprintCallable)
	float GetHPRatio();

	class ATPSCharacter* TempTPSCharacter;

	void SetEquipmentAbility();


	//델리게이트
	//FCharHPUpdateDeleate CharHPUpdateDeleate;	
};