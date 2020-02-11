// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSEnemyStatComponent.h"

// Sets default values for this component's properties
UTPSEnemyStatComponent::UTPSEnemyStatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	bWantsInitializeComponent = true;
	// ...
}


// Called when the game starts
void UTPSEnemyStatComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UTPSEnemyStatComponent::InitializeComponent()
{
	Super::InitializeComponent();
	SetDamage(0.0f);
}

void UTPSEnemyStatComponent::SetDamage(float NewDamage)// 데미지를 입는 함수.
{
	Health -= NewDamage;
	HPChangeDel.Broadcast();
}
float UTPSEnemyStatComponent::GetHPRatio()
{
	return Health / MaxHealth;
}