// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RobotBattleGameOverMenu.generated.h"

/**
 * 
 */
UCLASS()
class ROBOTBATTLE_API URobotBattleGameOverMenu : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class  UTextBlock* WinText;
	
	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UButton* RestartBtn;

	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UButton* ChangeCharacterBtn;

	UPROPERTY(EditAnywhere, meta=(BindWidget))
	class UButton* MainMenuBtn;
	
};
