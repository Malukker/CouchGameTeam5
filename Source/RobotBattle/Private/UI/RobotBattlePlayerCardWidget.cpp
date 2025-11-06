// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/RobotBattlePlayerCardWidget.h"
#include "Components/TextBlock.h"

void URobotBattlePlayerCardWidget::SetPlayerName(const FString& NewName)
{
	if (Text_PlayerName)
	{
		Text_PlayerName->SetText(FText::FromString(NewName));
	}
}

