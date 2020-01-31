// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Perception/AIPerceptionTypes.h"
#include "TPSEnemy.h"
#include "TPSAIController.generated.h"

/**
 * 
 */
UCLASS()
class MAINPJ1011_API ATPSAIController : public AAIController
{
	GENERATED_BODY()
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	class UAISenseConfig_Sight* SightDefaultConfig;

protected:
	UFUNCTION()
	void PerceptionUpdate(const TArray<AActor*>& UpdatedActors);

	virtual void OnSightPerceptionUpdate(AActor* actor, FAIStimulus stimulus);

public:
	ATPSAIController();
	virtual void OnPossess(APawn* InPawn) override;

	static const FName TargetLocation;
	static const FName MoveLocation;
	static const FName CanSeePlayer;
	static const FName IsInCover;

	UPROPERTY()
	bool CanSeePlayerAI = false;
	UPROPERTY()
	bool IsInCoverAI = false;

	UFUNCTION()
	UBlackboardComponent* GetBlackboard();

	ATPSEnemy* ControllingEnemy;

public:
	UPROPERTY()
		class UBehaviorTree* BTAsset;
	UPROPERTY()
		class UBlackboardData* BBAsset;

};
