// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/RobotBattleCharacterSelection.h"

#include "Characters/RobotCharacter.h"
#include "Characters/MainMenu/PlayerMenuActor.h"
#include "Kismet/GameplayStatics.h"

void URobotBattleCharacterSelection::AssignPlayerControllerToRobotPart(APlayerController* InController)
{
}

void URobotBattleCharacterSelection::MoveToNextPart(EPlayerMenuInputDirection InDirection,
                                                    APlayerController* InController)
{
	if (!InController) { return; }

	if (InDirection != EPlayerMenuInputDirection::Left || InDirection == EPlayerMenuInputDirection::Right) { return; }

	TArray<APlayerController*> PlayerControllers;
	BodyPartFromController.GetKeys(PlayerControllers);
	for (auto PlayerController : PlayerControllers)
	{
		if (PlayerController == InController)
		{
			switch (BodyPartFromController[InController])
			{
			case ERobotID::Robot1:
				BodyPartFromController[InController] = ERobotID::Robot2;
				break;
			case ERobotID::Robot2:
				BodyPartFromController[InController] = ERobotID::Robot1;
				break;
			default:
				break;
			}
			break;
		}
	}
}

void URobotBattleCharacterSelection::CancelAssignment(APlayerController* InController)
{
}

void URobotBattleCharacterSelection::NativeOnInitialized()
{
	Super::NativeOnInitialized();
	BindEventsToMenuControl();
}

void URobotBattleCharacterSelection::BindEventsToMenuControl()
{
	TArray<AActor*> Players;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerMenuActor::StaticClass(), Players);

	for (auto Actor : Players)
	{
		Cast<APlayerMenuActor>(Actor)->InputMoveEvent.AddDynamic(this, &URobotBattleCharacterSelection::MoveToNextPart);
	}
}
