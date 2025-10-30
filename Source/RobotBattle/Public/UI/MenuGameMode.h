// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MenuGameMode.generated.h"

class UInputMappingContext;
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

protected:
	virtual void BeginPlay() override;
	
private:
	void CreatePlayerMenuActors() const;
	
	void BindMenuInputsToPlayers() const;
};
