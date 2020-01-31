// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_GetIntoCover.generated.h"

/**
 * 
 */
UCLASS()
class MAINPJ1011_API UBTTask_GetIntoCover : public UBTTaskNode
{
	GENERATED_BODY()
	UBTTask_GetIntoCover();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)override;
	bool IsInLoc;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds);

};
