// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RobotBattlePlayerCardWidget.h"
#include "RobotBattleTeamSelectMenu.generated.h"

class AMainMenuHUD;
class UHorizontalBox;
class UVerticalBox;
class UPlayerCardWidget;
class UVerticalBoxSlot;

UENUM(BlueprintType)
enum class ETeamBox : uint8
{
	HomeUp,
	HomeDown,
	AwayUp,
	AwayDown
};

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

	UFUNCTION(BlueprintCallable)
	void SetBoxReady(ETeamBox Box, bool bIsReady);

	UFUNCTION()
	void OnPlayerMoveInput(EPlayerMenuInputDirection Direction, APlayerController* Controller);

	UFUNCTION()
	void OnPlayerValidateInput(APlayerController* Controller);

	UFUNCTION()
	void OnPlayerCancelInput(APlayerController* Controller);
	
	UFUNCTION()
	int32 GetControllerIndexForZone(const FString& ZoneName);

	virtual void NativeConstruct() override;
	
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
	UVerticalBox* Box_CenterUp;
	UPROPERTY()
	UVerticalBoxSlot* Box_CenterUpSlot;

	UPROPERTY(meta = (BindWidget))
	UVerticalBox* Box_CenterDown;
	UPROPERTY()                        
	UVerticalBoxSlot* Box_CenterDownSlot;

	UPROPERTY(meta = (BindWidget))
	UImage* IMG_WaitingPlayer1;

	UPROPERTY(meta = (BindWidget))
	UImage* IMG_WaitingPlayer2;

	UPROPERTY(meta = (BindWidget))
	UImage* IMG_WaitingPlayer3;

	UPROPERTY(meta = (BindWidget))
	UImage* IMG_WaitingPlayer4;
	
	UPROPERTY(meta = (BindWidget))
	UImage* IMG_ReadyPlayer1;

	UPROPERTY(meta = (BindWidget))
	UImage* IMG_ReadyPlayer2;

	UPROPERTY(meta = (BindWidget))
	UImage* IMG_ReadyPlayer3;

	UPROPERTY(meta = (BindWidget))
	UImage* IMG_ReadyPlayer4;
	
	UPROPERTY()
	TMap<int32, bool> PlayerReadyState;
	
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
