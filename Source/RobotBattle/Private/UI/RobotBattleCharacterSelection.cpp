// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/RobotBattleCharacterSelection.h"

#include "Arena/ArenaSettings.h"
#include "Characters/RobotCharacter.h"
#include "Characters/MainMenu/PlayerMenuActor.h"
#include "Kismet/GameplayStatics.h"
#include "Match/RobotGameInstance.h"
#include "UI/MenuGameMode.h"



void URobotBattleCharacterSelection::ValidateSelection(APlayerController* InController)
{
	ValidationByController[InController] = true;
	ValidateEvent.Broadcast(InController->GetLocalPlayer()->GetLocalPlayerIndex());
	
	for (const auto Pair : ValidationByController)
	{
		if (!Pair.Value)
		{
			return;
		}
	}

	for (int i = 0; i < 4; i++)
	{
		if (GameInstance->RobotID.Num() < 4)
		{
			GameInstance->RobotID.Add(ERobotID::None);
		}
	}
	
	for (TPair<APlayerController*, ERobotID> Pair : BodyPartByController)
	{
		int Index;
		GameInstance->PlayersPos.Find(Pair.Key->GetLocalPlayer()->GetLocalPlayerIndex(), Index);
		GameInstance->RobotID[Index] = Pair.Value;
	}

	TArray<AActor*> Players;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerMenuActor::StaticClass(), Players);

	for (const auto Actor : Players)
	{
		const auto TempActor = Cast<APlayerMenuActor>(Actor);

		TempActor->InputMoveEvent.RemoveDynamic(this, &URobotBattleCharacterSelection::ChangeRobotPartSelectionForPlayer);
		TempActor->InputValidateEvent.RemoveDynamic(this, &URobotBattleCharacterSelection::ValidateSelection);
		TempActor->InputCancelEvent.RemoveDynamic(this, &URobotBattleCharacterSelection::CancelSelection);
	}

	StartGameEvent.Broadcast();
}

void URobotBattleCharacterSelection::ChangeRobotPartSelectionForPlayer(FVector2D InDirection,  APlayerController* InController)
{
	if (!InController  || ValidationByController[InController] || InDirection.X == 0) { return; }

	uint8 IdAsUint = static_cast<uint8>(BodyPartByController[InController]);
	uint8 newId = IdAsUint + (InDirection.X > 0 ? 1 : -1);
	
	const UArenaSettings* ArenaSettings = GetDefault<UArenaSettings>();
	if (newId == 0) BodyPartByController[InController] = static_cast<ERobotID>(ArenaSettings->RobotCharacterDownClass.Num());
	else if (newId > ArenaSettings->RobotCharacterDownClass.Num()) BodyPartByController[InController] = ERobotID::Robot1;
	else BodyPartByController[InController] = static_cast<ERobotID>(newId);
	
	SelectionChangeEvent.Broadcast(
		InController->GetLocalPlayer()->GetLocalPlayerIndex(),
		BodyPartByController[InController],
		(InDirection.X > 0));
}

void URobotBattleCharacterSelection::CancelSelection(APlayerController* InController)
{
	ValidationByController[InController] = false;
	CancellationEvent.Broadcast(InController->GetLocalPlayer()->GetLocalPlayerIndex());
}

void URobotBattleCharacterSelection::InitializeAndBindInputs()
{
	GameInstance = Cast<URobotGameInstance>(GetWorld()->GetGameInstance());

	GameMode = Cast<AMenuGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	
	TArray<AActor*> Players;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerMenuActor::StaticClass(), Players);

	for (const auto Actor : Players)
	{
		const auto TempActor = Cast<APlayerMenuActor>(Actor);

		TempActor->InputMoveEvent.AddDynamic(this, &URobotBattleCharacterSelection::ChangeRobotPartSelectionForPlayer);
		TempActor->InputValidateEvent.AddDynamic(this, &URobotBattleCharacterSelection::ValidateSelection);
		TempActor->InputCancelEvent.AddDynamic(this, &URobotBattleCharacterSelection::CancelSelection);
	}

	for (int i = 0; i < Players.Num(); i++)
	{
		APlayerController* PlayerController = UGameplayStatics::GetPlayerControllerFromID(GetWorld(), i);
		int Index;
		GameInstance->PlayersPos.Find(PlayerController->GetLocalPlayer()->GetLocalPlayerIndex(), Index);
		BodyPartByController.Add(PlayerController, GameInstance->RobotID[Index]);
		ValidationByController.Add(PlayerController, false);
	}
}