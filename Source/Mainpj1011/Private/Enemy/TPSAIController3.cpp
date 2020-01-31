// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSAIController3.h"

ATPSAIController3::ATPSAIController3()
{
	static ConstructorHelpers::FObjectFinder<UBehaviorTree>BT_Object(TEXT("/Game/MyNew/AI/BT_EnemyAssert"));
	if (BT_Object.Succeeded())
	{
		BTAsset = BT_Object.Object;
	}
}