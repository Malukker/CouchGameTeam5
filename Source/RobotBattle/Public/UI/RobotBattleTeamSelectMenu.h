// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RobotBattlePlayerCardWidget.h"
#include "RobotBattleTeamSelectMenu.generated.h"

class UHorizontalBox;
class UPlayerCardWidget;

UCLASS()
class ROBOTBATTLE_API URobotBattleTeamSelectMenu : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	UFUNCTION(BlueprintCallable)
	void AddPlayer(int32 PlayerID);

	UFUNCTION(BlueprintCallable)
	void MovePlayerToZone(int32 PlayerID, const FString& ZoneName);

protected:
	
	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* Box_HomeUp;

	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* Box_HomeDown;

	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* Box_AwayUp;

	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* Box_AwayDown;

	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* Box_CenterUp;

	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* Box_CenterDown;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<URobotBattlePlayerCardWidget> PlayerCardClass;

private:
	UPROPERTY()
	TMap<int32, URobotBattlePlayerCardWidget*> PlayerCards;

	UHorizontalBox* GetZoneByName(const FString& ZoneName) const;	
};
