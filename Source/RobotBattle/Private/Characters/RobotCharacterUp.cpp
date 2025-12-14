// Fill out your copyright notice in the Description page of Project Settings.

#include "Characters/RobotCharacterUp.h"

#include "EnhancedInputComponent.h"
#include "MathUtil.h"
#include "Arena/ArenaSettings.h"
#include "Characters/RobotCharacterInputData.h"
#include "Characters/RobotCharacterPositionEnum.h"
#include "Characters/RobotCharacterStateID.h"
#include "Characters/RobotCharacterStateMachine.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ARobotCharacterUp::ARobotCharacterUp()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MyCapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(FName("MyCapsuleComponent"));
	MyCapsuleComponent->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
}

void ARobotCharacterUp::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (IsLookingOpponent)
	{float PosActual = GetActorLocation().X;
		if (FMathf::Abs(PosActual - PosSpring) > MinAngle)
		{
			PosSpring = FMathf::Clamp( PosSpring, PosActual - MinAngle, PosActual + MinAngle);
		}
	
		int Direction = FMathf::SignAsInt(PosActual - PosSpring);
		VelocitySpring += Direction * DeltaSeconds * Acceleration;
		if (VelocitySpring > MaxSpeed)
		{
			VelocitySpring = FMathf::SignAsInt(VelocitySpring) * MaxSpeed;
		}
		VelocitySpring = FMath::Lerp(
			VelocitySpring,
			0,
			 FMathf::Clamp(DeltaSeconds * Damping, 0.0f, 1.0f)
			);
		PosSpring += VelocitySpring * DeltaSeconds;

		FVector NewLocation = GetActorLocation();
		NewLocation.X = PosSpring;
		NewLocation.Z += 50;
		FVector Dir = GetActorLocation() - NewLocation;
		Dir.Normalize();
		if (FMathf::Abs(Dir.X ) < .05f)
		{
			SetActorRotation( FQuat::Identity);
			return;
		}

		// Convert the angle to degrees
		float AngleInDegrees = FMath::RadiansToDegrees(FMathf::Atan2(Dir.Z, Dir.X));
		
		FRotator NewRotation = FRotator(AngleInDegrees + 90, 0, 0);
	
		SetActorRotation( FQuat(NewRotation));
	}
}

UCapsuleComponent* ARobotCharacterUp::GetMyCapsuleComponent()
{
	return MyCapsuleComponent;
}

// Called when the game starts or when spawn

void ARobotCharacterUp::BindInputAndActions(UEnhancedInputComponent* EnhancedInputComponent)
{
	Super::BindInputAndActions(EnhancedInputComponent);
#pragma region AttackBindings
	if (InputDataGameplay->InputActionAttack1)
	{
		EnhancedInputComponent->BindAction(InputDataGameplay->InputActionAttack1, ETriggerEvent::Started, this,
		                                   &ARobotCharacterUp::OnInputAttack1);
	}

	if (InputDataGameplay->InputActionAttack2)
	{
		EnhancedInputComponent->BindAction(InputDataGameplay->InputActionAttack2, ETriggerEvent::Started, this,
		                                   &ARobotCharacterUp::OnInputAttack2);
	}

	if (InputDataGameplay->InputActionAttack3)
	{
		EnhancedInputComponent->BindAction(InputDataGameplay->InputActionAttack3, ETriggerEvent::Started, this,
		                                   &ARobotCharacterUp::OnInputAttack3);
	}

#pragma endregion
}

void ARobotCharacterUp::BeginPlay()
{
	Super::BeginPlay();
	HitStopEvent.AddDynamic(this,&ARobotCharacterUp::OnHitStop);
}

#pragma region Attacks
void ARobotCharacterUp::OnInputAttack1(const FInputActionValue& InputActionValue)
{
	if (UGameplayStatics::IsGamePaused(GetWorld())) return;
	if (StateMachine->GetCurrentStateID() == ERobotCharacterStateID::Attack
		|| StateMachine->GetCurrentStateID() == ERobotCharacterStateID::LoadingAttack) return;
	CurrentTypeAttack = EAttackID::Type1;
	InputAttackEvent.Broadcast();
}

void ARobotCharacterUp::OnInputAttack2(const FInputActionValue& InputActionValue)
{
	if (UGameplayStatics::IsGamePaused(GetWorld())) return;
	if (StateMachine->GetCurrentStateID() == ERobotCharacterStateID::Attack
		|| StateMachine->GetCurrentStateID() == ERobotCharacterStateID::LoadingAttack) return;
	CurrentTypeAttack = EAttackID::Type2;
	InputAttackEvent.Broadcast();
}

void ARobotCharacterUp::OnInputAttack3(const FInputActionValue& InputActionValue)
{
	if (UGameplayStatics::IsGamePaused(GetWorld())) return;
	if (StateMachine->GetCurrentStateID() == ERobotCharacterStateID::Attack
		|| StateMachine->GetCurrentStateID() == ERobotCharacterStateID::LoadingAttack) return;
	CurrentTypeAttack = EAttackID::Type3;
	InputAttackEvent.Broadcast();
}



void ARobotCharacterUp::OnInputAttackDuo(const FInputActionValue& InputActionValue)
{
	if (UGameplayStatics::IsGamePaused(GetWorld())) return;
	AttackDuoManagerEvent.Broadcast(ERobotCharacterPositionEnum::Up);
}
#pragma endregion

bool ARobotCharacterUp::StartAttackDuo()
{
	if (!CanAttackDuo) return false;
	if (StateMachine->GetCurrentStateID() == ERobotCharacterStateID::Attack
		|| StateMachine->GetCurrentStateID() == ERobotCharacterStateID::LoadingAttack
		|| StateMachine->GetCurrentStateID() == ERobotCharacterStateID::Stun) return false;
	CurrentTypeAttack = EAttackID::Ultimate;
	InputAttackEvent.Broadcast();
	return true;
}

void ARobotCharacterUp::PlayDash()
{
	PlayAnimMontage(DashAnimMontage);
}

ERobotCharacterPositionEnum ARobotCharacterUp::GetPositionEnum()
{
	return ERobotCharacterPositionEnum::Up;
}

void ARobotCharacterUp::OnInputRightDash(const FInputActionValue& InputActionValue)
{
	if (UGameplayStatics::IsGamePaused(GetWorld())) return;
	DashDirectionX = 1;
	if (FMathf::Sign(GetOrientX()) != FMathf::Sign(GetDashDirectionX()))
	{
		InputDashManagerEvent.Broadcast(ERobotCharacterPositionEnum::Up);
	}
}

void ARobotCharacterUp::OnInputLeftDash(const FInputActionValue& InputActionValue)
{
	if (UGameplayStatics::IsGamePaused(GetWorld())) return;
	DashDirectionX = -1;
	if (FMathf::Sign(GetOrientX()) != FMathf::Sign(GetDashDirectionX()))
	{
		InputDashManagerEvent.Broadcast(ERobotCharacterPositionEnum::Up);
	}
}

void ARobotCharacterUp::OnHitStop(int Damage)
{
	const UArenaSettings* Settings = GetDefault<UArenaSettings>();
	CustomTimeDilation = Settings->HitStopScale;
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [&]()
		{
			CustomTimeDilation = 1.f;
		}, Settings->HitStopTimerModifier*Damage, false);
}

