// Fill out your copyright notice in the Description page of Project Settings.


#include "Match/TeamManager.h"

#include "Characters/RobotCharacter.h"
#include "Arena/ArenaPlayerStart.h"
#include "Arena/ArenaSettings.h"
#include <Characters/RobotCharacterSettings.h>
#include <Characters/RobotCharacterInputData.h>
#include "LocalMultiplayerSubsystem.h"
#include "InputMappingContext.h"
#include "Components/CapsuleComponent.h"
#include "Match/RobotGameInstance.h"
#include "UI/UIGamePlayInterface.h"


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
	
	if (DashBuffer > 0) DashBuffer -= DeltaTime;
	if (DashBuffer < 0 && WantInvinsibility) WantInvinsibility = false;
	
	if (InvinsibilityFrames > 0) InvinsibilityFrames --;
	if (InvinsibilityFrames == 0 && !CanTakeDamage) CanTakeDamage = true;
		
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
	UInputMappingContext* InputMappingContextMenu = LoadInputMappingContextFromConfig(ERobotCharacterPositionEnum::None);

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

		NewCharacter->SetRobotBodyID(GameInstance->RobotID[Team * 2 + PartNb]);
		NewCharacter->InputData = InputData;
		NewCharacter->InputMappingContextMenu = InputMappingContextMenu;
		switch (Pos)
		{
		case ERobotCharacterPositionEnum::Down:
			NewCharacter->InputMappingContextGameplay = InputMappingContextDown;
			break;
		case ERobotCharacterPositionEnum::Up:
			NewCharacter->InputMappingContextGameplay = InputMappingContextUp;
			break;
		default:
			return;
		}
		NewCharacter->HurtManagerEvent.AddDynamic(this, &ATeamManager::TeamTakeDamage);
		NewCharacter->LockManagerEvent.AddDynamic(this, &ATeamManager::TeamPartLock);
		NewCharacter->GuardManagerEvent.AddDynamic(this, &ATeamManager::Guard);
		NewCharacter->GuardResetManagerEvent.AddDynamic(this, &ATeamManager::GuardReset);
		NewCharacter->ChargeManagerEvent.AddDynamic(this, &ATeamManager::Charge);
		NewCharacter->AttackDuoManagerEvent.AddDynamic(this, &ATeamManager::AttackDuo);
		NewCharacter->InputDashManagerEvent.AddDynamic(this, &ATeamManager::DashInvinsibility);
		NewCharacter->Team = Team;
		NewCharacter->AutoPossessPlayer = TEnumAsByte<EAutoReceiveInput::Type>(GameInstance->PlayersPos[Team * 2 + PartNb] + 1);
		NewCharacter->SetOrientX(SpawnPoint->GetStartOrientX());
		NewCharacter->GetCapsuleComponent()->SetCollisionObjectType(TeamCollision);
		NewCharacter->GetCapsuleComponent()->SetCollisionResponseToChannel(OpponentCollision, ECollisionResponse::ECR_Block);
		NewCharacter->GetCapsuleComponent()->SetCollisionResponseToChannel(AttackChannel, ECollisionResponse::ECR_Block);
		NewCharacter->FinishSpawning(SpawnPoint->GetTransform());

		TeamLifeMax += NewCharacter->Life;
		TeamGuardMax += NewCharacter->Guard;
		TeamChargeMax += NewCharacter->Charge;
		InvinsibilityFramesOrigin += NewCharacter->InvinsibilityFrames;
	}
	TeamGuard = TeamGuardMax;
	TeamLife = TeamLifeMax;
	TeamCharge = 0;
	UIInterface->SetHealthPlayer(Team, TeamLife, TeamLifeMax);
	UIInterface->SetChargePlayer(Team, TeamCharge, TeamChargeMax);
	
	RobotParts[ERobotCharacterPositionEnum::Up]->AttachToComponent(
	RobotParts[ERobotCharacterPositionEnum::Down]->GetMesh(),
		FAttachmentTransformRules
		(
		EAttachmentRule::SnapToTarget,
		EAttachmentRule::SnapToTarget,
		EAttachmentRule::SnapToTarget,
		true
		),
		"Bones_Attach");
}

void ATeamManager::ResetCharacters()
{
	RobotParts[ERobotCharacterPositionEnum::Down]->SetActorLocation(SpawnPoint->GetTransform().GetLocation());
	TeamGuard = TeamGuardMax;
	TeamLife = TeamLifeMax;
	TeamCharge = 0;
	UIInterface->SetHealthPlayer(Team, TeamLife, TeamLifeMax);
	UIInterface->SetChargePlayer(Team, TeamCharge, TeamChargeMax);
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

void ATeamManager::TeamTakeDamage(int Damage, float StunTime)
{
	if (CanGuard && TeamGuard > 0)
	{
		if (RobotParts[ERobotCharacterPositionEnum::Down]->GetRobotCharacterDownChargeID() == ERobotCharacterDownChargeID::None) TeamCharge++;
		TeamGuard--;
	}
	else if (CanTakeDamage)
	{
		RobotParts[ERobotCharacterPositionEnum::Up]->SetStunTimer(StunTime);
		RobotParts[ERobotCharacterPositionEnum::Down]->SetStunTimer(StunTime);
		RobotParts[ERobotCharacterPositionEnum::Up]->HurtEvent.Broadcast();
		RobotParts[ERobotCharacterPositionEnum::Down]->HurtEvent.Broadcast();
		TeamLife -= Damage;
		if (TeamLife < 0)
		{
			TeamLife = 0;
			DeathEvent.Broadcast(Team);
		}
		UIInterface->SetHealthPlayer(Team, TeamLife, TeamLifeMax);
	}
}

void ATeamManager::TeamPartLock(ERobotCharacterPositionEnum Position, bool Lock)
{
	switch (Position)
	{
	case ERobotCharacterPositionEnum::Down:
		if (Lock)
		{
			RobotParts[ERobotCharacterPositionEnum::Down]->LockEvent.Broadcast();
		}
		else
		{
			RobotParts[ERobotCharacterPositionEnum::Down]->UnlockEvent.Broadcast();
		}
		break;
	case ERobotCharacterPositionEnum::Up:
		if (Lock)
		{
			RobotParts[ERobotCharacterPositionEnum::Up]->LockEvent.Broadcast();
		}
		else
		{
			RobotParts[ERobotCharacterPositionEnum::Up]->UnlockEvent.Broadcast();
		}
		break;
	default: ;
	}
}

void ATeamManager::GuardReset()
{
	TeamGuard = TeamGuardMax;
}

void ATeamManager::Guard(bool Guard)
{
	CanGuard = Guard;
}

void ATeamManager::DashInvinsibility(ERobotCharacterPositionEnum Position)
{
	switch (Position)
	{
	case ERobotCharacterPositionEnum::Down:
		if (IsDashing)
		{
			IsDashing = false;
			InvinsibilityFrames = 0;
		}
		else
		{
			IsDashing = true;
			CanTakeDamage = false;
			InvinsibilityFrames = InvinsibilityFramesOrigin;
			if (WantInvinsibility) InvinsibilityFrames += InvinsibilityFramesOrigin;
		}
		break;
	case ERobotCharacterPositionEnum::Up:
		if (IsDashing)
		{
			InvinsibilityFrames += InvinsibilityFramesOrigin;
		}
		else
		{
			DashBuffer = 0.33f;
			WantInvinsibility = true;
		}
		break;
	default: ;
	}
}

void ATeamManager::Charge(ERobotCharacterPositionEnum Position)
{
	switch (Position)
	{
	case ERobotCharacterPositionEnum::Down:
		TeamCharge++;
		if (TeamCharge > TeamChargeMax) TeamCharge = TeamChargeMax;
		if (TeamCharge == TeamChargeMax)
		{
			RobotParts[ERobotCharacterPositionEnum::Up]->ManageChargeEvent(true);
		}
		break;
	case ERobotCharacterPositionEnum::Up:
		TeamCharge = 0;
		RobotParts[ERobotCharacterPositionEnum::Up]->ManageChargeEvent(false);
		RobotParts[ERobotCharacterPositionEnum::Down]->ManageChargeEvent(true);
		break;
	default: ;
	}
	UIInterface->SetChargePlayer(Team, TeamCharge, TeamChargeMax);
}

void ATeamManager::AttackDuo(ERobotCharacterPositionEnum Position)
{
	switch (Position)
	{
	case ERobotCharacterPositionEnum::Down:
		RobotParts[ERobotCharacterPositionEnum::Up]->AddDamageBonus();
		break;
	case ERobotCharacterPositionEnum::Up:
		RobotParts[ERobotCharacterPositionEnum::Down]->ManageChargeEvent(false);
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
		return CharacterSettings->InputMappingContextMenu.LoadSynchronous();
	}
	return nullptr;
}

