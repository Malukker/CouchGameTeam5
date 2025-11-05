// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/RobotBattleGameOverMenu.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

bool URobotBattleGameOverMenu::Initialize()
{
	Super::Initialize();

	if (RestartBtn)
	{
		RestartBtn->OnClicked.AddDynamic(this, &URobotBattleGameOverMenu::OnClickedChangeCharacterButton);
	}

	if (ChangeCharacterBtn)
	{
		ChangeCharacterBtn->OnClicked.AddDynamic(this, &URobotBattleGameOverMenu::OnClickedChangeCharacterButton);
	}

	if (MainMenuBtn)
	{
		MainMenuBtn->OnClicked.AddDynamic(this, &URobotBattleGameOverMenu::OnClickedMainMenuButton);
	}
	
	return true;
	
}

void URobotBattleGameOverMenu::OnClickedChangeCharacterButton()
{
	
}

void URobotBattleGameOverMenu::OnClickedMainMenuButton()
{
	UGameplayStatics::OpenLevel(this, FName(""));
}

void URobotBattleGameOverMenu::OnClickedRestartButton()
{
	UGameplayStatics::OpenLevel(this, FName(UGameplayStatics::GetCurrentLevelName(this, true)));
}
