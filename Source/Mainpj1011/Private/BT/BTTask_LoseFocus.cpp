// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_LoseFocus.h"
#include "TPSAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
// AI�� �÷��̾ ��ġ�� �ü��� ������ ������ �����ϴ� BTTask.
UBTTask_LoseFocus::UBTTask_LoseFocus()
{
	NodeName = TEXT("LoseFocus");
}

EBTNodeResult::Type UBTTask_LoseFocus::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (ControllingPawn == nullptr)
	{
		return EBTNodeResult::Failed;
	}
	else
	{
		OwnerComp.GetAIOwner()->ClearFocus(EAIFocusPriority::Gameplay);
		return EBTNodeResult::Succeeded;
	}
}
