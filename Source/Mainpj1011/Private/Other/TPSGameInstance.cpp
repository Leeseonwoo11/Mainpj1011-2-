// Fill out your copyright notice in the Description page of Project Settings.


#include "TPSGameInstance.h"

int32 UTPSGameInstance::RandonNumberRet(int32 a, int32 b)
{
	return FMath::RandRange(a, b);
}