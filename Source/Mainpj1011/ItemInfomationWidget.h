// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ArmorStruct.h"
#include "SpawningArmor.h"
#include "ItemInfomationWidget.generated.h"

/**
 * 
 */
UCLASS()
class MAINPJ1011_API UItemInfomationWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void bindArmor(class ASpawningArmor* NewArmor);

protected:
	virtual void NativeConstruct();
private:
	class UTextBlock* BrandName;
	class UTextBlock* TypeName;
	class UBorder* Border;
	TWeakObjectPtr<class ASpawningArmor> SpawningArmor;

public:
	void UpdateWidget();


};
