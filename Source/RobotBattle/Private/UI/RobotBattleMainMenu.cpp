// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/RobotBattleMainMenu.h"

#include "Chaos/PBDRigidClusteringAlgo.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "UI/MenuGameMode.h"


bool URobotBattleMainMenu::Initialize()
{
	Super::Initialize();

	if (PlayBtn)
	{
		PlayBtn->OnClicked.AddDynamic(this, &URobotBattleMainMenu::StartGame);
	}

	if (OptionBtn)
	{
		OptionBtn->OnClicked.AddDynamic(this, &URobotBattleMainMenu::OptionGame);
	}
	
	if (QuitBtn)
	{
		QuitBtn->OnClicked.AddDynamic(this, &URobotBattleMainMenu::QuitGame);
	}
	
	
	return true;
}



void URobotBattleMainMenu::StartGame()
{
	AMenuGameMode* GM = Cast<AMenuGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	GM->StartSelectionTeam();
	RemoveFromParent();
}

void URobotBattleMainMenu::OptionGame()
{
	
}

void URobotBattleMainMenu::QuitGame()
{
	UKismetSystemLibrary::QuitGame(this, nullptr,EQuitPreference::Quit, true);
}