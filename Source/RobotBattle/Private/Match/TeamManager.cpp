// Fill out your copyright notice in the Description page of Project Settings.


#include "Match/TeamManager.h"

#include "Characters/RobotCharacter.h"
#include "Arena/ArenaPlayerStart.h"
#include "Arena/ArenaSettings.h"
#include "InputMappingContext.h"
#include <Characters/RobotCharacterSettings.h>
#include <Characters/RobotCharacterInputData.h>

#include "LocalMultiplayerSubsystem.h"


// Sets default values
ATeamManager::ATeamManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called every frame
void ATeamManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATeamManager::SpawnCharacters()
{
	TSubclassOf<ARobotCharacter> RobotCharacterDOWNClass = GetRobotCharacterClassFromID(Team * 2, ERobotCharacterPositionEnum::Down);
	if (RobotCharacterDOWNClass == nullptr) return;

	ARobotCharacter* NewCharacter = GetWorld()->SpawnActorDeferred <ARobotCharacter>(
		RobotCharacterDOWNClass,
		SpawnPoint->GetTransform()
	);
	
	if (NewCharacter == nullptr) return;
	RobotParts.Add(NewCharacter);
	
	TSubclassOf<ARobotCharacter> RobotCharacterUPClass = GetRobotCharacterClassFromID(Team * 2, ERobotCharacterPositionEnum::Down);
	if (RobotCharacterUPClass == nullptr) return;
	//TO DO
	//SPAWN UP CHARACTER ON SOCKET FROM DOWN CHARACTER
	NewCharacter = GetWorld()->SpawnActorDeferred <ARobotCharacter>(
		RobotCharacterDOWNClass,
		SpawnPoint->GetTransform()
	);
	
	if (NewCharacter == nullptr) return;
	RobotParts.Add(NewCharacter);

	InitCharacters();
}

void ATeamManager::InitCharacters()
{
	URobotCharacterInputData* InputData = LoadInputDataFromConfig();
	UInputMappingContext* InputMappingContextDown = LoadInputMappingContextFromConfig(ERobotCharacterPositionEnum::Down);
	UInputMappingContext* InputMappingContextUp = LoadInputMappingContextFromConfig(ERobotCharacterPositionEnum::Up);
	
	for (TObjectPtr<ARobotCharacter> Character : RobotParts)
	{
		Character->InputData = InputData;
		switch (Character->GetPositionEnum())
		{
		case ERobotCharacterPositionEnum::Down:
			Character->InputMappingContext = InputMappingContextDown;
			break;
		case ERobotCharacterPositionEnum::Up:
			Character->InputMappingContext = InputMappingContextUp;
			break;
		default:
			return;
		}
		Character->AutoPossessPlayer = SpawnPoint->AutoReceiveInput;
		Character->SetOrientX(SpawnPoint->GetStartOrientX());
		Character->FinishSpawning(SpawnPoint->GetTransform());
	}
	
	UGameInstance* GameInstance = GetWorld()->GetGameInstance();
	if (GameInstance == nullptr) return;

	ULocalMultiplayerSubsystem* LocalMultiplayerSubsystem = GameInstance->GetSubsystem<ULocalMultiplayerSubsystem>();
	if (LocalMultiplayerSubsystem == nullptr) return;
}

TSubclassOf<ARobotCharacter> ATeamManager::GetRobotCharacterClassFromID(uint8 ID, ERobotCharacterPositionEnum Pos) const 
{
	const UArenaSettings* ArenaSettings = GetDefault<UArenaSettings>();
	switch (Pos)
	{
		case ERobotCharacterPositionEnum::Down:
			return ArenaSettings->RobotCharacterDownClass[ID];
		case ERobotCharacterPositionEnum::Up:
			return ArenaSettings->RobotCharacterUpClass[ID];
	}
	return nullptr;
}

URobotCharacterInputData* ATeamManager::LoadInputDataFromConfig() {
	const URobotCharacterSettings* CharacterSettings = GetDefault<URobotCharacterSettings>();
	if (CharacterSettings == nullptr) return nullptr;
	return CharacterSettings->InputData.LoadSynchronous();
}

UInputMappingContext* ATeamManager::LoadInputMappingContextFromConfig(ERobotCharacterPositionEnum Position) {
	const URobotCharacterSettings* CharacterSettings = GetDefault<URobotCharacterSettings>();
	if (CharacterSettings == nullptr) return nullptr;
	switch (Position)
	{
	case ERobotCharacterPositionEnum::Down:
		return CharacterSettings->InputMappingContextDown.LoadSynchronous();

	case ERobotCharacterPositionEnum::Up:
		return CharacterSettings->InputMappingContextUp.LoadSynchronous();

	case ERobotCharacterPositionEnum::None:
		return nullptr;
	}
	return nullptr;
}
