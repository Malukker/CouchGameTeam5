// Fill out your copyright notice in the Description page of Project Settings.


#include "Match/MatchGameMode.h"

#include "Kismet/GameplayStatics.h"
#include "LocalMultiplayer/Public/LocalMultiplayerSubsystem.h"
#include "Match/TeamManager.h"


void AMatchGameMode::BeginPlay() {
	Super::BeginPlay();
	CreateAndInitPlayers();
}

void AMatchGameMode::CreateAndInitPlayers() const{
	UGameInstance* GameInstance = GetWorld()->GetGameInstance();
	if (GameInstance == nullptr) return;

	ULocalMultiplayerSubsystem* LocalMultiplayerSubsystem = GameInstance->GetSubsystem<ULocalMultiplayerSubsystem>();
	if (LocalMultiplayerSubsystem == nullptr) return;

	LocalMultiplayerSubsystem->CreatePlayers();
	
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATeamManager::StaticClass(), FoundActors);

	for (int i = 0; i < FoundActors.Num(); ++i) {
		ATeamManager* ATeamManagerActor = Cast<ATeamManager>(FoundActors[i]);
		if (ATeamManagerActor == nullptr) continue;

		ATeamManagerActor->SpawnCharacters();
	}
}