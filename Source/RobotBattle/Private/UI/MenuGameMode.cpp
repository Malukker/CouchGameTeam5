// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MenuGameMode.h"

#include "EnhancedInputSubsystems.h"
#include "LocalMultiplayerSubsystem.h"
#include "Characters/MainMenu/PlayerMenuActor.h"
#include "Match/RobotGameInstance.h"

void AMenuGameMode::BeginPlay()
{
	Super::BeginPlay();
	CreatePlayerMenuActors();
	BindMenuInputsToPlayers();
}

void AMenuGameMode::CreatePlayerMenuActors() const
{
	const URobotGameInstance* GameInstance = GetWorld()->GetGameInstance<URobotGameInstance>();
	if (GameInstance == nullptr) { return; }

	ULocalMultiplayerSubsystem* LocalMultiplayerSubsystem = GameInstance->GetSubsystem<ULocalMultiplayerSubsystem>();
	if (LocalMultiplayerSubsystem == nullptr) { return; }

	LocalMultiplayerSubsystem->CreatePlayers();

	for (auto Controller : LocalMultiplayerSubsystem->Controllers)
	{
		if (Controller != nullptr)
		{
			APlayerMenuActor* PlayerMenuActor = GetWorld()->SpawnActor<APlayerMenuActor>(MenuActorBlueprintClass);
			Controller->Possess(PlayerMenuActor);
			PlayerMenuActor->SelfPlayerController = Controller;
		}
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
