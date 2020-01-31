// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_ShootFromCover.h"
#include "TPSAIController.h"
#include "TPSGameInstance.h"


UBTTask_ShootFromCover::UBTTask_ShootFromCover()
{
	NodeName = TEXT("ShootFromCover");
}

EBTNodeResult::Type UBTTask_ShootFromCover::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{

	ATPSEnemy* TPSEnemy = Cast<ATPSEnemy>(OwnerComp.GetAIOwner()->GetPawn());
	if (TPSEnemy == nullptr)
	{
		return EBTNodeResult::Failed;
	}
	else
	{
		TPSEnemy->SetFalseCoverState();
		TPSEnemy->SetTrueAimState();
		TPSEnemy->CStartShooting();
		bNotifyTick = 1;
		return EBTNodeResult::InProgress;
	}
}

EBTNodeResult::Type UBTTask_ShootFromCover::AbortTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	ATPSEnemy* TPSEnemy = Cast<ATPSEnemy>(OwnerComp.GetAIOwner()->GetPawn());
	if (TPSEnemy == nullptr)
	{
		return EBTNodeResult::Failed;
	}
	else
	{
		TPSEnemy->SetFalseCoverState();
		TPSEnemy->SetFalseAimState();
		TPSEnemy->CStopShoothing();
		return EBTNodeResult::Aborted;
	}
}

void UBTTask_ShootFromCover::TickTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
	ATPSEnemy* TPSEnemy = Cast<ATPSEnemy>(OwnerComp.GetAIOwner()->GetPawn());
	if (TPSEnemy != nullptr)
	{
		if (TPSEnemy->count%100 == 30)
		{
			TPSEnemy->SetTrueCoverState();
			TPSEnemy->SetFalseAimState();
			TPSEnemy->CStopShoothing();
			TPSEnemy->count = 0;
			bNotifyTick = 0;
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}
	}
}



