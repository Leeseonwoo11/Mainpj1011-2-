// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTaskNode_CallOutTarget.generated.h"

/**
 * 
 */
UCLASS()
class MAINPJ1011_API UBTTaskNode_CallOutTarget : public UBTTaskNode
{
	GENERATED_BODY()

	UBTTaskNode_CallOutTarget();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);
	
};
