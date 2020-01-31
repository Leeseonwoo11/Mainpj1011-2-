// Fill out your copyright notice in the Description page of Project Settings.


#include "SkillObject.h"

// Sets default values
ASkillObject::ASkillObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Body = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Body"));
	Body->SetupAttachment(GetCapsuleComponent());
	SkillRange = CreateDefaultSubobject<USphereComponent>(TEXT("SkillRange"));
	SkillRange->SetupAttachment(GetCapsuleComponent());
	SkillType = ESkillType::NoSkillObj;
}

// Called when the game starts or when spawned
void ASkillObject::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ASkillObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASkillObject::SetupPlayerInputComponent(UInputComponent * PlayerInputComponent)
{
}

void ASkillObject::OnActorBeginOverlap(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
}

