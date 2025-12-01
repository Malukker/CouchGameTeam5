// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RobotBattlePlayerCardWidget.generated.h"

class UImage;

UCLASS()
class ROBOTBATTLE_API URobotBattlePlayerCardWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void SetPlayerImage(int32 PlayerID);

protected:
	UPROPERTY(meta = (BindWidget))
	UImage* Image_Player;
	
	UPROPERTY(EditDefaultsOnly, Category = "Player Image")
	UTexture2D* PlayerImage1;

	UPROPERTY(EditDefaultsOnly, Category = "Player Image")
	UTexture2D* PlayerImage2;

	UPROPERTY(EditDefaultsOnly, Category = "Player Image")
	UTexture2D* PlayerImage3;

	UPROPERTY(EditDefaultsOnly, Category = "Player Image")
	UTexture2D* PlayerImage4;

};
