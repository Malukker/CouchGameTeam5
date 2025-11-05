// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MenuGameMode.h"

#include "EnhancedInputSubsystems.h"
#include "LocalMultiplayerSubsystem.h"
#include "Blueprint/UserWidget.h"
#include "Characters/MainMenu/PlayerMenuActor.h"
#include "Match/RobotGameInstance.h"
#include "UI/RobotBattleTeamSelectMenu.h"
#include "UI/RobotBattleCharacterSelection.h"

void AMenuGameMode::StartSelectionCharacter()
{
	CreateWidget<URobotBattleCharacterSelection>(GetWorld(), CharacterSelectWidget)->AddToViewport();
}

void AMenuGameMode::StartSelectionTeam()
{
	CreateWidget<URobotBattleTeamSelectMenu>(GetWorld(), TeamSelectWidget)->AddToViewport();
}

void AMenuGameMode::BeginPlay()
{
	Super::BeginPlay();
	
	CreatePlayerMenuActors();
	BindMenuInputsToPlayers();
	
	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &AMenuGameMode::StartSelectionTeam);
}

void AMenuGameMode::CreatePlayerMenuActors() const
{
	const URobotGameInstance* GameInstance = GetWorld()->GetGameInstance<URobotGameInstance>();
	if (GameInstance == nullptr) { return; }

	ULocalMultiplayerSubsystem* LocalMultiplayerSubsystem = GameInstance->GetSubsystem<ULocalMultiplayerSubsystem>();
	if (LocalMultiplayerSubsystem == nullptr) { return; }

	LocalMultiplayerSubsystem->CreatePlayers();

	for (int i = 0; i < 4; i++)
	{
		APlayerMenuActor* PlayerMenuActor = GetWorld()->SpawnActorDeferred <APlayerMenuActor>(
			MenuActorBlueprintClass,
			FTransform::Identity
		);
		PlayerMenuActor->AutoPossessPlayer = TEnumAsByte<EAutoReceiveInput::Type>(i + 1);
		PlayerMenuActor->FinishSpawning(FTransform::Identity);
	}
}

void AMenuGameMode::BindMenuInputsToPlayers() const
{
	const URobotGameInstance* GameInstance = GetWorld()->GetGameInstance<URobotGameInstance>();
	if (GameInstance == nullptr) { return; }

	ULocalMultiplayerSubsystem* LocalMultiplayerSubsystem = GameInstance->GetSubsystem<ULocalMultiplayerSubsystem>();
	if (LocalMultiplayerSubsystem == nullptr) { return; }

	for (auto Controller : LocalMultiplayerSubsystem->Controllers)
	{
		if (Controller != nullptr)
		{
			if (Controller->GetLocalPlayer() != nullptr)
			{
				if (Controller->GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>() != nullptr)
				{
					Controller->GetLocalPlayer()->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>()
					          ->AddMappingContext(ImcMenu, 0);
				}
			}
		}
	}
}
