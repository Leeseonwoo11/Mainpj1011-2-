// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SpawningArmor.h"
#include "TPSCharacterStatComponent.generated.h"

//플레이어의 스탯을 관리하는 컴포넌트 (HP, 체감온도, 무기파워,스킬파워)

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
	float ColdDamage = 0.5f;   //5초당 체감온도 5씩떨어짐
	float WarmProperty = 5.0f; //5초당 불에 있으면 체감온도 5도씩 올라감

	UFUNCTION(BlueprintCallable)
	void CallCold(); // 추위 시작 (온도감소 타이머 시작)
	UFUNCTION(BlueprintCallable)
	void CallWarm(); // 따뜻함 시작 (온도증가 타이머 시작)
	UFUNCTION(BlueprintCallable)
	void ClearCold(); // 추위 중지
	UFUNCTION(BlueprintCallable)
	void ClearWarm(); // 따뜻함 중지
	FTimerHandle ColdTimer;
	FTimerHandle WarmTimer;

	UFUNCTION()
	void SetDamage(float Damage); // 데미지 입는 함수.
	UFUNCTION()
	void SetColdDamage();// 온도감소함수(타이머로 실행)
	UFUNCTION()
	void SetWarm();// 온도증가함수(타이머로 실행)
	UFUNCTION(BlueprintCallable)
	float GetHPRatio();// HP비율 리턴함수

	class ATPSCharacter* TempTPSCharacter;

	void SetEquipmentAbility();// 장비능력치 적용함수

};