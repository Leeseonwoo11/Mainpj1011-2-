// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_ShootFromCover.generated.h"

/**
 * 
 */
UCLASS()
class MAINPJ1011_API UBTTask_ShootFromCover : public UBTTaskNode
{
	GENERATED_BODY()
	UBTTask_ShootFromCover();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)override;
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);


	bool Fireflag = false;
	bool Timerflag = true;
	void SetTrueStopFire();
};
