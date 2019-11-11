// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSGameModeBase.h"
#include "TPSCharacter.h"

ATPSGameModeBase::ATPSGameModeBase()
{
	DefaultPawnClass = ATPSCharacter::StaticClass();
}

void ATPSGameModeBase::BeginPlay()
{
	Super::BeginPlay();
}