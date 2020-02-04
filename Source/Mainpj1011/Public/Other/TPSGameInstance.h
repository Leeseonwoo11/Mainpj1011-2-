// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "Engine/GameInstance.h"
#include "TPSGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class MAINPJ1011_API UTPSGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UFUNCTION()
	int32 RandonNumberRet(int32 a, int32 b);

	//스코어 관련 변수,함수
private:
	UPROPERTY()
	int32 TimeScore = 0;
	UPROPERTY()
	int32 KillScore = 0;
	UPROPERTY()
	int32 ItemScore = 0;
public:
	UFUNCTION(BlueprintCallable)
	void ScoreReset();
	UFUNCTION(BlueprintCallable)
	int32 GetTotalScore();
	UFUNCTION(BlueprintCallable)
	int32 GetTimeScore();
	UFUNCTION(BlueprintCallable)
	int32 GetKillScore();
	UFUNCTION(BlueprintCallable)
	int32 GetItemScore();
	UFUNCTION()
	void AddTimeScore(int32 Score);
	UFUNCTION()
	void AddKillScore(int32 Score);
	UFUNCTION()
	void AddItemScore(int32 Score);
};
