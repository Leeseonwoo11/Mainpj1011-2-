// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TPSEnemy.h"
#include "TPSEnemy3.generated.h"

/**
 * 
 */
 // TPSEnemey를 상속받으며 머터리얼과 AIController만 바꾼다.

UCLASS()
class MAINPJ1011_API ATPSEnemy3 : public ATPSEnemy
{
	GENERATED_BODY()
public:
	ATPSEnemy3();
};
