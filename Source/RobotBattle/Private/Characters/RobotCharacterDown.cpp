// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/RobotCharacterDown.h"
#include "Characters/RobotCharacterInputData.h"
#include "EnhancedInputComponent.h"
#include "Characters/RobotCharacterPositionEnum.h"
#include "Characters/RobotCharacterSettings.h"
#include "Kismet/GameplayStatics.h"


// Sets default values
ARobotCharacterDown::ARobotCharacterDown()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void ARobotCharacterDown::BeginPlay()
{
	Super::BeginPlay();
	
}

void ARobotCharacterDown::OnInputMoveX(const FInputActionValue& InputActionValue)
{
	if (UGameplayStatics::IsGamePaused(GetWorld())) return;
	InputMoveX = InputActionValue.Get<float>();
}

void ARobotCharacterDown::OnInputJump(const FInputActionValue& InputActionValue)
{
	if (UGameplayStatics::IsGamePaused(GetWorld())) return;
	InputJumpEvent.Broadcast();
}

void ARobotCharacterDown::OnInputAttackDuo(const FInputActionValue& InputActionValue)
{
	if (UGameplayStatics::IsGamePaused(GetWorld())) return;
	AttackDuoManagerEvent.Broadcast(ERobotCharacterPositionEnum::Down);
}

void ARobotCharacterDown::OnInputRightDash(const FInputActionValue& InputActionValue)
{
	if (UGameplayStatics::IsGamePaused(GetWorld())) return;
	if (!CanDash) return;
	DashDirectionX = 1;
	InputDashEvent.Broadcast();
}

void ARobotCharacterDown::OnInputLeftDash(const FInputActionValue& InputActionValue)
{
	if (UGameplayStatics::IsGamePaused(GetWorld())) return;
	if (!CanDash) return;
	DashDirectionX = -1;
	InputDashEvent.Broadcast();
}

void ARobotCharacterDown::BindInputAndActions(UEnhancedInputComponent* EnhancedInputComponent)
{
	Super::BindInputAndActions(EnhancedInputComponent);
	

	if (InputDataGameplay->InputActionJump)
	{
		EnhancedInputComponent->BindAction(InputDataGameplay->InputActionJump,ETriggerEvent::Started,this,&ARobotCharacterDown::OnInputJump);
	}
}

ERobotCharacterPositionEnum ARobotCharacterDown::GetPositionEnum()
{
	return ERobotCharacterPositionEnum::Down;
}

ERobotCharacterDownChargeID ARobotCharacterDown::GetRobotCharacterDownChargeID()
{
	return RobotCharacterDownChargeID;
}

void ARobotCharacterDown::DoGuardTest()
{
	if (FMath::Sign(GetOrientX()) != FMath::Sign(GetInputMoveX()))
	{
		if (WalkForward)
		{
			GuardManagerEvent.Broadcast(true);
			//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Blue, TEXT("Guard"));
		}
		WalkForward = false;
	}
	else
	{
		if (!WalkForward)
		{
			GuardManagerEvent.Broadcast(false);
			//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Blue, TEXT("No Guard"));
		}
		WalkForward = true;
	}
}


