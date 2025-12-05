// Fill out your copyright notice in the Description page of Project Settings.


#include "Match/WinManager.h"

#include "Arena/ArenaSettings.h"
#include "Characters/RobotCharacter.h"
#include "Characters/RobotCharacterPositionEnum.h"
#include "GameFramework/PlayerStart.h"
#include "Kismet/GameplayStatics.h"
#include "Match/RobotGameInstance.h"


class ARobotCharacter;
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

	
	
	// 0 for Down and 1 for Up for Winner
	for (int PartNb = 0; PartNb < 2; PartNb++)
	{
		ERobotCharacterPositionEnum Pos = ERobotCharacterPositionEnum::None;
		if (PartNb == 0) Pos = ERobotCharacterPositionEnum::Down;
		else Pos = ERobotCharacterPositionEnum::Up;
		
		TSubclassOf<ARobotCharacter> RobotCharacterClass = GetRobotCharacterClassFromID(
			GameInstance->RobotID[GameInstance->TeamWin * 2 + PartNb], Pos);
		if (RobotCharacterClass == nullptr) return;

		ARobotCharacter* NewCharacter = GetWorld()->SpawnActorDeferred <ARobotCharacter>(
			RobotCharacterClass,
			WinnerSpawn->GetTransform()
		);
	
		if (NewCharacter == nullptr) return;
		RobotPartsWin.Add(Pos, NewCharacter);
		UGameplayStatics::FinishSpawningActor(NewCharacter, WinnerSpawn->GetTransform());
		
	}
	RobotPartsWin[ERobotCharacterPositionEnum::Up]->AttachToComponent(
	RobotPartsWin[ERobotCharacterPositionEnum::Down]->GetMesh(),
		FAttachmentTransformRules(
			EAttachmentRule::SnapToTarget,
			EAttachmentRule::SnapToTarget,
			EAttachmentRule::KeepWorld,
			false),
		"Bones_Attach");


	// 0 for Down and 1 for Up for looser
	for (int PartNb = 0; PartNb < 2; PartNb++)
	{
		ERobotCharacterPositionEnum Pos = ERobotCharacterPositionEnum::None;
		if (PartNb == 0) Pos = ERobotCharacterPositionEnum::Down;
		else Pos = ERobotCharacterPositionEnum::Up;
		
		TSubclassOf<ARobotCharacter> RobotCharacterClass = GetRobotCharacterClassFromID(
			GameInstance->RobotID[GameInstance->TeamLoose * 2 + PartNb], Pos);
		if (RobotCharacterClass == nullptr) return;

		ARobotCharacter* NewCharacter = GetWorld()->SpawnActorDeferred <ARobotCharacter>(
			RobotCharacterClass,
			LoserSpawn->GetTransform()
		);
	
		if (NewCharacter == nullptr) return;
		RobotPartsLose.Add(Pos, NewCharacter);
		UGameplayStatics::FinishSpawningActor(NewCharacter, LoserSpawn->GetTransform());
		
	}
	RobotPartsLose[ERobotCharacterPositionEnum::Up]->AttachToComponent(
	RobotPartsLose[ERobotCharacterPositionEnum::Down]->GetMesh(),
		FAttachmentTransformRules(
			EAttachmentRule::SnapToTarget,
			EAttachmentRule::SnapToTarget,
			EAttachmentRule::KeepWorld,
			false),
		"Bones_Attach");


	//RobotPartsLose[ERobotCharacterPositionEnum::Up]->SetActorRotation(FRotator::ZeroRotator);
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
