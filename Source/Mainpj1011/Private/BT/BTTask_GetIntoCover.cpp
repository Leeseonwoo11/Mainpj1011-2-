// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_GetIntoCover.h"
#include "TPSAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

// 적 AI가 Movelocation으로 이동하고 이동을 완료하면 엄페상태로 행동하게 하는 BTTask

// 적 AI의 위치와 MoveLocation의 거리가 멀다면 Movelocation으로 이동하고 거리가 50이하라면 AI는 엄페상태로 들어간다.
// EBTNodeResult상태가 Inprogress 상태라면 MoveLocation으로 움직이고나서도 Succeeded로 바뀌지 않는다. 그래서 TickTask에서 Movelocation과의 거리가 가까운지 먼지 계속 체크하고 
// 가깝다면 EBTNodeResult를 Succeeded로 바꿔준다.
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
			|| TPSEnemy->GetActorLocation().Y >= (MoveLoc.Y + 50) || TPSEnemy->GetActorLocation().Y <= (MoveLoc.Y - 50))// MoveLoc의 반경 50의 거리 밖에 Movelocatoin이 위치한다면
		{
			TPSAIController->MoveToLocation(MoveLoc); // MoveLoc으로 움직임
			bNotifyTick = 1;
			return EBTNodeResult::InProgress; //진행중
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
