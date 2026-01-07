// Fill out your copyright notice in the Description page of Project Settings.


#include "Match/TeamManager.h"

#include "Characters/RobotCharacter.h"
#include "Arena/ArenaPlayerStart.h"
#include "Arena/ArenaSettings.h"
#include <Characters/RobotCharacterSettings.h>
#include <Characters/RobotCharacterInputData.h>
#include "LocalMultiplayerSubsystem.h"
#include "InputMappingContext.h"
#include "NiagaraFunctionLibrary.h"
#include "Characters/RobotControllerVibrationData.h"
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
	if (DashBuffer <= 0 && WantInvinsibility) WantInvinsibility = false;
	
	if (InvinsibilityFrames > 0) InvinsibilityFrames --;
	if (InvinsibilityFrames == 0 && !CanTakeDamage) CanTakeDamage = true;
	
	if (UltimateBuffer > 0) UltimateBuffer -= DeltaTime;
	if (UltimateBuffer <= 0 && WantUltimate != -1) WantUltimate = -1;

	if (ComboTimer > 0) ComboTimer -= DeltaTime;
	if (ComboTimer <= 0 && Combo > 0) {Combo = 0; UIInterface->SetComboHit(Team, Combo);}

	if (BoostTimer > 0) BoostTimer -= DeltaTime;
	if (BoostTimer <= 0 && WantBoost) {WantBoost = false; RobotParts[ERobotCharacterPositionEnum::Up]->SetBoost(false);}

	if(RobotParts[ERobotCharacterPositionEnum::Up]->GetLookingOpponent() &&
		RobotParts[ERobotCharacterPositionEnum::Down]->GetLookingOpponent())
	{
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
	TeamLifeMax = 0;
	TeamChargeMax = 0;

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
			SpawnPoint->GetTransform(),
			nullptr,
			nullptr,
			ESpawnActorCollisionHandlingMethod::AlwaysSpawn
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
		NewCharacter->AttackManagerEvent.AddDynamic(this, &ATeamManager::TeamDoAttack);
		NewCharacter->AirStopManagerEvent.AddDynamic(this, &ATeamManager::TeamAirBlock);
		NewCharacter->AttackDuoManagerEvent.AddDynamic(this, &ATeamManager::AttackDuo);
		NewCharacter->BoostManagerEvent.AddDynamic(this, &ATeamManager::Boost);
		NewCharacter->KnockBackEvent.AddDynamic(this, &ATeamManager::KnockBack);
		NewCharacter->GuardResetManagerEvent.AddDynamic(this, &ATeamManager::GuardReset);
		NewCharacter->GuardManagerEvent.AddDynamic(this, &ATeamManager::TeamDoGuard);
		NewCharacter->HurtManagerEvent.AddDynamic(this, &ATeamManager::TeamTakeDamage);
		NewCharacter->ChargeManagerEvent.AddDynamic(this, &ATeamManager::Charge);
		NewCharacter->InputDashManagerEvent.AddDynamic(this, &ATeamManager::DashInvinsibility);
		NewCharacter->VibrationControllerEvent.AddDynamic(this,&ATeamManager::StartControllerVibration);
		NewCharacter->SetTeam(Team);
		NewCharacter->SetLookingOpponent(true);
		NewCharacter->AutoPossessPlayer = TEnumAsByte<EAutoReceiveInput::Type>(GameInstance->PlayersPos[Team * 2 + PartNb] + 1);
		NewCharacter->SetOrientX(SpawnPoint->GetStartOrientX());
		NewCharacter->GetMyCapsuleComponent()->SetCollisionObjectType(TeamCollision);
		NewCharacter->GetMyCapsuleComponent()->SetCollisionResponseToChannel(OpponentCollision, ECollisionResponse::ECR_Block);
		NewCharacter->GetCollision()->SetCollisionResponseToChannel(AttackChannel, ECollisionResponse::ECR_Block);
		NewCharacter->FinishSpawning(SpawnPoint->GetTransform());
		PlayersController.Add(Pos, Cast<APlayerController>(NewCharacter->GetController()));

		TeamLifeMax += NewCharacter->GetLife();
		TeamGuardMax += NewCharacter->GetGuard();
		TeamChargeMax += NewCharacter->GetCharge();
		InvinsibilityFramesOrigin += NewCharacter->GetInvinsibilityFrames();
	}
	ResetCharactersLocation();
}

void ATeamManager::ResetCharacters()
{
	ResetCharactersLocation();
	RobotParts[ERobotCharacterPositionEnum::Up]->SetLookingOpponent(true);
	UCharacterMovementComponent* MovementComponent = RobotParts[ERobotCharacterPositionEnum::Up]->GetCharacterMovement();
	MovementComponent->StopMovementImmediately();
	MovementComponent->SetDefaultMovementMode();
	MovementComponent->Deactivate();
	MovementComponent = RobotParts[ERobotCharacterPositionEnum::Down]->GetCharacterMovement();
	MovementComponent->StopMovementImmediately();
	MovementComponent->SetDefaultMovementMode();
	GuardReset();
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

void ATeamManager::ResetCharactersLocation()
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
}

void ATeamManager::SetInput(bool Value)
{
	if (Value)
	{
		CanTakeDamage = true;
		PlayersController[ERobotCharacterPositionEnum::Down]->Possess(RobotParts[ERobotCharacterPositionEnum::Down]);
		PlayersController[ERobotCharacterPositionEnum::Up]->Possess(RobotParts[ERobotCharacterPositionEnum::Up]);
	}
	else
	{
		CanTakeDamage = false;
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
	return static_cast<float>(TeamLife) / TeamLifeMax;
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
	UCharacterMovementComponent* MovementComponent = RobotParts[ERobotCharacterPositionEnum::Down]->GetCharacterMovement();
	if (GetOpponentLocation().X - GetTeamLocation().X > 0)
	{
		LaunchVelocity.X*=-1;
	}
	MovementComponent->Launch(LaunchVelocity * KnockBackMultiplier);
}

void ATeamManager::TeamDoGuard(int Damage, float StunTime, float ChipDamage)
{
	if (GetLifePercent() == 0.f) return;	
	if (TeamGuard > 0)
	{
		if (RobotParts[ERobotCharacterPositionEnum::Down]->GetRobotCharacterDownChargeID() == ERobotCharacterDownChargeID::Tank)
		{
			Charge(ERobotCharacterPositionEnum::Down);
		}
		
		TeamLife -= Damage * ChipDamage;
		UIInterface->SetHealthPlayer(Team, TeamLife, TeamLifeMax);
		if (TeamLife <= 0)
		{
			Death();
			return;
		}
		
		TeamGuard--;
		RobotParts[ERobotCharacterPositionEnum::Up]->GuardEvent.Broadcast(TeamGuardMax, TeamGuard);
		RobotParts[ERobotCharacterPositionEnum::Down]->GuardEvent.Broadcast(TeamGuardMax, TeamGuard);
		if (TeamGuard == 0)
		{
			const URobotCharacterSettings* Settings = GetDefault<URobotCharacterSettings>();

			//Start Vibration for Guard Break
			URobotControllerVibrationData* VibrationData = LoadVibrationDataFromConfig();
			if (VibrationData && VibrationData->GuardBreakVibration)
			{
				for (const TPair<ERobotCharacterPositionEnum, TObjectPtr<APlayerController>>& Pair : PlayersController)
				{
					Pair.Value->ClientPlayForceFeedback(VibrationData->GuardBreakVibration);
				}
			}
			
			USoundBase* GuardBreakSound =Settings->GuardBreak.LoadSynchronous();
			RobotParts[ERobotCharacterPositionEnum::Up]->SetStunTimer(StunDuration);
			RobotParts[ERobotCharacterPositionEnum::Down]->SetStunTimer(StunDuration);
			RobotParts[ERobotCharacterPositionEnum::Up]->HurtEvent.Broadcast();
			RobotParts[ERobotCharacterPositionEnum::Down]->HurtEvent.Broadcast();
			UGameplayStatics::PlaySoundAtLocation(GetWorld(),GuardBreakSound,GetActorLocation());
		}
		KnockBackMultiplier = .5f;
	}
	else
	{
		TeamTakeDamage(Damage, StunTime);
	}
}

void ATeamManager::TeamTakeDamage(int Damage, float StunTime)
{
	if (GetLifePercent() == 0.f) return;	
	if (CanTakeDamage)
	{
		RobotParts[ERobotCharacterPositionEnum::Up]->SetStunTimer(StunTime);
		RobotParts[ERobotCharacterPositionEnum::Down]->SetStunTimer(StunTime);
		RobotParts[ERobotCharacterPositionEnum::Up]->HurtEvent.Broadcast();
		RobotParts[ERobotCharacterPositionEnum::Down]->HurtEvent.Broadcast();
		TeamLife -= (IsLoadingUltimate ? (Damage * .7f) : Damage);
		Combo = 0;
		KnockBackMultiplier = (IsLoadingUltimate ? .5f : 1);
		
		if (TeamLife <= 0)
		{
			Death();
		}
		UIInterface->SetHealthPlayer(Team, TeamLife, TeamLifeMax);
		UIInterface->SetComboHit(Team, Combo);
	}
	else
	{
		KnockBackMultiplier = 0;
	}
}

void ATeamManager::Death()
{
	TeamLife = 0;
	RobotParts[ERobotCharacterPositionEnum::Up]->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

	UCharacterMovementComponent* MovementComponent = RobotParts[ERobotCharacterPositionEnum::Up]->GetCharacterMovement();
	MovementComponent->Activate();
	FVector LaunchVelocity(750,0.f,750);
	if (GetOpponentLocation().X - GetTeamLocation().X > 0)
	{
		LaunchVelocity.X*=-1;
	}
	MovementComponent->Launch(LaunchVelocity);
	LaunchVelocity.Z = 200;
	RobotParts[ERobotCharacterPositionEnum::Down]->GetCharacterMovement()->Launch(LaunchVelocity);

	UNiagaraFunctionLibrary::SpawnSystemAttached(
		TeamDownDestroy,
		RobotParts[ERobotCharacterPositionEnum::Down]->GetMesh(),
		FName("Bones_Attach"),
		FVector::ZeroVector,
		FRotator(90,0,0),
		EAttachLocation::Type::SnapToTarget,
		true,
		true);
			
	UNiagaraFunctionLibrary::SpawnSystemAttached(
		TeamUpDestroy,
		RobotParts[ERobotCharacterPositionEnum::Up]->GetMesh(),
		FName("Bones_Attach"),
		FVector::ZeroVector,
		FRotator::ZeroRotator,
		EAttachLocation::Type::SnapToTarget,
		true,
		true);

	//Make a slowmotion during a certain delay
	const UArenaSettings* ArenaSettings = GetDefault<UArenaSettings>();
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(), ArenaSettings->SlowMotionScale);
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [&]()
	{
		RobotParts[ERobotCharacterPositionEnum::Up]->SetLookingOpponent(false);
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1);
		DeathEvent.Broadcast(Team);
	}, ArenaSettings->SlowMotionDuration * ArenaSettings->SlowMotionScale, false);
}

void ATeamManager::TeamAirBlock()
{
	UCharacterMovementComponent* MovementComponent = RobotParts[ERobotCharacterPositionEnum::Down]->GetCharacterMovement();
	MovementComponent->StopMovementImmediately();
			
}

void ATeamManager::GuardReset()
{
	TeamGuard = TeamGuardMax;
	RobotParts[ERobotCharacterPositionEnum::Up]->GuardEvent.Broadcast(TeamGuardMax, TeamGuard);
	RobotParts[ERobotCharacterPositionEnum::Down]->GuardEvent.Broadcast(TeamGuardMax, TeamGuard);
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
			if (WantInvinsibility)
			{
				InvinsibilityFrames += InvinsibilityFramesOrigin;
				RobotParts[ERobotCharacterPositionEnum::Up]->PlayDash();
				if (TeamBoost)
				{
					UNiagaraFunctionLibrary::SpawnSystemAttached(
						TeamBoost,
						RobotParts[ERobotCharacterPositionEnum::Up]->GetMesh(),
						NAME_None,
						FVector::ZeroVector,
						FRotator::ZeroRotator,
						EAttachLocation::Type::SnapToTarget,
						true,
						true);
				}
			}
		}
		break;
	case ERobotCharacterPositionEnum::Up:
		if (IsDashing)
		{
			InvinsibilityFrames += InvinsibilityFramesOrigin;
			RobotParts[ERobotCharacterPositionEnum::Up]->PlayDash();
			if (TeamBoost)
			{
				UNiagaraFunctionLibrary::SpawnSystemAttached(
					TeamBoost,
					RobotParts[ERobotCharacterPositionEnum::Up]->GetMesh(),
					NAME_None,
					FVector::ZeroVector,
					FRotator::ZeroRotator,
					EAttachLocation::Type::SnapToTarget,
					true,
					true);
			}
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
		if (IsLoadingUltimate) return;
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
			if (WantUltimate == 1)
			{
				IsLoadingUltimate = RobotParts[ERobotCharacterPositionEnum::Up]->StartAttackDuo();
			}
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
			if (WantUltimate == 0) 
			{
				CanTakeDamage = false;
				IsLoadingUltimate = RobotParts[ERobotCharacterPositionEnum::Up]->StartAttackDuo();
			}
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
	BoostTimer = .15f;
	WantBoost = true;
}


URobotCharacterInputData* ATeamManager::LoadInputDataFromConfig() {
	const URobotCharacterSettings* CharacterSettings = GetDefault<URobotCharacterSettings>();
	if (CharacterSettings == nullptr) return nullptr;
	return CharacterSettings->InputDataGameplay.LoadSynchronous();
}

URobotControllerVibrationData* ATeamManager::LoadVibrationDataFromConfig()
{
	const URobotCharacterSettings* CharacterSettings = GetDefault<URobotCharacterSettings>();
	if (CharacterSettings == nullptr) return nullptr;
	return CharacterSettings->ControllerVibrationData.LoadSynchronous();
}

void ATeamManager::StartControllerVibration(ERobotID RobotID, EAttackID AttackID)
{
	if (URobotGameInstance* GI = GetGameInstance<URobotGameInstance>())
	{
		if (!GI->bEnableControllerVibration)
		{
			UE_LOG(LogTemp, Log, TEXT("NO VIBRATION (disabled in settings)"));
			return;
		}
	}
	
	URobotControllerVibrationData* VibrationData = LoadVibrationDataFromConfig();
	if (VibrationData == nullptr
		|| !VibrationData->VibrationMap.Contains(RobotID)
		|| !VibrationData->VibrationMap[RobotID].RobotAttackType.Contains(AttackID)) return;
	
	if (VibrationData->VibrationMap[RobotID].RobotAttackType[AttackID])
	{
		for (const TPair<ERobotCharacterPositionEnum, TObjectPtr<APlayerController>>& Pair : PlayersController)
		{
			Pair.Value->ClientPlayForceFeedback(VibrationData->VibrationMap[RobotID].RobotAttackType[AttackID]);
		}
	}
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

