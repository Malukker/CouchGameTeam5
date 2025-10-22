// Fill out your copyright notice in the Description page of Project Settings.


#include "Match/MatchGameMode.h"
#include "Characters/RobotCharacter.h"
#include "Arena/ArenaPlayerStart.h"
#include "Arena/ArenaSettings.h"
#include "Kismet/GameplayStatics.h"
#include "InputMappingContext.h"
#include "LocalMultiplayer/Public/LocalMultiplayerSubsystem.h"
#include <Characters/RobotCharacterSettings.h>
#include <Characters/RobotCharacterInputData.h>


void AMatchGameMode::BeginPlay() {
	Super::BeginPlay();
	CreateAndInitPlayers();

	TArray<AArenaPlayerStart*> PlayerStartsPoints;
	FindPlayerStartActorsInArena(PlayerStartsPoints);
	SpawnCharacters(PlayerStartsPoints);

	for (AArenaPlayerStart* PlayerStartPoint : PlayerStartsPoints)
	{
		EAutoReceiveInput::Type InputType = PlayerStartPoint->AutoReceiveInput.GetValue();
		TSubclassOf<ARobotCharacter> RobotCharacterClass = GetRobotCharacterClassFromInputType(InputType);
		if (RobotCharacterClass == nullptr) continue;

		GEngine->AddOnScreenDebugMessage(
			-1,
			3.f,
			FColor::Cyan,
			RobotCharacterClass->GetFName().ToString()
		);
	}
}

void AMatchGameMode::FindPlayerStartActorsInArena(TArray<AArenaPlayerStart*>& ResultsActors) {
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AArenaPlayerStart::StaticClass(), FoundActors);

	for (int i = 0; i < FoundActors.Num(); ++i) {
		AArenaPlayerStart* ArenaPlayerStartActor = Cast<AArenaPlayerStart>(FoundActors[i]);
		if (ArenaPlayerStartActor == nullptr) continue;

		ResultsActors.Add(ArenaPlayerStartActor);
	}
}
void 
AMatchGameMode::SpawnCharacters(const TArray<AArenaPlayerStart*> SpawnPoints)
{
	URobotCharacterInputData* InputData = LoadInputDataFromConfig();
	UInputMappingContext* InputMappingContextDown = LoadInputMappingContextFromConfig(ERobotCharacterPositionEnum::Down);
	UInputMappingContext* InputMappingContextUp = LoadInputMappingContextFromConfig(ERobotCharacterPositionEnum::Up);

	for (AArenaPlayerStart* SpawnPoint : SpawnPoints)
	{
		EAutoReceiveInput::Type InputType = SpawnPoint->AutoReceiveInput.GetValue();
		TSubclassOf<ARobotCharacter> RobotCharacterClass = GetRobotCharacterClassFromInputType(InputType);
		if (RobotCharacterClass == nullptr) continue;


		ARobotCharacter* NewCharacter = GetWorld()->SpawnActorDeferred <ARobotCharacter>(
			RobotCharacterClass,
			SpawnPoint->GetTransform()
		);

		if (NewCharacter == nullptr)continue;
		NewCharacter->InputData = InputData;
		switch (NewCharacter->GetPositionEnum())
		{
		case ERobotCharacterPositionEnum::Down:
			NewCharacter->InputMappingContext = InputMappingContextDown;
			break;
		case ERobotCharacterPositionEnum::Up:
			NewCharacter->InputMappingContext = InputMappingContextUp;
			break;
		default:
			continue;
		}
		NewCharacter->AutoPossessPlayer = SpawnPoint->AutoReceiveInput;
		NewCharacter->SetOrientX(SpawnPoint->GetStartOrientX());
		NewCharacter->FinishSpawning(SpawnPoint->GetTransform());
	}
}

TSubclassOf<ARobotCharacter> AMatchGameMode::GetRobotCharacterClassFromInputType(
	EAutoReceiveInput::Type InputType) const 
{
	const UArenaSettings* ArenaSettings = GetDefault<UArenaSettings>();
	switch (InputType) {
	case EAutoReceiveInput::Player0:
		return ArenaSettings->RobotCharacterClassP0;

	case EAutoReceiveInput::Player1:
		return ArenaSettings->RobotCharacterClassP1;

	case EAutoReceiveInput::Player2:
		return  ArenaSettings->RobotCharacterClassP2;

	case EAutoReceiveInput::Player3:
		return  ArenaSettings->RobotCharacterClassP3;

	default:
		return nullptr;
	}
}

URobotCharacterInputData* AMatchGameMode::LoadInputDataFromConfig() {
	const URobotCharacterSettings* CharacterSettings = GetDefault<URobotCharacterSettings>();
	if (CharacterSettings == nullptr) return nullptr;
	return CharacterSettings->InputData.LoadSynchronous();
}

UInputMappingContext* AMatchGameMode::LoadInputMappingContextFromConfig(ERobotCharacterPositionEnum Position) {
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

void AMatchGameMode::CreateAndInitPlayers() const{
	UGameInstance* GameInstance = GetWorld()->GetGameInstance();
	if (GameInstance == nullptr) return;

	ULocalMultiplayerSubsystem* LocalMultiplayerSubsystem = GameInstance->GetSubsystem<ULocalMultiplayerSubsystem>();
	if (LocalMultiplayerSubsystem == nullptr) return;

	LocalMultiplayerSubsystem->CreatePlayers();
}