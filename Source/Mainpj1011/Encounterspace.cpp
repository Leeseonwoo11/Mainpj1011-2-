// Fill out your copyright notice in the Description page of Project Settings.


#include "Encounterspace.h"
#include "TPSAIController.h"

// Sets default values
AEncounterspace::AEncounterspace()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	EncountSpace = CreateDefaultSubobject<UBoxComponent>(TEXT("EncountSpace"));
	EncountSpace->SetGenerateOverlapEvents(true);
}

// Called when the game starts or when spawned
void AEncounterspace::BeginPlay()
{
	Super::BeginPlay();

	EncounterDelegate.BindUFunction(this, FName("CallOutTarget"));
}

// Called every frame
void AEncounterspace::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEncounterspace::CallOutTarget(FVector TargetLoc)
{
	TargetLocation = TargetLoc;
	TSubclassOf<ATPSEnemy> TPSEnemy;
	TPSEnemy = ATPSEnemy::StaticClass();
	TArray<AActor*> EnemyActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), TPSEnemy, EnemyActors);

	for(AActor* Enemy : EnemyActors)
	{
		if (IsOverlappingActor(Enemy))
		{
			ATPSAIController* EnemyAIController = Cast<ATPSAIController>(Cast<ATPSEnemy>(Enemy)->GetController());
			EnemyAIController->GetBlackboard()->SetValueAsVector(FName("TargetLocation"), TargetLocation);
			EnemyAIController->SetFocus(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
		}
	}
	
}
