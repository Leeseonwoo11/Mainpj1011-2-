// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_ShootFromCover.h"
#include "TPSAIController.h"
#include "TPSGameInstance.h"


UBTTask_ShootFromCover::UBTTask_ShootFromCover()
{
	NodeName = TEXT("ShootFromCover");
	bNotifyTaskFinished = 1;
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
	if ((int32)DeltaSeconds % 500 >= 400)
	{
		this->ReturnSuccessFunc();
	}
}

void UBTTask_ShootFromCover::OnTaskFinished(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, EBTNodeResult::Type TaskResult)
{
	FTimerManager TimeManager;
	
	if(TaskResult == EBTNodeResult::InProgress )
	{
		uint32 bNotifyTick = 1;
	}
}

EBTNodeResult::Type UBTTask_ShootFromCover::ReturnSuccessFunc()
{
	uint32 bNotifyTick = 0;
	return EBTNodeResult::Succeeded;
}


