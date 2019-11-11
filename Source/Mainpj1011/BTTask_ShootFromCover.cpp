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
	if (Fireflag)
	{
		TPSEnemy->CStopShoothing();
		TPSEnemy->SetTrueCoverState();
		TPSEnemy->SetFalseAimState();
		Fireflag = false;
		Timerflag = true;
		return EBTNodeResult::Succeeded;
	}
	else
	{
		TPSEnemy->SetFalseCoverState();
		TPSEnemy->SetTrueAimState();
		TPSEnemy->CStartShooting();
		if (Timerflag)
		{
			UTPSGameInstance* GameInstance = Cast<UTPSGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
			if (GameInstance == nullptr)
			{
				return EBTNodeResult::Failed;
			}
			GameInstance->GetTimerManager().SetTimer(GameInstance->AICoverShotTimer, this, &UBTTask_ShootFromCover::SetTrueStopFire, 1.0, false, 3.0f);
			Timerflag = false;
		}

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


void UBTTask_ShootFromCover::SetTrueStopFire()
{
	Fireflag = true;
}
