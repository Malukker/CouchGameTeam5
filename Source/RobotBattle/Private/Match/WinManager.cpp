// Fill out your copyright notice in the Description page of Project Settings.


#include "Match/WinManager.h"

#include "Arena/ArenaSettings.h"
#include "Characters/RobotCharacter.h"
#include "Characters/RobotCharacterPositionEnum.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"
#include "Match/RobotGameInstance.h"

// Sets default values
AWinManager::AWinManager()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AWinManager::BeginPlay()
{
	URobotGameInstance* GameInstance = GetWorld()->GetGameInstance<URobotGameInstance>();
	if (GameInstance == nullptr) return;
	
	SpawnRobot(GameInstance, &RobotPartsWin, WinnerSpawn, GameInstance->TeamWin);
	SpawnRobot(GameInstance, &RobotPartsLose, LoserSpawn, (GameInstance->TeamWin == 1 ? 0 : 1));

	RobotPartsWin[ERobotCharacterPositionEnum::Up]->PlayEnd(true);
	RobotPartsLose[ERobotCharacterPositionEnum::Up]->PlayEnd(false);
}


void AWinManager::SpawnRobot(URobotGameInstance* GameInstance,TMap<ERobotCharacterPositionEnum, TObjectPtr<ARobotCharacter>>* RobotParts, APlayerStart* SpawnPoint, int index)
{
	for (int PartNb = 0; PartNb < 2; PartNb++)
	{
		ERobotCharacterPositionEnum Pos = ERobotCharacterPositionEnum::None;
		if (PartNb == 0) Pos = ERobotCharacterPositionEnum::Down;
		else Pos = ERobotCharacterPositionEnum::Up;
		
		TSubclassOf<ARobotCharacter> RobotCharacterClass = GetRobotCharacterClassFromID(
			GameInstance->RobotID[index * 2 + PartNb], Pos);
		if (RobotCharacterClass == nullptr) return;

		ARobotCharacter* NewCharacter = GetWorld()->SpawnActorDeferred <ARobotCharacter>(
			RobotCharacterClass,
			SpawnPoint->GetTransform()
		);
	
		if (NewCharacter == nullptr) return;
		RobotParts->Add(Pos, NewCharacter);
		UGameplayStatics::FinishSpawningActor(NewCharacter, SpawnPoint->GetTransform());
	}
	RobotParts->Find(ERobotCharacterPositionEnum::Up)->Get()->AttachToComponent(
	RobotParts->Find(ERobotCharacterPositionEnum::Down)->Get()->GetMesh(),
		FAttachmentTransformRules(
			EAttachmentRule::SnapToTarget,
			EAttachmentRule::KeepWorld,
			EAttachmentRule::KeepWorld,
			false),
		"Bones_Attach");
	RobotParts->Find(ERobotCharacterPositionEnum::Down)->Get()->SetActorLocation(SpawnPoint->GetActorLocation());
}



TSubclassOf<ARobotCharacter> AWinManager::GetRobotCharacterClassFromID(ERobotID ID, ERobotCharacterPositionEnum Pos) const 
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
