// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_Focus.h"
#include "TPSAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "TPSCharacter.h"

// AI가 플레이어 쪽으로 방향을 틀어 플레이어를 바라보도록 명령하는 BTTask
UBTTask_Focus::UBTTask_Focus()
{
	NodeName = TEXT("Focus");
}

EBTNodeResult::Type UBTTask_Focus::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);

	auto ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (ControllingPawn == nullptr)
	{
		return EBTNodeResult::Failed;
	}
	ATPSCharacter* Character = Cast<ATPSCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (Character == nullptr)
	{
		return EBTNodeResult::Failed;
	}
	else
	{
		OwnerComp.GetAIOwner()->SetFocus(Character);
		return EBTNodeResult::Succeeded;
	}
}
