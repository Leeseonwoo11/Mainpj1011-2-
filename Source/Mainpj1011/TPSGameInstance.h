// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "Engine/GameInstance.h"
#include "TPSGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class MAINPJ1011_API UTPSGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UPROPERTY()
	FTimerHandle AICoverShotTimer;
};
