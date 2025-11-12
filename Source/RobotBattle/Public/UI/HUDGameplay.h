// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "HUDGameplay.generated.h"

/**
 * 
 */
UCLASS()
class ROBOTBATTLE_API AHUDGameplay : public AHUD
{
	GENERATED_BODY()
	
	virtual void BeginPlay() override;
	
public:
	UFUNCTION()
	void SpawnUIPause();
	
private:
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<UUserWidget> UIPauseClass;
	
	UPROPERTY()
	UUserWidget* UIPause;
};
