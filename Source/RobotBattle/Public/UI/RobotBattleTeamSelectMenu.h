// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RobotBattlePlayerCardWidget.h"
#include "RobotBattleTeamSelectMenu.generated.h"

class AMainMenuHUD;
class UHorizontalBox;
class UPlayerCardWidget;

UCLASS()
class ROBOTBATTLE_API URobotBattleTeamSelectMenu : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void CustomConstruct();

	UFUNCTION(BlueprintCallable)
	void AddPlayer(int32 PlayerID);

	UFUNCTION(BlueprintCallable)
	void MovePlayerToZone(int32 PlayerID, const FString& ZoneName);

	UFUNCTION()
	void OnPlayerMoveInput(EPlayerMenuInputDirection Direction, APlayerController* Controller);

	UFUNCTION()
	void OnPlayerValidateInput(APlayerController* Controller);

	UFUNCTION()
	void OnPlayerCancelInput(APlayerController* Controller);

	UFUNCTION()
	int32 GetControllerIndexForZone(const FString& ZoneName);

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

	UPROPERTY()
	TMap<int32, FString> CurrentZones;

	UPROPERTY()
	TMap<int32, bool> HasValidatedByPlayer;

	bool IsZoneOccupied(const FString& ZoneName) const;
	
	UHorizontalBox* GetZoneByName(const FString& ZoneName) const;	
};
