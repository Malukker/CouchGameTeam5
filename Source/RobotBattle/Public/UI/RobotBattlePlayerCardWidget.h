// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RobotBattlePlayerCardWidget.generated.h"

class UTextBlock;

UCLASS()
class ROBOTBATTLE_API URobotBattlePlayerCardWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void SetPlayerName(const FString& NewName);

protected:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* Text_PlayerName;
};
