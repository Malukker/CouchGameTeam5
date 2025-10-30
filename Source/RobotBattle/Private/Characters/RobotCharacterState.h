// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Characters/RobotCharacterStateID.h"
#include "RobotCharacterState.generated.h"

class ARobotCharacter;
class URobotCharacterStateMachine;
class URobotCharacterSettings;
class UCharacterMovementComponent;

UCLASS(Abstract)
class URobotCharacterState : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	URobotCharacterState();

	virtual ERobotCharacterStateID GetStateID();

	virtual void StateInit(URobotCharacterStateMachine* InStateMachine);
	virtual void StateEnter(ERobotCharacterStateID PreviousStateID);
	virtual void StateExit(ERobotCharacterStateID NextState);
	virtual void StateTick(float DeltaTime);

protected:
	UPROPERTY()
	TObjectPtr<ARobotCharacter> Character;

	UPROPERTY()
	TObjectPtr<URobotCharacterStateMachine> StateMachine;

	UPROPERTY()
	const URobotCharacterSettings* CharacterSettings;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<UCharacterMovementComponent> CharacterMovement;
};
