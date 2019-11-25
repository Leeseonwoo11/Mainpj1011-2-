// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSAIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "ConstructorHelpers.h"

const FName ATPSAIController::TargetLocation(TEXT("TargetLocation"));
const FName ATPSAIController::MoveLocation(TEXT("MoveLocation"));
const FName ATPSAIController::CanSeePlayer(TEXT("CanSeePlayer"));
const FName ATPSAIController::IsInCover(TEXT("IsInCover"));


ATPSAIController::ATPSAIController()
{
	PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("PerceptionComponent"));
	PerceptionComponent->OnPerceptionUpdated.AddDynamic(this, &ATPSAIController::PerceptionUpdate);

	SightDefaultConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	SightDefaultConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightDefaultConfig->DetectionByAffiliation.bDetectFriendlies = true;
	SightDefaultConfig->DetectionByAffiliation.bDetectNeutrals = true;
	PerceptionComponent->ConfigureSense(*SightDefaultConfig);

	static ConstructorHelpers::FObjectFinder<UBlackboardData>BB_Object(TEXT("/Game/MyNew/AI/BB_Enemy"));
	if (BB_Object.Succeeded())
	{
		BBAsset = BB_Object.Object;
	}
	static ConstructorHelpers::FObjectFinder<UBehaviorTree>BT_Object(TEXT("/Game/MyNew/AI/BT_EnemyCover"));
	if (BT_Object.Succeeded())
	{
		BTAsset = BT_Object.Object;
	}
}
void ATPSAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	ControllingEnemy = Cast<ATPSEnemy>(InPawn);
	if (UseBlackboard(BBAsset, Blackboard))
	{
		if (!RunBehaviorTree(BTAsset))
		{
			UE_LOG(LogTemp,Error,TEXT("BT is not running"));
		}
	}
}

UBlackboardComponent * ATPSAIController::GetBlackboard()
{
	return Blackboard;
}

void ATPSAIController::PerceptionUpdate(const TArray<AActor*>& UpdatedActors)
{
	for (AActor* TempActor : UpdatedActors)
	{
		FActorPerceptionBlueprintInfo ActorPerceptionInfo;
		if (PerceptionComponent->GetActorsPerception(TempActor, ActorPerceptionInfo))
		{
			for (int32 index = 0; index < ActorPerceptionInfo.LastSensedStimuli.Num(); ++index)
			{
				if (ActorPerceptionInfo.LastSensedStimuli[index].IsValid())
				{
					switch (index)
					{
					case 0: // Sight perception update
						OnSightPerceptionUpdate(TempActor, ActorPerceptionInfo.LastSensedStimuli[index]);
						break;

					default:
						UE_LOG(LogTemp, Warning, TEXT("no sense"));
						break;
					}
				}
			}
		}
	}
}

void ATPSAIController::OnSightPerceptionUpdate(AActor * TempActor, FAIStimulus Stimulus)
{
	UE_LOG(LogType, Warning, TEXT("SightPerceptioning....................."));

	if (TempActor->ActorHasTag(TEXT("PLAYER")))
	{
		if (Stimulus.WasSuccessfullySensed())
		{
			UE_LOG(LogType, Error, TEXT("Sense and AICon canseepalyer"));
			Blackboard->SetValueAsBool(CanSeePlayer, true);
			CanSeePlayerAI = true;
		}
		else
		{
			UE_LOG(LogType, Error, TEXT("Sense but AICon can not seepalyer"));

			CanSeePlayerAI = false;
			Blackboard->SetValueAsBool(CanSeePlayer, false);
		}
	}
	else
	{
		UE_LOG(LogType, Error, TEXT("No Sense AICon can not seepalyer"));

		CanSeePlayerAI = false;
		Blackboard->SetValueAsBool(CanSeePlayer, false);
	}
	if (Blackboard->GetValueAsBool(CanSeePlayer) && ControllingEnemy->bCrouchState)
	{
		Blackboard->SetValueAsBool(IsInCover, true);
	}

}
