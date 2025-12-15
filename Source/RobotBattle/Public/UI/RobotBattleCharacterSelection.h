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
	
	UFUNCTION(BlueprintImplementableEvent)
	void SelectionChangeEvent(uint8 ControllerID, ERobotID RobotID);
	
	UFUNCTION(BlueprintImplementableEvent)
	void ValidateEvent(uint8 ControllerID);

	UFUNCTION(BlueprintImplementableEvent)
	void StartGameEvent();

	UFUNCTION(BlueprintImplementableEvent)
	void CancellationEvent(uint8 ControllerID);

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