// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "TPSEnemyStatComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnHPChangeDelegate);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MAINPJ1011_API UTPSEnemyStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UTPSEnemyStatComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;
	virtual void InitializeComponent() override;

public:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Health = 100.0f;
	UFUNCTION()
	void SetDamage(float NewDamage);
	UFUNCTION()
	float GetHPRatio(); //HP 비율 리턴 (HP바 위젯에서 피통설정 하기위해)

	//델리게이트
	FOnHPChangeDelegate HPChangeDel;
};
