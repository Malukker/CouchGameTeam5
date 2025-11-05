// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RobotBattleCharacterSelection.generated.h"

enum class ERobotID : uint8;
enum class EPlayerMenuInputDirection : int;
/**
 * 
 */
UCLASS()
class ROBOTBATTLE_API URobotBattleCharacterSelection : public UUserWidget
{
	GENERATED_BODY()

public:
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnSelectionChange,
		uint8, ControllerID,
    	ERobotID, RobotID);
	UPROPERTY(BlueprintAssignable)
	FOnSelectionChange SelectionChangeEvent;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSelectionValidation, uint8, ControllerID);
	UPROPERTY(BlueprintAssignable)
	FOnSelectionValidation ValidateEvent;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSelectionCancellation, uint8, ControllerID);
	UPROPERTY(BlueprintAssignable)
	FOnSelectionCancellation CancellationEvent;

	UFUNCTION()
	void ValidateSelection(APlayerController* InController);

	UFUNCTION()
	void ChangeRobotPartSelectionForPlayer(EPlayerMenuInputDirection InDirection, APlayerController* InController);

	UFUNCTION()
	void CancelSelection(APlayerController* InController);

	UFUNCTION(BlueprintCallable)
	void BindEventsToMenuControl();
	
protected:
	virtual void NativeOnInitialized() override;

private:
	bool CanInput;
	
	UPROPERTY()
	TMap<APlayerController*, ERobotID> BodyPartByController;

	UPROPERTY()
	TMap<APlayerController*, bool> ValidationByController;

	void ResetInput();
};