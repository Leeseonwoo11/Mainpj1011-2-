// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Blueprint/UserWidget.h"
#include "TPSGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class MAINPJ1011_API ATPSGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	ATPSGameModeBase();

public:
	virtual void BeginPlay() override;
public:
};
