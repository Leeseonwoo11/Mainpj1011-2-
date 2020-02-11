// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_GetIntoCover.h"
#include "TPSAIController.h"
#include "BehaviorTree/BlackboardComponent.h"

// �� AI�� Movelocation���� �̵��ϰ� �̵��� �Ϸ��ϸ� ������·� �ൿ�ϰ� �ϴ� BTTask

// �� AI�� ��ġ�� MoveLocation�� �Ÿ��� �ִٸ� Movelocation���� �̵��ϰ� �Ÿ��� 50���϶�� AI�� ������·� ����.
// EBTNodeResult���°� Inprogress ���¶�� MoveLocation���� �����̰����� Succeeded�� �ٲ��� �ʴ´�. �׷��� TickTask���� Movelocation���� �Ÿ��� ������� ���� ��� üũ�ϰ� 
// �����ٸ� EBTNodeResult�� Succeeded�� �ٲ��ش�.
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
			|| TPSEnemy->GetActorLocation().Y >= (MoveLoc.Y + 50) || TPSEnemy->GetActorLocation().Y <= (MoveLoc.Y - 50))// MoveLoc�� �ݰ� 50�� �Ÿ� �ۿ� Movelocatoin�� ��ġ�Ѵٸ�
		{
			TPSAIController->MoveToLocation(MoveLoc); // MoveLoc���� ������
			bNotifyTick = 1;
			return EBTNodeResult::InProgress; //������
		}
		TPSAIController->GetBlackboard()->SetValueAsBool(TEXT("IsInCover"), true); // �����尪 ������� ����
		TPSEnemy->CEnterCover(); //�� ĳ���� ������·� ����
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
					||TPSEnemy->GetActorLocation().X >= (MoveLoc.X + 50) && TPSEnemy->GetActorLocation().Y >= (MoveLoc.Y - 50))// MoveLoc���� XY�� �󿡼� 50�� �Ÿ� ���� �ִٸ�
				{
					TPSAIController->GetBlackboard()->SetValueAsBool(TEXT("IsInCover"), true); // �����尪 ������� ����
					TPSEnemy->CEnterCover(); //�� ĳ���� ������·� ����
					bNotifyTick = 0;
					FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
				}

				else
				{
					//TPSEnemy->COutCover();
					TPSAIController->MoveToLocation(MoveLoc); // MoveLoc���� ������
				}
			}
		}
	}

}
