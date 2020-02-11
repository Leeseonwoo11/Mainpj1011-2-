// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "GameFramework/HUD.h"
#include "CrosshairHUD.generated.h"

/**
 * 
 */

//조준선을 표시해주는 HUD

UCLASS()
class MAINPJ1011_API ACrosshairHUD : public AHUD
{
	GENERATED_BODY()

protected:
	UPROPERTY()
	UTexture2D* CrosshairTexture;
public:
	ACrosshairHUD();
	virtual void DrawHUD() override;
};
