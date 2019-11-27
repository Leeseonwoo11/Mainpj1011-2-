// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSCharacterStatComponent.h"

// Sets default values for this component's properties
UTPSCharacterStatComponent::UTPSCharacterStatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UTPSCharacterStatComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UTPSCharacterStatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UTPSCharacterStatComponent::SetDamage(float Damage)
{
	if (Damage > 0)
	{
		PlayerHealth -= Damage;
	}
}

float UTPSCharacterStatComponent::GetHPRatio()
{
	return PlayerHealth / 100.0f;
}

