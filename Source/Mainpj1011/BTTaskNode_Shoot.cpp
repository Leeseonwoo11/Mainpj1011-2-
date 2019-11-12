// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTaskNode_Shoot.h"
#include "TPSEnemy.h"
#include "TPSAIController.h"

EBTNodeResult::Type UBTTaskNode_Shoot::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	ATPSEnemy* TPSEnemy = Cast<ATPSEnemy>(OwnerComp.GetAIOwner()->GetPawn());
	if (TPSEnemy == nullptr)
	{
		return EBTNodeResult::Failed;
	}
	TPSEnemy->SetTrueAimState();
	TPSEnemy->CStartShooting();
	return EBTNodeResult::InProgress;
	
}

EBTNodeResult::Type UBTTaskNode_Shoot::AbortTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	ATPSEnemy* TPSEnemy = Cast<ATPSEnemy>(OwnerComp.GetAIOwner()->GetPawn());
	if (TPSEnemy == nullptr)
	{
		return EBTNodeResult::Failed;
	}
	TPSEnemy->SetFalseAimState();
	TPSEnemy->CStopShoothing();
	return EBTNodeResult::Aborted;

}
