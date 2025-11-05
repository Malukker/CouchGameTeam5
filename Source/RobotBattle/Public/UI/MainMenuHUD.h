// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MainMenuHUD.generated.h"

class URobotBattleCharacterSelection;
class URobotBattleTeamSelectMenu;
/**
 * 
 */
UCLASS()
class ROBOTBATTLE_API AMainMenuHUD : public AHUD
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	TObjectPtr<URobotBattleTeamSelectMenu> TeamSelectWidget;

	UPROPERTY(EditAnywhere)
	TObjectPtr<URobotBattleCharacterSelection> CharacterSelectionWidget;

	void SetPositionsToCharacterSelection(TMap<int32, FString> SlotNameByPlayerID);
};
