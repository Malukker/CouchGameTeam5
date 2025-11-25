// Fill out your copyright notice in the Description page of Project Settings.


#include "Match/TeamManager.h"

#include "Characters/RobotCharacter.h"
#include "Arena/ArenaPlayerStart.h"
#include "Arena/ArenaSettings.h"
#include <Characters/RobotCharacterSettings.h>
#include <Characters/RobotCharacterInputData.h>
#include "LocalMultiplayerSubsystem.h"
#include "InputMappingContext.h"
#include "Components/BoxComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
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
	
	if (UltimateBuffer > 0) UltimateBuffer -= DeltaTime;
	if (UltimateBuffer < 0 && WantUltimate != -1) WantUltimate = -1;
		
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
		NewCharacter->InputDataGameplay = InputData;
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
		NewCharacter->LockManagerEvent.AddDynamic(this, &ATeamManager::TeamAirBlock);
		NewCharacter->EnergyManagerEvent.AddDynamic(this, &ATeamManager::SwitchEnergy);
		NewCharacter->GuardManagerEvent.AddDynamic(this, &ATeamManager::Guard);
		NewCharacter->GuardResetManagerEvent.AddDynamic(this, &ATeamManager::GuardReset);
		NewCharacter->ChargeManagerEvent.AddDynamic(this, &ATeamManager::Charge);
		NewCharacter->AttackDuoManagerEvent.AddDynamic(this, &ATeamManager::AttackDuo);
		NewCharacter->InputDashManagerEvent.AddDynamic(this, &ATeamManager::DashInvinsibility);
		NewCharacter->Team = Team;
		NewCharacter->AutoPossessPlayer = TEnumAsByte<EAutoReceiveInput::Type>(GameInstance->PlayersPos[Team * 2 + PartNb] + 1);
		NewCharacter->SetOrientX(SpawnPoint->GetStartOrientX());
		NewCharacter->GetCollision()->SetCollisionObjectType(TeamCollision);
		NewCharacter->GetCollision()->SetCollisionResponseToChannel(OpponentCollision, ECollisionResponse::ECR_Block);
		NewCharacter->GetCollision()->SetCollisionResponseToChannel(AttackChannel, ECollisionResponse::ECR_Block);
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
		FAttachmentTransformRules::SnapToTargetNotIncludingScale,
		"Bones_Attach");

	RobotParts[ERobotCharacterPositionEnum::Down]->SetEnergy(true);
}

void ATeamManager::ResetCharacters()
{
	RobotParts[ERobotCharacterPositionEnum::Down]->SetActorLocation(SpawnPoint->GetTransform().GetLocation());
	TeamGuard = TeamGuardMax;
	TeamLife = TeamLifeMax;
	TeamCharge = 0;
	UIInterface->SetHealthPlayer(Team, TeamLife, TeamLifeMax);
	UIInterface->SetChargePlayer(Team, TeamCharge, TeamChargeMax);
	RobotParts[ERobotCharacterPositionEnum::Up]->SetEnergy(false);
	RobotParts[ERobotCharacterPositionEnum::Down]->SetEnergy(true);
	RobotParts[ERobotCharacterPositionEnum::Up]->ResetStateMachine();
	RobotParts[ERobotCharacterPositionEnum::Down]->ResetStateMachine();
}

void ATeamManager::InversePlayer()
{
	AController* RobotPartOne = RobotParts[ERobotCharacterPositionEnum::Down]->GetController();
	AController* RobotPartTwo = RobotParts[ERobotCharacterPositionEnum::Up]->GetController();
	RobotPartOne->UnPossess();
	RobotPartTwo->UnPossess();
	RobotPartOne->Possess(RobotParts[ERobotCharacterPositionEnum::Up]);
	RobotPartTwo->Possess(RobotParts[ERobotCharacterPositionEnum::Down]);
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

float ATeamManager::GetLife()
{
	return TeamLife;
}

FVector ATeamManager::GetTeamLocation()
{
	return RobotParts[ERobotCharacterPositionEnum::Down]->GetActorLocation();
}

void ATeamManager::TeamTakeDamage(int Damage, float StunTime, FVector2D KnockBackVelocity)
{
	FVector LaunchVelocity(KnockBackVelocity.X,0.f,KnockBackVelocity.Y);
	UCharacterMovementComponent* MovementComponent =RobotParts[ERobotCharacterPositionEnum::Down]->GetCharacterMovement();
	if (GetOpponentLocation().X - GetTeamLocation().X > 0)
	{
		LaunchVelocity.X*=-1;
	}
	if (CanGuard && TeamGuard > 0)
	{
		if (RobotParts[ERobotCharacterPositionEnum::Down]->GetRobotCharacterDownChargeID() == ERobotCharacterDownChargeID::None) TeamCharge++;
		MovementComponent->Launch(LaunchVelocity/2);
		TeamGuard--;
	}
	else if (CanTakeDamage)
	{
		MovementComponent->Launch(LaunchVelocity);
		RobotParts[ERobotCharacterPositionEnum::Up]->SetStunTimer(StunTime);
		RobotParts[ERobotCharacterPositionEnum::Down]->SetStunTimer(StunTime);
		RobotParts[ERobotCharacterPositionEnum::Up]->HurtEvent.Broadcast();
		RobotParts[ERobotCharacterPositionEnum::Down]->HurtEvent.Broadcast();
		TeamLife -= Damage;
		Combo++;
		
		if (TeamLife <= 0)
		{
			TeamLife = 0;

			//Make a slowmotion during a certain delay
			const UArenaSettings* ArenaSettings = GetDefault<UArenaSettings>();
			UGameplayStatics::SetGlobalTimeDilation(GetWorld(), ArenaSettings->SlowMotionScale);
			FTimerHandle TimerHandle;
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, [&]()
			{
				UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1);
				DeathEvent.Broadcast(Team);
			}, ArenaSettings->SlowMotionDuration * ArenaSettings->SlowMotionScale, false);
			
		}
		UIInterface->SetHealthPlayer(Team, TeamLife, TeamLifeMax);
		UIInterface->SetComboHit(Team, Combo);
	}
}

void ATeamManager::TeamAirBlock(bool Lock)
{
	UCharacterMovementComponent* MovementComponent = RobotParts[ERobotCharacterPositionEnum::Down]->GetCharacterMovement();
	if (Lock)
	{
		OriginalGravityScale = MovementComponent->GravityScale;
		MovementComponent->GravityScale = 0;
		MovementComponent->StopMovementImmediately();
	}
	else if(OriginalGravityScale != -1)
	{
		MovementComponent->GravityScale = OriginalGravityScale;
		OriginalGravityScale = -1;
	}
}

void ATeamManager::SwitchEnergy()
{
	RobotParts[ERobotCharacterPositionEnum::Down]->SwitchEnergy();
	RobotParts[ERobotCharacterPositionEnum::Up]->SwitchEnergy();
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
			RobotParts[ERobotCharacterPositionEnum::Up]->SetCanAttackDuo(true);
			RobotParts[ERobotCharacterPositionEnum::Down]->SetCanAttackDuo(true);
		}
		break;
	case ERobotCharacterPositionEnum::Up:
		TeamCharge = 0;
		RobotParts[ERobotCharacterPositionEnum::Up]->SetCanAttackDuo(false);
		RobotParts[ERobotCharacterPositionEnum::Down]->SetCanAttackDuo(false);
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
		if (IsUltimating)
		{
			RobotParts[ERobotCharacterPositionEnum::Up]->AddDamageBonus();
		}
		else
		{
			if (WantUltimate == 1) IsUltimating = RobotParts[ERobotCharacterPositionEnum::Up]->StartAttackDuo();
			UltimateBuffer = 0.33f;
			WantUltimate = 0;
		}
		break;
	case ERobotCharacterPositionEnum::Up:
		if (IsUltimating)
		{
			RobotParts[ERobotCharacterPositionEnum::Up]->AddDamageBonus();
		}
		else
		{
			if (WantUltimate == 0) IsUltimating = RobotParts[ERobotCharacterPositionEnum::Up]->StartAttackDuo();
			UltimateBuffer = 0.33f;
			WantUltimate = 1;
		}
		break;
	case ERobotCharacterPositionEnum::None:
		IsUltimating = false;
	default: ;
	}
}

URobotCharacterInputData* ATeamManager::LoadInputDataFromConfig() {
	const URobotCharacterSettings* CharacterSettings = GetDefault<URobotCharacterSettings>();
	if (CharacterSettings == nullptr) return nullptr;
	return CharacterSettings->InputDataGameplay.LoadSynchronous();
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

