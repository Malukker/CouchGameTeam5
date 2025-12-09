// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Attacks/AttackStruct.h"
#include "Enums/RobotCharacterDownChargeID.h"
#include "GameFramework/Character.h"
#include "Interface/Robot.h"
#include "RobotCharacter.generated.h"

class URobotGameInstance;
class UBoxComponent;
class AHUDGameplay;
enum class ERobotCharacterUpID : uint8;
enum class ERobotCharacterPositionEnum : uint8;
enum class ERobotCharacterDownChargeID : uint8;
enum class  ERobotID : uint8;
enum class EAttackID: uint8;
class URobotCharacterStateMachine;
class URobotCharacterInputData;
class URobotBattleInputDataMenu;
class UInputMappingContext;
class UEnhancedInputComponent;
struct FInputActionValue;

UCLASS()
class ROBOTBATTLE_API ARobotCharacter : public ACharacter, public IRobot
{
	GENERATED_BODY()

#pragma region Unreal Default

public:
	// Sets default values for this character's properties
	ARobotCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY()
	AHUDGameplay* HUDGameplay;
	
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UAudioComponent> AudioComponent;

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

	void ResetStateMachine();

protected:
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<URobotCharacterStateMachine> StateMachine;

#pragma endregion

#pragma region Input Data / Mapping Context

public:
	UPROPERTY()
	TObjectPtr<UInputMappingContext> InputMappingContextGameplay;
	UPROPERTY()
	TObjectPtr<UInputMappingContext> InputMappingContextMenu;

	UPROPERTY()
	TObjectPtr<URobotCharacterInputData> InputDataGameplay;

protected:
	void SetupMappingContextIntoController(bool bMenu) const;

#pragma endregion

#pragma region Input
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FInputAttackEvent);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FInputJumpEvent);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FInputDashManagerEvent,ERobotCharacterPositionEnum ,Position);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FInputDashEvent);	

public:
	float GetInputMoveX() const;
	bool IsWalkingForward() const;
	
	EAttackID GetCurrentTypeAttack() const;
	void SetStunTimer(float StunTime) ;
	float GetStunTimer() const;
	
	int GetDashDirectionX() const;
	void UseDash();
	void ResetDash();
	void SetRobotBodyID(ERobotID Robot);
	ERobotID GetRobotBodyID() const;
	
	virtual void TakeDamageFromAttack(int Damage, float StunTime);
	void StartControllerVibration(::ERobotID ERobotID, ::EAttackID EAttackID);
	void KnockBackFromNotify(FVector2D Velocity);

	UPROPERTY()
	FInputJumpEvent InputJumpEvent;

	UPROPERTY()
	FInputAttackEvent InputAttackEvent;

	UPROPERTY()
	FInputDashEvent InputDashEvent;
	
	UPROPERTY()
	FInputDashManagerEvent InputDashManagerEvent;


protected:
	UPROPERTY()
	float InputMoveX = 0.f;
	UPROPERTY()
	float StunTimer = 0;
	UPROPERTY()
	int DashDirectionX = 0;
	UPROPERTY()
	bool CanDash = true;
	UPROPERTY()
	bool WantSwitch = false;
	UPROPERTY()
	ERobotID RobotID = ERobotID::None;

	UPROPERTY()
	EAttackID CurrentTypeAttack = EAttackID::None;

	virtual void OnInputMoveX(const FInputActionValue& InputActionValue);
	virtual void OnInputRightDash(const FInputActionValue& InputActionValue);
	virtual void OnInputLeftDash(const FInputActionValue& InputActionValue);
	virtual void OnInputAttackDuo(const FInputActionValue& InputActionValue);
	virtual void OnInputPause(const FInputActionValue& InputActionValue);
	virtual void BindInputAndActions(UEnhancedInputComponent* EnhancedInputComponent);


#pragma endregion

#pragma region Info
public:
	UFUNCTION()
	virtual	ERobotCharacterPositionEnum GetPositionEnum();

	UFUNCTION()
	virtual ERobotCharacterDownChargeID GetRobotCharacterDownChargeID();

	virtual FVector GetRobotLocation() override;

	virtual bool IsTargetFollowable() override;

	UBoxComponent* GetCollision() const;

	int GetTeam();
	void SetTeam(int NewTeam);
	void SetLookingOpponent(bool Value);
	int GetLife();
	int GetGuard();
	int GetInvinsibilityFrames();
	void PlayIntro();
	virtual void PlayDash();
	void PlayEnd(bool Win);
	
protected:
	UPROPERTY(EditAnywhere)
	UBoxComponent* BoxComponent;
	
	UPROPERTY(EditAnywhere)
	UAnimMontage* Intro;
	UPROPERTY(EditAnywhere)
	UAnimMontage* EndWin;
	UPROPERTY(EditAnywhere)
	UAnimMontage* EndDefeat;
	
	UPROPERTY(BlueprintReadOnly)
	int Team;

	UPROPERTY(EditAnywhere)
	int Life = 500;

	UPROPERTY(EditAnywhere)
	int Guard = 2;

	UPROPERTY(EditAnywhere)
	bool IsFollowable = false;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool IsLookingOpponent = false;
	
	UPROPERTY(EditAnywhere)
	bool MeshMirror = false;

	UPROPERTY(EditAnywhere)
	int InvinsibilityFrames = 12;
	
	UPROPERTY(EditAnywhere)
	float NerfStatsMultiplier = 0.5f;
	

#pragma endregion

#pragma region Damage/Stun
public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FHurtManagerEvent, int, Damage, float, StunTimer);
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FGuardManagerEvent, int, Damage, float, StunTimer);
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FGuardEvent, int, GuardMax, int, GuardLeft);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAttackManagerEvent, bool, HasAttack);
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FDoGuardEvent, bool, Guard);
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAirStopManagerEvent);
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FHurtEvent);
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FBoostEvent);
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FGuardResetManagerEvent);

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FVibrationControllerEvent,ERobotID, RobotID,EAttackID ,AttackID);

	UPROPERTY()
	FAirStopManagerEvent AirStopManagerEvent;
	
	UPROPERTY()
	FHurtEvent HurtEvent;
	
	UPROPERTY(BlueprintAssignable, Category = "GuardEvent")
	FGuardEvent GuardEvent;
	
	UPROPERTY(BlueprintAssignable, Category = "GuardEvent")
	FDoGuardEvent DoGuardEvent;
	
	UPROPERTY()
	FGuardManagerEvent GuardManagerEvent;
	
	UPROPERTY(BlueprintAssignable, Category = "GuardEvent")
	FGuardResetManagerEvent GuardResetManagerEvent;
	
	UPROPERTY()
	FHurtManagerEvent HurtManagerEvent;

	UPROPERTY()
	FVibrationControllerEvent VibrationControllerEvent;

	UPROPERTY()
	FAttackManagerEvent AttackManagerEvent;

	UPROPERTY()
	FBoostEvent BoostManagerEvent;

	UPROPERTY()
	FBoostEvent BoostEvent;

	UFUNCTION()
	void SetBoost(bool Value);
	UFUNCTION()
	bool GetBoost();

	UFUNCTION()
	void SetGuard(bool Value);
	
private:

	bool UseBoost = false;

	bool CanGuard = false;
	
#pragma endregion

#pragma region Charge
public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FChargeManagerEvent,ERobotCharacterPositionEnum ,Position);
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FAttackDuoManagerEvent,ERobotCharacterPositionEnum ,Position);
	
	virtual void SetCanAttackDuo(bool CanAttack);
	virtual bool StartAttackDuo();
	
	void AddAttackDuoBonus();
	int GetAttackDuoBonus();
	void ResetAttackDuoBonus();

	int GetCharge();
	
protected:
	UPROPERTY(EditAnywhere)
	int Charge = 0;
	
	int AttackDuoBonus = 0;
	bool CanAttackDuo = false;

public:
	UPROPERTY()
	FChargeManagerEvent ChargeManagerEvent;
	
	UPROPERTY()
	FAttackDuoManagerEvent AttackDuoManagerEvent;
	
#pragma endregion

#pragma region HitStop
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHitStop,int,Damage);
	FHitStop HitStopEvent;
#pragma endregion

#pragma region KnockBack
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FKnockBack,FVector2D,KnockBackVelocity);
	FKnockBack KnockBackEvent;
#pragma endregion
	
};
