// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PlayerMenuActor.generated.h"

class UInputAction;
struct FInputActionValue;
class APlayerController;

UCLASS()
class ROBOTBATTLE_API APlayerMenuActor : public APawn
{
	GENERATED_BODY()

public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnPlayerMenuMoveInput,
		FVector2D, Direction,
		APlayerController*, Controller);
	UPROPERTY()
	FOnPlayerMenuMoveInput InputMoveEvent;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerMenuValidateInput, APlayerController*, Controller);
	UPROPERTY(Blueprintable)
	FOnPlayerMenuValidateInput InputValidateEvent;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnPlayerMenuCancelInput, APlayerController*, Controller);
	UPROPERTY()
	FOnPlayerMenuCancelInput InputCancelEvent;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInput);
	UPROPERTY(BlueprintAssignable)
	FOnInput OnInput;
	
	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UInputAction> InputActionMove;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UInputAction> InputActionValidate;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UInputAction> InputActionCancel;

	UPROPERTY(EditAnywhere)
	TObjectPtr<UInputAction> InputAnyKey;

	UPROPERTY()
	TObjectPtr<APlayerController> SelfPlayerController;
	
private:
	void BeginPlay();
	
	void BindActions(UEnhancedInputComponent* Enhanced);
	
	void OnInputMove(const FInputActionValue& InputActionValue);
	
	void OnInputValidate(const FInputActionValue& InputActionValue);
	
	void OnInputCancel(const FInputActionValue& InputActionValue);
	
	void OnInputAnyKey(const FInputActionValue& InputActionValue);
};
