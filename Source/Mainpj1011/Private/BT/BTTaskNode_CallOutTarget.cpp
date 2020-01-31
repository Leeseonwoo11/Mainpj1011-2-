// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTaskNode_CallOutTarget.h"
#include "TPSAIController.h"
#include "Encounterspace.h"

UBTTaskNode_CallOutTarget::UBTTaskNode_CallOutTarget()
{
}

EBTNodeResult::Type UBTTaskNode_CallOutTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{

	TSubclassOf<AEncounterspace> EncounterSpace;
	EncounterSpace = AEncounterspace::StaticClass();
	TArray<AActor*> EncounterActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), EncounterSpace, EncounterActors);

	ATPSAIController* EnemyAI = Cast<ATPSAIController>(OwnerComp.GetAIOwner());
	if (EnemyAI == nullptr)
	{
		return EBTNodeResult::Failed;
	}
	FVector TargetLoc = EnemyAI->GetBlackboard()->GetValueAsVector(FName("TargetLocation"));

	if (EncounterActors.Num() <= 0)
	{
		return EBTNodeResult::Failed;
	}
	AEncounterspace* EncSpace = Cast<AEncounterspace>(EncounterActors.Pop());
	if (EncSpace == nullptr)
	{
		return EBTNodeResult::Failed;
	}
	else
	{
		EncSpace->EncounterDelegate.ExecuteIfBound(TargetLoc);
		return EBTNodeResult::Succeeded;
	}
	
	return EBTNodeResult::Failed;

}
