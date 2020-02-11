// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSAIController2.h"

ATPSAIController2::ATPSAIController2()
{
	static ConstructorHelpers::FObjectFinder<UBehaviorTree>BT_Object(TEXT("/Game/MyNew/AI/BT_EnemyCover2")); //BT만 새로 지정해준다.
	if (BT_Object.Succeeded())
	{
		BTAsset = BT_Object.Object;
	}
}