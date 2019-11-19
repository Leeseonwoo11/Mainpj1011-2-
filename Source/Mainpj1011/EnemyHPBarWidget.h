// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EnemyHPBarWidget.generated.h"

/**
 * 
 */
UCLASS()
class MAINPJ1011_API UEnemyHPBarWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void BindEnemyHP(class UTPSEnemyStatComponent* NewStatComponent);

protected:
	virtual void NativeConstruct();
	void UpdateHPWidget();

private:
	TWeakObjectPtr<class UTPSEnemyStatComponent> CurrentStatComponent;

	UPROPERTY()
	class UProgressBar* HPProgressBar;
};
