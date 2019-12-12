// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "EngineMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemInfomationWidget.generated.h"

/**
 * 
 */
UCLASS()
class MAINPJ1011_API UItemInfomationWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void BindArmor(class ASpawningArmor* NewArmor);
	void BindWeapon(class ASpawningWeapon* NewWeapon);

protected:
	virtual void NativeConstruct();
private:
	class UTextBlock* BrandName;
	class UTextBlock* TypeName;
	class UBorder* Border;
	TWeakObjectPtr<class ASpawningArmor> SpawningArmor;
	TWeakObjectPtr<class ASpawningWeapon> SpawningWeapon;

public:
	void UpdateArmorWidget();
	void UpadateWeaponWidget();


};
