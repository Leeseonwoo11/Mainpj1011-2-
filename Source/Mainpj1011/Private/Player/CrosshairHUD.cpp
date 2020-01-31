// Fill out your copyright notice in the Description page of Project Settings.


#include "CrosshairHUD.h"
#include "Engine/Canvas.h"



ACrosshairHUD::ACrosshairHUD()
{
	ConstructorHelpers::FObjectFinder<UTexture2D>T_CROSS(TEXT("/Game/MyNew/HUD/crosshair"));
	if (T_CROSS.Succeeded())
	{
		CrosshairTexture = T_CROSS.Object;
	}
}

void ACrosshairHUD::DrawHUD()
{
	Super::DrawHUD();
	if (CrosshairTexture)
	{
		FVector2D Center(Canvas->ClipX*0.5f, Canvas->ClipY*0.5f);
		FVector2D CrossHairDrawPosition(Center.X - (CrosshairTexture->GetSurfaceWidth()*0.5f), Center.Y - (CrosshairTexture->GetSurfaceHeight()*0.5f));
		FCanvasTileItem TileItem(CrossHairDrawPosition, CrosshairTexture->Resource, FLinearColor::White);
		TileItem.BlendMode = SE_BLEND_Translucent;
		Canvas->DrawItem(TileItem);
	}
}
