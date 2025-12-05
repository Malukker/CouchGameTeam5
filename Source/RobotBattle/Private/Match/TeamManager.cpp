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

	if (ComboTimer > 0) ComboTimer -= DeltaTime;
	if (ComboTimer <= 0 && Combo > 0) {Combo = 0; UIInterface->SetComboHit(Team, Combo);}

	if (BoostTimer > 0) BoostTimer -= DeltaTime;
	if (BoostTimer <= 0 && WantBoost) {WantBoost = false; RobotParts[ERobotCharacterPositionEnum::Up]->SetBoost(false);}
		
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
		NewCharacter->AttackManagerEvent.AddDynamic(this, &ATeamManager::TeamDoAttack);
		NewCharacter->AirStopManagerEvent.AddDynamic(this, &ATeamManager::TeamAirBlock);
		NewCharacter->AttackDuoManagerEvent.AddDynamic(this, &ATeamManager::AttackDuo);
		NewCharacter->BoostManagerEvent.AddDynamic(this, &ATeamManager::Boost);
		NewCharacter->KnockBackEvent.AddDynamic(this, &ATeamManager::KnockBack);
		NewCharacter->GuardManagerEvent.AddDynamic(this, &ATeamManager::Guard);
		NewCharacter->GuardResetManagerEvent.AddDynamic(this, &ATeamManager::GuardReset);
		NewCharacter->ChargeManagerEvent.AddDynamic(this, &ATeamManager::Charge);
		NewCharacter->InputDashManagerEvent.AddDynamic(this, &ATeamManager::DashInvinsibility);
		NewCharacter->SetTeam(Team);
		NewCharacter->AutoPossessPlayer = TEnumAsByte<EAutoReceiveInput::Type>(GameInstance->PlayersPos[Team * 2 + PartNb] + 1);
		NewCharacter->SetOrientX(SpawnPoint->GetStartOrientX());
		NewCharacter->GetCapsuleComponent()->SetCollisionObjectType(TeamCollision);
		NewCharacter->GetCapsuleComponent()->SetCollisionResponseToChannel(OpponentCollision, ECollisionResponse::ECR_Block);
		NewCharacter->GetCollision()->SetCollisionResponseToChannel(AttackChannel, ECollisionResponse::ECR_Block);
		NewCharacter->FinishSpawning(SpawnPoint->GetTransform());
		PlayersController.Add(Pos, Cast<APlayerController>(NewCharacter->GetController()));

		TeamLifeMax += NewCharacter->GetLife();
		TeamGuardMax += NewCharacter->GetGuard();
		TeamChargeMax += NewCharacter->GetCharge();
		InvinsibilityFramesOrigin += NewCharacter->GetInvinsibilityFrames();
	}
}

void ATeamManager::ResetCharacters()
{
	RobotParts[ERobotCharacterPositionEnum::Down]->SetActorLocation(SpawnPoint->GetTransform().GetLocation());
	RobotParts[ERobotCharacterPositionEnum::Up]->AttachToComponent(
	RobotParts[ERobotCharacterPositionEnum::Down]->GetMesh(),
		FAttachmentTransformRules(
			EAttachmentRule::SnapToTarget,
			EAttachmentRule::SnapToTarget,
			EAttachmentRule::KeepWorld,
			false),
		"Bones_Attach");

	TeamGuard = TeamGuardMax;
	TeamLife = TeamLifeMax;
	TeamCharge = 0;
	Combo = 0;
	UIInterface->SetHealthPlayer(Team, TeamLife, TeamLifeMax);
	UIInterface->SetChargePlayer(Team, TeamCharge, TeamChargeMax);
	UIInterface->SetComboHit(Team, Combo);
	RobotParts[ERobotCharacterPositionEnum::Up]->CustomTimeDilation = 1.f;
	RobotParts[ERobotCharacterPositionEnum::Up]->ResetStateMachine();
	RobotParts[ERobotCharacterPositionEnum::Down]->ResetStateMachine();
}

void ATeamManager::SetInput(bool Value)
{
	if (Value)
	{
		PlayersController[ERobotCharacterPositionEnum::Down]->Possess(RobotParts[ERobotCharacterPositionEnum::Down]);
		PlayersController[ERobotCharacterPositionEnum::Up]->Possess(RobotParts[ERobotCharacterPositionEnum::Up]);
	}
	else
	{
		PlayersController[ERobotCharacterPositionEnum::Down]->UnPossess();
		PlayersController[ERobotCharacterPositionEnum::Up]->UnPossess();
	}
}

void ATeamManager::PlayIntro()
{
	RobotParts[ERobotCharacterPositionEnum::Down]->PlayIntro();
	RobotParts[ERobotCharacterPositionEnum::Up]->PlayIntro();
}

void ATeamManager::InversePlayer()
{
	APlayerController* PlayerController = PlayersController[ERobotCharacterPositionEnum::Down];
	PlayersController[ERobotCharacterPositionEnum::Down] = PlayersController[ERobotCharacterPositionEnum::Up];
	PlayersController[ERobotCharacterPositionEnum::Up] = PlayerController;
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

float ATeamManager::GetLifePercent()
{
	return TeamLife / TeamLifeMax;
}

FVector ATeamManager::GetTeamLocation()
{
	return RobotParts[ERobotCharacterPositionEnum::Down]->GetActorLocation();
}

void ATeamManager::TeamDoAttack(bool HasTouch)
{
	if (HasTouch)
	{
		Combo++;
		ComboTimer = ComboResetTime;
	}else
	{
		Combo = 0;
	}
	UIInterface->SetComboHit(Team, Combo);
}

void ATeamManager::KnockBack(FVector2D KnockBackVelocity)
{
	FVector LaunchVelocity(KnockBackVelocity.X,0.f,KnockBackVelocity.Y);
	UCharacterMovementComponent* MovementComponent =RobotParts[ERobotCharacterPositionEnum::Down]->GetCharacterMovement();
	if (GetOpponentLocation().X - GetTeamLocation().X > 0)
	{
		LaunchVelocity.X*=-1;
	}
	MovementComponent->Launch(LaunchVelocity * KnockBackMultiplier);
}

void ATeamManager::TeamTakeDamage(int Damage, float StunTime)
{
	if (GetLifePercent() <= 0) return;	
	if (CanGuard && TeamGuard > 0)
	{
		if (RobotParts[ERobotCharacterPositionEnum::Down]->GetRobotCharacterDownChargeID() == ERobotCharacterDownChargeID::Tank)
		{
			Charge(ERobotCharacterPositionEnum::Down);
		}
		TeamGuard--;
		RobotParts[ERobotCharacterPositionEnum::Up]->GuardEvent.Broadcast(TeamGuardMax, TeamGuard);
		RobotParts[ERobotCharacterPositionEnum::Down]->GuardEvent.Broadcast(TeamGuardMax, TeamGuard);
		if (TeamGuard == 0)
		{
			const URobotCharacterSettings* Settings = GetDefault<URobotCharacterSettings>();
			USoundBase* GuardBreakSound =Settings->GuardBreak.LoadSynchronous();
			RobotParts[ERobotCharacterPositionEnum::Up]->SetStunTimer(StunDuration);
			RobotParts[ERobotCharacterPositionEnum::Down]->SetStunTimer(StunDuration);
			RobotParts[ERobotCharacterPositionEnum::Up]->HurtEvent.Broadcast();
			RobotParts[ERobotCharacterPositionEnum::Down]->HurtEvent.Broadcast();
			UGameplayStatics::PlaySoundAtLocation(GetWorld(),GuardBreakSound,GetActorLocation());
		}
		KnockBackMultiplier = .5f;
	}
	else if (CanTakeDamage)
	{
		RobotParts[ERobotCharacterPositionEnum::Up]->SetStunTimer(StunTime);
		RobotParts[ERobotCharacterPositionEnum::Down]->SetStunTimer(StunTime);
		RobotParts[ERobotCharacterPositionEnum::Up]->HurtEvent.Broadcast();
		RobotParts[ERobotCharacterPositionEnum::Down]->HurtEvent.Broadcast();
		TeamLife -= Damage;
		Combo = 0;
		KnockBackMultiplier = 1;
		
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
	else
	{
		KnockBackMultiplier = 0;
	}
}

void ATeamManager::TeamAirBlock()
{
	UCharacterMovementComponent* MovementComponent = RobotParts[ERobotCharacterPositionEnum::Down]->GetCharacterMovement();
	OriginalGravityScale = MovementComponent->GravityScale;
	MovementComponent->GravityScale = 0;
	MovementComponent->StopMovementImmediately();
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [&]()
	{
		if ( RobotParts[ERobotCharacterPositionEnum::Down] != nullptr)
		{
			RobotParts[ERobotCharacterPositionEnum::Down]->GetCharacterMovement()->GravityScale = OriginalGravityScale;
		}
	}, .1f, false);
			
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
		if (IsLoadingUltimate)
		{
			RobotParts[ERobotCharacterPositionEnum::Up]->AddAttackDuoBonus();
		}
		else
		{
			if (WantUltimate == 1) IsLoadingUltimate = RobotParts[ERobotCharacterPositionEnum::Up]->StartAttackDuo();
			UltimateBuffer = 0.33f;
			WantUltimate = 0;
		}
		break;
	case ERobotCharacterPositionEnum::Up:
		if (IsLoadingUltimate)
		{
			RobotParts[ERobotCharacterPositionEnum::Up]->AddAttackDuoBonus();
		}
		else
		{
			if (WantUltimate == 0) IsLoadingUltimate = RobotParts[ERobotCharacterPositionEnum::Up]->StartAttackDuo();
			UltimateBuffer = 0.33f;
			WantUltimate = 1;
		}
		break;
	case ERobotCharacterPositionEnum::None:
		IsLoadingUltimate = false;
	default: ;
	}
}

void ATeamManager::Boost()
{
	RobotParts[ERobotCharacterPositionEnum::Up]->SetBoost(true);
	BoostTimer = .25f;
	WantBoost = true;
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

