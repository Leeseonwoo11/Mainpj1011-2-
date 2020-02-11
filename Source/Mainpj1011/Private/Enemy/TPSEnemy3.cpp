// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSEnemy3.h"
#include "TPSAIController3.h"

ATPSEnemy3::ATPSEnemy3()
{
	AIControllerClass = ATPSAIController3::StaticClass();//AIControllerπŸ≤„¡‹
	static ConstructorHelpers::FObjectFinder<UMaterial>M_BODY(TEXT("/Game/MyNew/Material/M_AssertEnemyBody"));// ∏”≈Õ∏ÆæÛ πŸ≤„¡‹
	if (M_BODY.Succeeded())
	{
		GetMesh()->SetMaterial(0, M_BODY.Object);
	}
}