// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EnemyHPBarWidget.generated.h"

// 블루프린트로 위젯을 만들고 프로그래스바의 모양이나 색깔을 지정한다. ->
// c++로 위젯의 동작부분을 만든다. ->
// 블루프린트의 부모를 만든c++위젯으로 변경한다. 
UCLASS()
class MAINPJ1011_API UEnemyHPBarWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	void BindEnemyHP(class UTPSEnemyStatComponent* NewStatComponent);// HPWidget과 적 AI의 statcomponent와 묶어주는 함수. TPSEnemy123클래스들에서 실행된다.

protected:
	virtual void NativeConstruct();
	void UpdateHPWidget();

private:
	TWeakObjectPtr<class UTPSEnemyStatComponent> CurrentStatComponent; //EnemyStatComponent의 포인터 설정

	UPROPERTY()
	class UProgressBar* HPProgressBar;
	
};
