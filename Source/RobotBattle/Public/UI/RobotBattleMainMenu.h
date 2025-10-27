// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RobotBattleMainMenu.generated.h"

/**
 * 
 */
UCLASS()
class ROBOTBATTLE_API URobotBattleMainMenu : public UUserWidget
{
	GENERATED_BODY()

public:
	
	virtual bool Initialize() override;
	
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* PlayBtn;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* OptionBtn;
	
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UButton* QuitBtn;

private:
	
	UFUNCTION(BlueprintCallable)
	void StartGame();

	UFUNCTION(BlueprintCallable)
	void OptionGame();

	UFUNCTION(BlueprintCallable)
	void QuitGame();
};
