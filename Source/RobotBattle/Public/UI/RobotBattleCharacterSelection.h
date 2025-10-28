// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RobotBattleCharacterSelection.generated.h"

enum class ERobotID : uint8;
enum class EPlayerMenuInputDirection : uint8;
/**
 * 
 */
UCLASS()
class ROBOTBATTLE_API URobotBattleCharacterSelection : public UUserWidget
{
	GENERATED_BODY()

public:
	void AssignPlayerControllerToRobotPart(APlayerController* InController);

	void MoveToNextPart(EPlayerMenuInputDirection InDirection, APlayerController* InController);

	void CancelAssignment(APlayerController* InController);

protected:
	virtual void NativeOnInitialized() override;

private:
	UPROPERTY()
	TMap<APlayerController*, ERobotID> BodyPartFromController;

	void BindEventsToMenuControl();
};