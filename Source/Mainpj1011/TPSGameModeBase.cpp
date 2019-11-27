// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSGameModeBase.h"
#include "TPSCharacter.h"
#include "CrosshairHUD.h"
#include "UserWidget.h"

ATPSGameModeBase::ATPSGameModeBase()
{
	DefaultPawnClass = ATPSCharacter::StaticClass();
	HUDClass = ACrosshairHUD::StaticClass();

}

void ATPSGameModeBase::BeginPlay()
{
	Super::BeginPlay();

}