// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_LoseFocus.generated.h"

/**
 * 
 */
UCLASS()
class MAINPJ1011_API UBTTask_LoseFocus : public UBTTaskNode
{
	GENERATED_BODY()
	UBTTask_LoseFocus();
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)override;
};
