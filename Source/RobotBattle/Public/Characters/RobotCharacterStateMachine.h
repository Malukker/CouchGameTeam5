// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "RobotCharacterStateMachine.generated.h"

enum class ERobotCharacterStateID : uint8;
class URobotCharacterState;
class ARobotCharacter;

/**
 * 
 */
UCLASS()
class ROBOTBATTLE_API URobotCharacterStateMachine : public UObject
{
	GENERATED_BODY()
	

public:
	void Init(ARobotCharacter* InCharacter);

	void Tick(float DeltaTime);

	ARobotCharacter* GetCharacter() const;

	UFUNCTION(BlueprintCallable)
	void ChangeState(ERobotCharacterStateID NextState);

	URobotCharacterState* GetState(ERobotCharacterStateID StateID);

	ERobotCharacterStateID GetCurrentStateID() const;

protected:
	UPROPERTY()
	TObjectPtr<ARobotCharacter> Character;

	UPROPERTY()
	TArray<URobotCharacterState*> AllStates;

	UPROPERTY(BlueprintReadOnly)
	ERobotCharacterStateID CurrentStateID;

	UPROPERTY()
	TObjectPtr<URobotCharacterState> CurrentState;

	void FindStates();

	void InitStates();
};
