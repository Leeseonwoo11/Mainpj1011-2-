// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_ShootFromCover.h"
#include "TPSAIController.h"
#include "TPSGameInstance.h"

//엄페상태에서 플레이어를 공격하는 역할을 수행하는 BTTask

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

EBTNodeResult::Type UBTTask_ShootFromCover::AbortTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory) // 작업을 중단함(조준,사격,엄페)
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

void UBTTask_ShootFromCover::TickTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds) //틱에서 30발을 다 쐇다면 Succeeded를 리턴해줘서 작업을 완료한다.
{
	ATPSEnemy* TPSEnemy = Cast<ATPSEnemy>(OwnerComp.GetAIOwner()->GetPawn());
	if (TPSEnemy != nullptr)
	{
		if (TPSEnemy->count%100 == 30) // 30발 사격완료시 조준,사격,엄페 중지 후 Succeeded리턴
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



