// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSGameInstance.h"

int32 UTPSGameInstance::RandonNumberRet(int32 a, int32 b)
{
	return FMath::RandRange(a, b);
}

void UTPSGameInstance::ScoreReset()
{
	TimeScore = 0;
	KillScore = 0;
	ItemScore = 0;
}

int32 UTPSGameInstance::GetTotalScore()
{
	int32 TotalScore = TimeScore + KillScore + ItemScore;
	return TotalScore;
}

int32 UTPSGameInstance::GetTimeScore()
{
	return TimeScore;
}

int32 UTPSGameInstance::GetKillScore()
{
	return KillScore;
}

int32 UTPSGameInstance::GetItemScore()
{
	return ItemScore;
}

void UTPSGameInstance::AddTimeScore(int32 Score)
{
	TimeScore += Score;
}

void UTPSGameInstance::AddKillScore(int32 Score)
{
	KillScore += Score;
}

void UTPSGameInstance::AddItemScore(int32 Score)
{
	ItemScore += Score;
}
