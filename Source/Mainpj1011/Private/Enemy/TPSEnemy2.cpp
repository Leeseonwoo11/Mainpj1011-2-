// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSEnemy2.h"
#include "TPSAIController2.h"

ATPSEnemy2::ATPSEnemy2()
{
	AIControllerClass = ATPSAIController2::StaticClass();
	static ConstructorHelpers::FObjectFinder<UMaterial>M_BODY(TEXT("/Game/MyNew/Material/M_Enemy2Body"));
	if (M_BODY.Succeeded())
	{
		GetMesh()->SetMaterial(0, M_BODY.Object);
	}
}