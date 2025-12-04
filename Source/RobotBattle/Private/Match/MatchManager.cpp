// Fill out your copyright notice in the Description page of Project Settings.


#include "Match/MatchManager.h"

#include "Blueprint/UserWidget.h"
#include "Match/TeamManager.h"
#include "UI/RobotBattleGameplayUI.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
AMatchManager::AMatchManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AMatchManager::BeginPlay()
{
	Super::BeginPlay();
	UIGameplay = CreateWidget<URobotBattleGameplayUI>(GetWorld(), UIGameplayClass);
	UIGameplay->AddToViewport();
	UUserWidget* UIRound = CreateWidget<UUserWidget>(GetWorld(), UIRoundClass);
	UIRound->AddToViewport();
	
	UIGameplay->OnTimeOver.AddDynamic(this, &AMatchManager::EndFightOnTimeOut);
	TScriptInterface<IUIGamePlayInterface> UIInterface = TScriptInterface<IUIGamePlayInterface>(UIGameplay);
	for (ATeamManager* Team : Teams)
	{
		Team->UIInterface = UIInterface;
		Team->DeathEvent.AddDynamic(this, &AMatchManager::EndFightOnRobotDefeat);
	}
	TeamsWin.Init(0,2);
	Round = 0;
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [&]()
	{
		ResetFight();
	}, .1f, false);
}

void AMatchManager::ResetFight()
{
	for (ATeamManager* Team : Teams)
	{
		Team->SetInput(false);
	}
	RoundEvent.Broadcast(Round);
	for (ATeamManager* Team : Teams)
	{
		Team->ResetCharacters();
	}
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [&]()
	{
		for (ATeamManager* Team : Teams)
		{
			Team->PlayIntro();
		}
	}, 1.5f, false);
	FTimerHandle TimerHandleSecond;
	GetWorld()->GetTimerManager().SetTimer(TimerHandleSecond, [&]()
	{
		for (ATeamManager* Team : Teams)
		{
			Team->SetInput(true);
		}
		UIGameplay->StartTimer(RoundTime);
	}, 5.f, false);
}

void AMatchManager::EndFightOnTimeOut()
{
	if (Teams[0]->GetLife() > Teams[1]->GetLife())
	{
		EndFightOnRobotDefeat(1);
	}
	else if (Teams[1]->GetLife() > Teams[0]->GetLife())
	{
		EndFightOnRobotDefeat(0);
	}
	else
	{
		ResetFight();
	}
}

void AMatchManager::EndFightOnRobotDefeat(int LosingTeam)
{
	Round++;
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
		if (TeamWin == 2)
		{
			UIGameplay->RemoveFromParent();
			UUserWidget* UIGameOver = CreateWidget<UUserWidget>(GetWorld(), UIGameOverClass);
			UIGameOver->AddToViewport();
			UGameplayStatics::SetGamePaused(GetWorld(), true);
			return;
		}
	}
	if (TeamsWin[0] == 1 && TeamsWin[1] == 1)
	{
		for (ATeamManager* Team : Teams)
		{
			Team->InversePlayer();
		}
	}
	ResetFight();
}

// Called every frame
void AMatchManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

