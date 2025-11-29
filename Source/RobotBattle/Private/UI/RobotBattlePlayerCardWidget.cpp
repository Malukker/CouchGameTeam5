// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/RobotBattlePlayerCardWidget.h"
#include "Components/Image.h"

void URobotBattlePlayerCardWidget::SetPlayerImage(int32 PlayerID)
{
	if (!Image_Player) return;

	UTexture2D*  TextureToUse = nullptr;

	switch (PlayerID)
	{
	case 0:
		TextureToUse = PlayerImage1;
		break;
	case 1:
		TextureToUse = PlayerImage2;
		break;
	case 2:
		TextureToUse = PlayerImage3;
		break;
	case 3:
		TextureToUse = PlayerImage4;
		break;
	default:
		break;
	}
	
	if (TextureToUse)
	{
		Image_Player->SetBrushFromTexture(TextureToUse);
	}
}

