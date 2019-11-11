// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_GetIntoCover.h"
#include "TPSAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_GetIntoCover::UBTTask_GetIntoCover()
{
	NodeName = TEXT("GetIntoCover");
}

EBTNodeResult::Type UBTTask_GetIntoCover::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (ControllingPawn == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	ATPSEnemy* TPSEnemy = Cast<ATPSEnemy>(ControllingPawn);
	if(TPSEnemy == nullptr)
	{
		return EBTNodeResult::Failed;
	}

	ATPSAIController * TPSAIController = Cast<ATPSAIController>(OwnerComp.GetAIOwner());
	if (TPSAIController == nullptr)
	{
		return EBTNodeResult::Failed;
	}
	else
	{
		FVector MoveLoc = TPSAIController->GetBlackboard()->GetValueAsVector(TEXT("MoveLocation"));
		TPSAIController->MoveToLocation(MoveLoc);
		TPSEnemy->CEnterCover();
		TPSAIController->GetBlackboard()->SetValueAsBool(TEXT("IsInCover"), true);
		return EBTNodeResult::Succeeded;
	}
}
