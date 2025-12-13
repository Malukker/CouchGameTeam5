// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RobotCharacter.h"
#include "RobotCharacterUp.generated.h"

UCLASS()
class ROBOTBATTLE_API ARobotCharacterUp : public ARobotCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ARobotCharacterUp();

	virtual void Tick(float DeltaSeconds) override;

protected:
	UPROPERTY(EditAnywhere)
	UAnimMontage* DashAnimMontage;

	
	virtual void BindInputAndActions(UEnhancedInputComponent* EnhancedInputComponent) override;
	virtual void BeginPlay() override;
	void OnInputAttack1(const FInputActionValue& InputActionValue);
	void OnInputAttack2(const FInputActionValue& InputActionValue);
	void OnInputAttack3(const FInputActionValue& InputActionValue);
	virtual void OnInputAttackDuo(const FInputActionValue& InputActionValue) override;
	virtual void OnInputRightDash(const FInputActionValue& InputActionValue) override;
	virtual void OnInputLeftDash(const FInputActionValue& InputActionValue) override;
	virtual ERobotCharacterPositionEnum GetPositionEnum() override;
	virtual bool StartAttackDuo() override;
	virtual void PlayDash() override;
	
	UFUNCTION()
	void OnHitStop(int Damage);
	
private:
	UPROPERTY(EditAnywhere)
	float Acceleration = 750.f;
	UPROPERTY(EditAnywhere)
	float Damping = 1.f;
	UPROPERTY(EditAnywhere)
	float MaxSpeed = 1500.f;
	
	UPROPERTY(EditAnywhere)
	float MinAngle = 50.f;
	
	UPROPERTY()
	float VelocitySpring;
	UPROPERTY()
	float PosSpring;
};
