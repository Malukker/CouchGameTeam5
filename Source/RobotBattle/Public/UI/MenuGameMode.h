// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MenuGameMode.generated.h"

class UUserWidget;
class URobotBattleCharacterSelection;
class URobotBattleTeamSelectMenu;
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
	TSubclassOf<UUserWidget> MainMenuWidget;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<URobotBattleTeamSelectMenu> TeamSelectWidget;

	UPROPERTY(EditAnywhere)
	TSubclassOf<URobotBattleCharacterSelection> CharacterSelectWidget;

	UFUNCTION(BlueprintCallable)
	void StartSelectionCharacter();

	UFUNCTION(BlueprintCallable)
	void StartSelectionTeam();

	void LoadBattleLevel();
	
protected:
	virtual void BeginPlay() override;
	
private:
	void CreatePlayerMenuActors() const;
	
	void BindMenuInputsToPlayers() const;
};
