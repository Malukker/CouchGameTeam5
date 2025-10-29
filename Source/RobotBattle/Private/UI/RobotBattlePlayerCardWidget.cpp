// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/TextBlock.h"
#include "UI/RobotBattlePlayerCardWidget.h"

void URobotBattlePlayerCardWidget::SetPlayerName(const FString& NewName)
{
	if (Text_PlayerName)
	{
		Text_PlayerName->SetText(FText::FromString(NewName));
	}
}
