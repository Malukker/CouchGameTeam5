// Fill out your copyright notice in the Description page of Project Settings.


#include "Match/MatchManager.h"

#include "Blueprint/UserWidget.h"
#include "Match/TeamManager.h"
#include "UI/RobotBattleHUD.h"


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
	URobotBattleHUD* UI = CreateWidget<URobotBattleHUD>(UIGameplay);
	UI->AddToViewport();
	IUIGamePlayInterface* UIInterface = Cast<IUIGamePlayInterface>(UI);
	for (ATeamManager* Team : Teams)
	{
		Team->UIInterface = UIInterface;
	}
}

// Called every frame
void AMatchManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

