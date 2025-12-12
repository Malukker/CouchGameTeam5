// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RobotBattleCharacterSelection.generated.h"

class AMenuGameMode;
class URobotGameInstance;
enum class ERobotID : uint8;
/**
 * 
 */
UCLASS()
class ROBOTBATTLE_API URobotBattleCharacterSelection : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<URobotGameInstance> GameInstance;
	
	UPROPERTY(BlueprintReadOnly)
	TObjectPtr<AMenuGameMode> GameMode;
	
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnSelectionChange,
		uint8, ControllerID,
    	ERobotID, RobotID,
    	bool, Forward);
	UPROPERTY(BlueprintAssignable)
	FOnSelectionChange SelectionChangeEvent;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSelectionValidation, uint8, ControllerID);
	UPROPERTY(BlueprintAssignable)
	FOnSelectionValidation ValidateEvent;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnStartGameEvent);
	UPROPERTY(BlueprintAssignable)
	FOnStartGameEvent StartGameEvent;

	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSelectionCancellation, uint8, ControllerID);
	UPROPERTY(BlueprintAssignable)
	FOnSelectionCancellation CancellationEvent;

	UFUNCTION()
	void ValidateSelection(APlayerController* InController);

	UFUNCTION()
	void ChangeRobotPartSelectionForPlayer(FVector2D InDirection, APlayerController* InController);

	UFUNCTION()
	void CancelSelection(APlayerController* InController);

	UFUNCTION(BlueprintCallable)
	void InitializeAndBindInputs();

private:
	UPROPERTY(EditAnywhere)
	TMap<APlayerController*, ERobotID> BodyPartByController;

	UPROPERTY()
	TMap<APlayerController*, bool> ValidationByController;
};