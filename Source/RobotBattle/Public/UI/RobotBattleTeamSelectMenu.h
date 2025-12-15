// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RobotBattlePlayerCardWidget.h"
#include "RobotBattleTeamSelectMenu.generated.h"

class AMenuGameMode;
class UCanvasPanel;
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


	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSelectionValidation, int, ControllerID);
	UPROPERTY(BlueprintAssignable)
	FOnSelectionValidation ValidateEvent;
	
	
	UFUNCTION(BlueprintCallable)
	void CustomConstruct();

	UFUNCTION(BlueprintCallable)
	void AddPlayer(int32 PlayerID);

	UFUNCTION(BlueprintCallable)
	void MovePlayerToZone(int32 PlayerID, const FString& ZoneName);

	UFUNCTION(BlueprintCallable)
	void SetBoxReady(ETeamBox Box, bool bIsReady);

	UFUNCTION()
	void OnPlayerMoveInput(FVector2D Direction, APlayerController* Controller);

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
	UVerticalBox* Box_Center;

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

	
	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* CanvasPanel_TeamSelection;

#pragma region ControlsBindWidget

	bool TeamSelectionDone = false;

	TArray<bool> PlayerChangedImg;
	
	UPROPERTY(meta = (BindWidget))
	UCanvasPanel* CanvasPanel_Controls;
	
	UPROPERTY(meta = (BindWidget))
	UImage* IMG_Player1;

	UPROPERTY(meta = (BindWidget))
	UImage* IMG_Player2;

	UPROPERTY(meta = (BindWidget))
	UImage* IMG_Player3;

	UPROPERTY(meta = (BindWidget))
	UImage* IMG_Player4;

	UPROPERTY(EditAnywhere)
	UTexture2D* IMG_Ready1;

	UPROPERTY(EditAnywhere)
	UTexture2D* IMG_Ready2;

	UPROPERTY(EditAnywhere)
	UTexture2D* IMG_Ready3;

	UPROPERTY(EditAnywhere)
	UTexture2D* IMG_Ready4;

	UPROPERTY(EditAnywhere)
	TArray<USoundBase*> IterationSoundTeamSelection;

	UPROPERTY(EditAnywhere)
	class USoundBase* ValidateSound;

#pragma endregion
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

	UPROPERTY()
	AMenuGameMode* GM ;
	
	UFUNCTION()
	void ChangeImageWhenReady(APlayerController* controller);

	void CheckAllBoolAndGoToNextLevel();
};
