// Fill out your copyright notice in the Description page of Project Settings.

#include "UI/RobotBattleCharacterSelection.h"
#include "Characters/RobotCharacter.h"
#include "Characters/MainMenu/PlayerMenuActor.h"
#include "Kismet/GameplayStatics.h"

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

	//placeholder give required data to game instance
	//Placeholder load level
}

void URobotBattleCharacterSelection::ChangeRobotPartSelectionForPlayer(EPlayerMenuInputDirection InDirection,
                                                                       APlayerController* InController)
{
	if (!InController) { return; }

	if (InDirection != EPlayerMenuInputDirection::Right &&	 InDirection != EPlayerMenuInputDirection::Left) { return; }
	
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

void URobotBattleCharacterSelection::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	BindEventsToMenuControl();
}

void URobotBattleCharacterSelection::ResetInput()
{
}

void URobotBattleCharacterSelection::BindEventsToMenuControl()
{
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
		BodyPartByController.Add(UGameplayStatics::GetPlayerControllerFromID(GetWorld(), i), ERobotID::Robot1);
		ValidationByController.Add(UGameplayStatics::GetPlayerControllerFromID(GetWorld(), i), false);
	}
}
