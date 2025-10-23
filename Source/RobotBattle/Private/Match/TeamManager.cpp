// Fill out your copyright notice in the Description page of Project Settings.


#include "Match/TeamManager.h"

#include "Characters/RobotCharacter.h"
#include "Characters/RobotCharacterUp.h"
#include "Characters/RobotCharacterDown.h"

#include "Arena/ArenaPlayerStart.h"
#include "Arena/ArenaSettings.h"
#include <Characters/RobotCharacterSettings.h>
#include <Characters/RobotCharacterInputData.h>
#include "LocalMultiplayerSubsystem.h"
#include "InputMappingContext.h"
#include "Match/RobotGameInstance.h"


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

	if (GetOpponentLocation().X - GetTeamLocation().X > 0)
	{
		RobotParts[ERobotCharacterPositionEnum::Down]->SetOrientX(1);
		RobotParts[ERobotCharacterPositionEnum::Up]->SetOrientX(1);
	}
	else 
	{
		RobotParts[ERobotCharacterPositionEnum::Down]->SetOrientX(-1);
		RobotParts[ERobotCharacterPositionEnum::Up]->SetOrientX(-1);
	}
}

void ATeamManager::SpawnCharacters()
{
	URobotGameInstance* GameInstance = GetWorld()->GetGameInstance<URobotGameInstance>();
	if (GameInstance == nullptr) return;
	
	ULocalMultiplayerSubsystem* LocalMultiplayerSubsystem = GameInstance->GetSubsystem<ULocalMultiplayerSubsystem>();
	if (LocalMultiplayerSubsystem == nullptr) return;

	URobotCharacterInputData* InputData = LoadInputDataFromConfig();
	UInputMappingContext* InputMappingContextDown = LoadInputMappingContextFromConfig(ERobotCharacterPositionEnum::Down);
	UInputMappingContext* InputMappingContextUp = LoadInputMappingContextFromConfig(ERobotCharacterPositionEnum::Up);

	TeamGuardMax = 0;
	TeamGuard = 0;

	// 0 for Down and 1 for Up
	for (int PartNb = 0; PartNb < 2; PartNb++)
	{
		ERobotCharacterPositionEnum Pos = ERobotCharacterPositionEnum::None;
		if (PartNb == 0) Pos = ERobotCharacterPositionEnum::Down;
		else Pos = ERobotCharacterPositionEnum::Up;
		
		TSubclassOf<ARobotCharacter> RobotCharacterClass = GetRobotCharacterClassFromID(
			GameInstance->RobotID[Team * 2 + PartNb], Pos);
		if (RobotCharacterClass == nullptr) return;

		ARobotCharacter* NewCharacter = GetWorld()->SpawnActorDeferred <ARobotCharacter>(
			RobotCharacterClass,
			SpawnPoint->GetTransform()
		);
	
		if (NewCharacter == nullptr) return;
		RobotParts.Add(Pos, NewCharacter);

		NewCharacter->InputData = InputData;
		switch (Pos)
		{
		case ERobotCharacterPositionEnum::Down:
			NewCharacter->InputMappingContext = InputMappingContextDown;
			break;
		case ERobotCharacterPositionEnum::Up:
			NewCharacter->InputMappingContext = InputMappingContextUp;
			break;
		default:
			return;
		}
		NewCharacter->AutoPossessPlayer = TEnumAsByte<EAutoReceiveInput::Type>(GameInstance->PlayersPos[Team * 2 + PartNb] + 1);
		NewCharacter->SetOrientX(SpawnPoint->GetStartOrientX());
		NewCharacter->FinishSpawning(SpawnPoint->GetTransform());

		//TeamLife += Character->Life;
		//TeamGuardMax += Character->Guard;
	}
	TeamGuard = TeamGuardMax;

	Cast<ARobotCharacterUp>(RobotParts[ERobotCharacterPositionEnum::Up])->AttachToLowerBody(
		RobotParts[ERobotCharacterPositionEnum::Down]->GetMesh(),
		Cast<ARobotCharacterDown>(RobotParts[ERobotCharacterPositionEnum::Down])->UpperBodySocketName);
}

TSubclassOf<ARobotCharacter> ATeamManager::GetRobotCharacterClassFromID(ERobotID ID, ERobotCharacterPositionEnum Pos) const 
{
	const UArenaSettings* ArenaSettings = GetDefault<UArenaSettings>();
	switch (Pos)
	{
		case ERobotCharacterPositionEnum::Down:
			return ArenaSettings->RobotCharacterDownClass[ID];
		case ERobotCharacterPositionEnum::Up:
			return ArenaSettings->RobotCharacterUpClass[ID];
		default: ;
	}
	return nullptr;
}

FVector ATeamManager::GetOpponentLocation()
{
	return Opponent->GetTeamLocation();
}

FVector ATeamManager::GetTeamLocation()
{
	return RobotParts[ERobotCharacterPositionEnum::Down]->GetActorLocation();
}

void ATeamManager::TeamTakeDamage(float Damage, float StunTime)
{
	if (IsGuarding && TeamGuard > 0) return;
	//RobotParts[ERobotCharacterPositionEnum::Up]->
	//RobotParts[ERobotCharacterPositionEnum::Down]->
	TeamGuard = TeamGuardMax;
}

void ATeamManager::TeamPartLock(ERobotCharacterPositionEnum Position)
{
	switch (Position)
	{
	case ERobotCharacterPositionEnum::Down:
		//RobotParts[ERobotCharacterPositionEnum::Up]
		break;
	case ERobotCharacterPositionEnum::Up:
		//RobotParts[ERobotCharacterPositionEnum::Down]
		break;
	default: ;
	}
}

void ATeamManager::TeamPartUnlock(ERobotCharacterPositionEnum Position)
{
	switch (Position)
	{
		case ERobotCharacterPositionEnum::Down:
			//RobotParts[ERobotCharacterPositionEnum::Up]
			break;
		case ERobotCharacterPositionEnum::Up:
			//RobotParts[ERobotCharacterPositionEnum::Down]
			break;
		default: ;
	}
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
