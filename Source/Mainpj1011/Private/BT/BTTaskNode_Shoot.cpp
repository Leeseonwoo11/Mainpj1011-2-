// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTaskNode_Shoot.h"
#include "TPSEnemy.h"
#include "TPSAIController.h"

// 서서쏴 자세로 조준,발사 역할을 수행하는 BTTask
// Succeeded를 리턴하지 않아서 BT의 조건만 맞는다면 계속 발사한다.
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
