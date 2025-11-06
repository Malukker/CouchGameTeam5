// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MenuGameMode.generated.h"

class URobotBattleCharacterSelection;
class URobotBattleTeamSelectMenu;
class AMainMenuHUD;
class UInputMappingContext;
class APlayerMenuActor;
/**
 * 
 */
UCLASS()
class ROBOTBATTLE_API AMenuGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	TObjectPtr<UInputMappingContext> ImcMenu;

	UPROPERTY(EditAnywhere)
	TSubclassOf<APlayerMenuActor> MenuActorBlueprintClass;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<URobotBattleTeamSelectMenu> TeamSelectWidget;

	UPROPERTY(EditAnywhere)
	TSubclassOf<URobotBattleCharacterSelection> CharacterSelectWidget;

	UFUNCTION()
	void StartSelectionCharacter();

	UFUNCTION()
	void StartSelectionTeam();

	void LoadBattleLevel();
	
protected:
	virtual void BeginPlay() override;
	
private:
	void CreatePlayerMenuActors() const;
	
	void BindMenuInputsToPlayers() const;
};
