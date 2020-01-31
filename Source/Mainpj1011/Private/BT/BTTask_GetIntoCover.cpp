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
		if (TPSEnemy->GetActorLocation().X >= (MoveLoc.X + 50) || TPSEnemy->GetActorLocation().X <= (MoveLoc.X - 50)
			|| TPSEnemy->GetActorLocation().Y >= (MoveLoc.Y + 50) || TPSEnemy->GetActorLocation().Y <= (MoveLoc.Y - 50))// MoveLoc에서 XY축 상에서 100의 거리 내에 없다면
		{
			TPSAIController->MoveToLocation(MoveLoc); // MoveLoc으로 움직임
			bNotifyTick = 1;
			return EBTNodeResult::InProgress;
		}
		TPSAIController->GetBlackboard()->SetValueAsBool(TEXT("IsInCover"), true); // 블랙보드값 엄페상태 만듬
		TPSEnemy->CEnterCover(); //적 캐릭터 엄페상태로 만듬
		return EBTNodeResult::Succeeded;
	}
}

void UBTTask_GetIntoCover::TickTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
	auto ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (ControllingPawn != nullptr)
	{
		ATPSEnemy* TPSEnemy = Cast<ATPSEnemy>(ControllingPawn);
		if (TPSEnemy != nullptr)
		{
			ATPSAIController * TPSAIController = Cast<ATPSAIController>(OwnerComp.GetAIOwner());
			if (TPSAIController != nullptr)
			{
				FVector MoveLoc = TPSAIController->GetBlackboard()->GetValueAsVector(TEXT("MoveLocation"));
				if (TPSEnemy->GetActorLocation().X >= (MoveLoc.X + 50) && TPSEnemy->GetActorLocation().Y >= (MoveLoc.Y + 50)
					|| TPSEnemy->GetActorLocation().X >= (MoveLoc.X - 50) && TPSEnemy->GetActorLocation().Y >= (MoveLoc.Y - 50)
					|| TPSEnemy->GetActorLocation().X <= (MoveLoc.X - 50) && TPSEnemy->GetActorLocation().Y <= (MoveLoc.Y + 50)
					||TPSEnemy->GetActorLocation().X >= (MoveLoc.X + 50) && TPSEnemy->GetActorLocation().Y >= (MoveLoc.Y - 50))// MoveLoc에서 XY축 상에서 50의 거리 내에 있다면
				{
					TPSAIController->GetBlackboard()->SetValueAsBool(TEXT("IsInCover"), true); // 블랙보드값 엄페상태 만듬
					TPSEnemy->CEnterCover(); //적 캐릭터 엄페상태로 만듬
					bNotifyTick = 0;
					FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
				}

				else
				{
					//TPSEnemy->COutCover();
					TPSAIController->MoveToLocation(MoveLoc); // MoveLoc으로 움직임
				}
			}
		}
	}
}
