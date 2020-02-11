// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EnemyHPBarWidget.generated.h"

// �������Ʈ�� ������ ����� ���α׷������� ����̳� ������ �����Ѵ�. ->
// c++�� ������ ���ۺκ��� �����. ->
// �������Ʈ�� �θ� ����c++�������� �����Ѵ�. 
UCLASS()
class MAINPJ1011_API UEnemyHPBarWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void BindEnemyHP(class UTPSEnemyStatComponent* NewStatComponent);// HPWidget�� �� AI�� statcomponent�� �����ִ� �Լ�. TPSEnemy123Ŭ�����鿡�� ����ȴ�.

protected:
	virtual void NativeConstruct();
	void UpdateHPWidget();

private:
	TWeakObjectPtr<class UTPSEnemyStatComponent> CurrentStatComponent; //EnemyStatComponent�� ������ ����

	UPROPERTY()
	class UProgressBar* HPProgressBar;
	
};
