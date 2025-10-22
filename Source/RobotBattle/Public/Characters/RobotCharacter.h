// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Attacks/AttackStruct.h"
#include "GameFramework/Character.h"
#include "RobotCharacter.generated.h"

enum class ERobotCharacterPositionEnum : uint8;
enum class  ERobotID : uint8;
enum class EAttackID: uint8;
class URobotCharacterStateMachine;
class URobotCharacterInputData;
class UInputMappingContext;
class UEnhancedInputComponent;
struct FInputActionValue;

UCLASS()
class ROBOTBATTLE_API ARobotCharacter : public ACharacter
{
	GENERATED_BODY()

#pragma region Unreal Default

public:
	// Sets default values for this character's properties
	ARobotCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

#pragma endregion Unreal Default

#pragma region Orient

public:
	float GetOrientX() const;

	void SetOrientX(float NewOrientX);

protected:
	UPROPERTY(BlueprintReadOnly)
	float OrientX = 1.f;

	void RotateMeshUsingOrientX() const;

#pragma endregion

#pragma region State Machine

public:
	void CreateStateMachine();

	void InitStateMachine();

	void TickStateMachine(float DeltaTime) const;

protected:
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<URobotCharacterStateMachine> StateMachine;

#pragma endregion

#pragma region Input Data / Mapping Context

public:
	UPROPERTY()
	TObjectPtr<UInputMappingContext> InputMappingContext;

	UPROPERTY()
	TObjectPtr<URobotCharacterInputData> InputData;

protected:
	void SetupMappingContextIntoController() const;

#pragma endregion

#pragma region Input
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInputAttackEvent, EAttackID, AttackType);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FInputJumpEvent);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FInputDashEvent);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInputStunEvent,float,StunTimer);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInputLockEvent,bool,IsLocked);
	

public:
	float GetInputMoveX() const;
	EAttackID GetCurrentTypeAttack() const;
	float GetStunTimer() const;
	void SetStunTimer(float NewStunTimer);
	ERobotID GetRobotBodyID() const;
	uint8 GetDashDirectionX() const;
	
	UPROPERTY()
	FInputJumpEvent InputJumpEvent;

	UPROPERTY()
	FInputAttackEvent InputAttackEvent;

	UPROPERTY()
	FInputDashEvent InputStunEvent;

	UPROPERTY()
	FInputDashEvent InputDashEvent;

	UPROPERTY(EditAnywhere)
	uint8 Life = 0;

	UPROPERTY(EditAnywhere)
	uint8 Guard = 0;

protected:
	UPROPERTY()
	float InputMoveX = 0.f;
	UPROPERTY()
	float StunTimer = 0;
	UPROPERTY()
	uint8 DashDirectionX = 0;
	UPROPERTY()
	ERobotID RobotID = ERobotID::None;

	UPROPERTY(EditAnywhere)
	EAttackID CurrentTypeAttack = EAttackID::None;

	virtual void BindInputAndActions(UEnhancedInputComponent* EnhancedInputComponent);


#pragma endregion

#pragma region Info
public:
	
	UFUNCTION()
	virtual	ERobotCharacterPositionEnum GetPositionEnum();
	
#pragma endregion

};
