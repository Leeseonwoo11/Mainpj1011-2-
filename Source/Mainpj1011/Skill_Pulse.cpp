// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill_Pulse.h"
#include "TPSEnemy.h"
#include "DrawDebugHelpers.h"

ASkill_Pulse::ASkill_Pulse()
{
	PrimaryActorTick.bCanEverTick = true;

	ConstructorHelpers::FObjectFinder<UStaticMesh>SM_PULSE(TEXT("/Game/StarterContent/Shapes/Shape_QuadPyramid"));
	if (SM_PULSE.Succeeded())
	{
		Body->SetStaticMesh(SM_PULSE.Object);
	}
	ConstructorHelpers::FObjectFinder<UMaterial>M_PULSE(TEXT("/Game/StarterContent/Materials/M_Metal_Gold"));
	if (M_PULSE.Succeeded())
	{
		Body->SetMaterial(0, M_PULSE.Object);
	}
	Body->SetWorldScale3D(FVector(0.3f, 0.3f, 0.3f));
	Body->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SkillRange->SetWorldScale3D(FVector(1.0f, 1.0f, 1.0f));
	SkillRange->SetCollisionProfileName(FName("Pulse"));
}

void ASkill_Pulse::BeginPlay()
{
	StartSearching();
}

void ASkill_Pulse::Tick(float DeltaTime)
{
	UE_LOG(LogTemp, Error, TEXT("%f"),Radius);

}

void ASkill_Pulse::StartSearching()
{
	UE_LOG(LogTemp, Error, TEXT("Start Searching!!!!!!!!!!!!!"));
	TSubclassOf<ATPSEnemy>TPSEnemyPtr;
	TPSEnemyPtr = ATPSEnemy::StaticClass();
	TArray<AActor*>Enemys;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), TPSEnemyPtr, Enemys);
	for (auto Enemy : Enemys)
	{
		if (Enemy != nullptr)
		{
			UE_LOG(LogTemp, Error, TEXT("ChangeRenderCustomDepth TRUE"));
			Cast<ATPSEnemy>(Enemy)->GetMesh()->SetRenderCustomDepth(true);
		}
	}
	GetWorldTimerManager().SetTimer(PulseTimer, this, &ASkill_Pulse::EndSearching, 1.0f, false, 10.0f);
	GetWorldTimerManager().SetTimer(DebugSphereTimer, this, &ASkill_Pulse::DebugSphere, 0.01,true);
}

void ASkill_Pulse::EndSearching()
{
	GetWorldTimerManager().ClearTimer(DebugSphereTimer);
	UE_LOG(LogTemp, Error, TEXT("End Searching!!!!!!!!!!!!!"));
	TSubclassOf<ATPSEnemy>TPSEnemyPtr;
	TPSEnemyPtr = ATPSEnemy::StaticClass();
	TArray<AActor*>Enemys;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), TPSEnemyPtr, Enemys);
	for(auto Enemy : Enemys)
	{
		if (Enemy != nullptr)
		{
			UE_LOG(LogTemp, Error, TEXT("ChangeRenderCustomDepth FALSE"));
			Cast<ATPSEnemy>(Enemy)->GetMesh()->SetRenderCustomDepth(false);
		}
	}
	Destroy();
}

void ASkill_Pulse::OnActorBeginOverlap(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{

}

void ASkill_Pulse::DebugSphere()
{
	if (Radius < 500)
	{
		DrawDebugSphere(GetWorld(), Body->GetComponentLocation(), Radius, 80, FColor::Orange, false, 0.01f);
		Radius += 10.0f;
	}
}
