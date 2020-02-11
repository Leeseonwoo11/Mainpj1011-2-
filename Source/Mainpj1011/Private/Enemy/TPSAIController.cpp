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
	static ConstructorHelpers::FObjectFinder<UBlackboardData>BB_Object(TEXT("/Game/MyNew/AI/BB_Enemy")); //블랙보드 지정
	if (BB_Object.Succeeded())
	{
		BBAsset = BB_Object.Object;
	}
	static ConstructorHelpers::FObjectFinder<UBehaviorTree>BT_Object(TEXT("/Game/MyNew/AI/BT_EnemyCover1")); // 비헤이비어트리 지정
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

void ATPSAIController::PerceptionUpdate(const TArray<AActor*>& UpdatedActors) // 지각 없데이트 AI는 시각만 사용하므로 Switch에 case 가 한개뿐이다.
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
		if (Stimulus.WasSuccessfullySensed()) //감각이 살아있고 플레이어를 발견했다.
		{
			UE_LOG(LogType, Error, TEXT("Sense and AICon canseepalyer"));
			Blackboard->SetValueAsBool(CanSeePlayer, true);
			CanSeePlayerAI = true;
		}
		else //감각은 살아있지만 플레이어를 발견하지 못했다.
		{
			UE_LOG(LogType, Error, TEXT("Sense but AICon can not seepalyer"));

			CanSeePlayerAI = false;
			Blackboard->SetValueAsBool(CanSeePlayer, false);
		}
	}
	else //감각이 죽어있고 플레이어도 찾지못했다.
	{
		UE_LOG(LogType, Error, TEXT("No Sense AICon can not seepalyer"));

		CanSeePlayerAI = false;
		Blackboard->SetValueAsBool(CanSeePlayer, false);
	}
	
	if (Blackboard->GetValueAsBool(CanSeePlayer) && ControllingEnemy->bCrouchState) // 블랙보드에서 플레이어를 찾았고 컨트롤하는 Enemy가 무릎쏴 상태라면 엄페 공격상태(IsInCover)로 바꿔준다.
	{
		Blackboard->SetValueAsBool(IsInCover, true);
	}

}
