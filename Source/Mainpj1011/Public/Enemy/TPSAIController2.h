// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TPSAIController.h"
#include "TPSAIController2.generated.h"

/**
 * 
 */

// TPSEnmey2를 움직이기 위한 AIController로 TPSAIcontroller를 상속받아 사용한다.
UCLASS()
class MAINPJ1011_API ATPSAIController2 : public ATPSAIController
{
	GENERATED_BODY()
public:
	ATPSAIController2();
	
};
