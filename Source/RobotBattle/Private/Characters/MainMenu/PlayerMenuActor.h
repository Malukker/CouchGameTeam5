// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlayerMenuActor.generated.h"

class UInputAction;
class FInputActionValue;
class APlayerController;

UENUM()
enum class EPlayerMenuInputDirection
{
	None = 0,
	Up,
	Down,
	Left,
	Right,
};

UCLASS()
class ROBOTBATTLE_API APlayerMenuActor : public APawn
{
	GENERATED_BODY()

public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnPlayerMenuMoveInput,
		EPlayerMenuInputDirection, Direction,
		APlayerController*, Controller)
	UPROPERTY()
	FOnPlayerMenuMoveInput InputMoveEvent;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerMenuValidateInput, APlayerController*, Controller)
	UPROPERTY()
	FOnPlayerMenuValidateInput InputValidateEvent;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerMenuCancelInput, APlayerController*, Controller)
	UPROPERTY()
	FOnPlayerMenuCancelInput InputCancelEvent;
	
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UInputAction> InputActionMoveInMenu;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UInputAction> InputActionValidate;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UInputAction> InputActionCancel;
	
private:
	UPROPERTY()
	TObjectPtr<APlayerController> SelfPlayerController;
	
	void BindActions(UEnhancedInputComponent* Enhanced);

	void OnInputMoveMenu(const FInputActionValue& InputActionValue);
	
	void OnInputValidate(const FInputActionValue& InputActionValue);
	
	void OnInputCancel(const FInputActionValue& InputActionValue);
};
