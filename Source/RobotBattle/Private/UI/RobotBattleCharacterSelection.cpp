// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/RobotBattleCharacterSelection.h"
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

	RemoveFromParent();
	GameMode->LoadBattleLevel();
}

void URobotBattleCharacterSelection::ChangeRobotPartSelectionForPlayer(EPlayerMenuInputDirection InDirection,
                                                                       APlayerController* InController)
{
	if (!InController  || ValidationByController[InController]) { return; }

	if (InDirection != EPlayerMenuInputDirection::Right && InDirection != EPlayerMenuInputDirection::Left) { return; }
	
	TArray<APlayerController*> PlayerControllers;
	BodyPartByController.GetKeys(PlayerControllers);
	for (auto PlayerController : PlayerControllers)
	{
		if (PlayerController == InController)
		{
			switch (BodyPartByController[InController])
			{
			case ERobotID::Robot1:
				BodyPartByController[InController] = ERobotID::Robot2;
				break;
			case ERobotID::Robot2:
				BodyPartByController[InController] = ERobotID::Robot1;
				break;
			default:
				break;
			}

			SelectionChangeEvent.Broadcast(
				InController->GetLocalPlayer()->GetLocalPlayerIndex(),
				BodyPartByController[InController]);
		}
	}
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