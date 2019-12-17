// Fill out your copyright notice in the Description page of Project Settings.


#include "Skill_Support.h"
#include "TPSCharacter.h"
#include "TPSCharacterStatComponent.h"

ASkill_Support::ASkill_Support()
{
	PrimaryActorTick.bCanEverTick = true;
	GetCapsuleComponent()->SetCollisionProfileName(FName("SkillObject"));
	GetCapsuleComponent()->SetSimulatePhysics(true);
	SkillRangeVector = FVector(5.0f, 5.0f, 5.0f);

	static ConstructorHelpers::FObjectFinder<UStaticMesh>SM_BODY(TEXT("/Game/StarterContent/Shapes/Shape_Cube"));
	if (SM_BODY.Succeeded())
	{
		Body->SetStaticMesh(SM_BODY.Object);
	}
	Body->SetWorldLocation(FVector(0.0f, 0.0f, -80.0f));
	Body->SetWorldScale3D(FVector(0.3f, 0.3f, 0.3f));
	Body->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Body->SetupAttachment(GetCapsuleComponent());

	Aura = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("SupportAura"));
	static ConstructorHelpers::FObjectFinder<UParticleSystem>PS_AURA(TEXT("/Game/MyNew/Particle/SkillEffect/SupportEffect"));
	if (PS_AURA.Succeeded())
	{
		Aura->SetTemplate(PS_AURA.Object);
	}
	Aura->SetupAttachment(GetCapsuleComponent());
	Aura->SetWorldLocation(FVector(0.0f, 0.0f, -88.0f));
	SkillRange->SetCollisionProfileName(TEXT("Pulse"));
}

void ASkill_Support::BeginPlay()
{
	Super::BeginPlay();
	
	SkillRange->OnComponentBeginOverlap.AddDynamic(this, &ASkill_Support::OnActorBeginOverlap);
	SkillRange->OnComponentEndOverlap.AddDynamic(this, &ASkill_Support::OnOverlapEnd);
	Aura->SetWorldScale3D(SkillRangeVector);
	SkillRange->SetWorldScale3D(SkillRangeVector*4.0f);
	this->SetLifeSpan(10.0f);
}
void ASkill_Support::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASkill_Support::OnActorBeginOverlap(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (OtherComp->ComponentHasTag(TEXT("PLAYER")))
	{
		GetWorldTimerManager().SetTimer(HealTimer, this, &ASkill_Support::HealCharacter, 1.0f, true);
	}
}

void ASkill_Support::OnOverlapEnd(UPrimitiveComponent * OverlappedComp, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex)
{
	if (OtherComp->ComponentHasTag(TEXT("PLAYER")))
	{
		GetWorldTimerManager().ClearTimer(HealTimer);
	}
}

void ASkill_Support::HealCharacter()
{
	ATPSCharacter* TempCharacter = Cast<ATPSCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(),0));
	if (TempCharacter != nullptr)
	{
		if (TempCharacter->PlayerStatComp->PlayerHealth < TempCharacter->PlayerStatComp->PlayerMaxHealth)// 현재체력이 맥스체력보다 적다면
		{
			TempCharacter->PlayerStatComp->PlayerHealth += (5.0f + SkillPower / 50.0f);
			UE_LOG(LogType, Error, TEXT("Healling........"));
		}
	}
}
