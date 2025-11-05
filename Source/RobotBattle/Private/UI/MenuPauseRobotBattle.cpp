// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MenuPauseRobotBattle.h"

#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "UI/RobotBattleGameOverMenu.h"

bool UMenuPauseRobotBattle::Initialize()
{
	Super::Initialize();

	if (ResumeBtn)
	{
		ResumeBtn->OnClicked.AddDynamic(this, &UMenuPauseRobotBattle::ResumeGame);
	}

	if (MainMenuBtn)
	{
		MainMenuBtn->OnClicked.AddDynamic(this, &UMenuPauseRobotBattle::MainMenuGame);
	}

	if (OptionBtn)
	{
		OptionBtn->OnClicked.AddDynamic(this, &UMenuPauseRobotBattle::OptionGame);
	}

	return true;
}

void UMenuPauseRobotBattle::ResumeGame()
{
	UGameplayStatics::SetGamePaused(GetWorld(), false);
}

void UMenuPauseRobotBattle::MainMenuGame()
{
	UGameplayStatics::OpenLevel(this,FName(""));
}

void UMenuPauseRobotBattle::OptionGame()
{
	RemoveFromParent();
	
	if (!SettingwidgetInstance)
	{
		SettingwidgetInstance = CreateWidget<UUserWidget>(GetWorld(),OptionMenu);
	}

	if (SettingwidgetInstance && !SettingwidgetInstance->IsInViewport())
	{
		SettingwidgetInstance->AddToViewport();
	}

	
}