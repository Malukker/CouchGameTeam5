// Fill out your copyright notice in the Description page of Project Settings.


#include "Match/MatchManager.h"

#include "Blueprint/UserWidget.h"
#include "Match/TeamManager.h"
#include "UI/RobotBattleHUD.h"
#include "UI/RobotBattleGameOverMenu.h"


// Sets default values
AMatchManager::AMatchManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	TeamsWin.Init(0,2);
}

// Called when the game starts or when spawned
void AMatchManager::BeginPlay()
{
	Super::BeginPlay();
	UIGameplay = CreateWidget<URobotBattleHUD>(GetWorld(), UIGameplayClass);
	UIGameplay->AddToViewport();
	UIGameplay->StartTimer(RoundTime);
	TScriptInterface<IUIGamePlayInterface> UIInterface = TScriptInterface<IUIGamePlayInterface>(UIGameplay);
	for (ATeamManager* Team : Teams)
	{
		Team->UIInterface = UIInterface;
		Team->DeathEvent.AddDynamic(this, &AMatchManager::EndFight);
	}
}

void AMatchManager::ResetFight()
{
	for (ATeamManager* Team : Teams)
	{
		Team->ResetCharacters();
	}
	UIGameplay->StartTimer(RoundTime);
}

void AMatchManager::EndFight(int LosingTeam)
{
	if (LosingTeam == 0)
	{
		TeamsWin[1]++;
		UIGameplay->SetRoundPlayer(1, TeamsWin[1]);
	}
	else if (LosingTeam == 1)
	{
		TeamsWin[0]++;
		UIGameplay->SetRoundPlayer(0, TeamsWin[0]);
	}
	for (int TeamWin : TeamsWin)
	{
		if (TeamWin == 1)
		{
			UIGameplay->RemoveFromParent();
			URobotBattleGameOverMenu* UIGameOver = CreateWidget<URobotBattleGameOverMenu>(GetWorld(), UIGameOverClass);
			UIGameOver->AddToViewport();
			return;
		}
	}
	ResetFight();
}

// Called every frame
void AMatchManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

