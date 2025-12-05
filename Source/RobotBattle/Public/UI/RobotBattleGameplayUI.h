// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UIGamePlayInterface.h"
#include "Blueprint/UserWidget.h"
#include "Components/EditableTextBox.h"
#include "RobotBattleGameplayUI.generated.h"


/**
 * 
 */
UCLASS()
class ROBOTBATTLE_API URobotBattleGameplayUI : public UUserWidget, public IUIGamePlayInterface
{
	GENERATED_BODY()

	DECLARE_DYNAMIC_MULTICAST_DELEGATE(FTimeOver);

public:
	
#pragma region Widget
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UProgressBar* HealthBarPlayer1;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UProgressBar* HealthBarPlayer2;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UProgressBar* ChargeBarPlayer1;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UProgressBar* ChargeBarPlayer2;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* HealthTextPlayer1;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* HealthTextPlayer2;
	
	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UTextBlock* TimerText;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UCheckBox* CheckBoxRound1;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UCheckBox* CheckBoxRound2;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UCheckBox* CheckBoxRound3;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UCheckBox* CheckBoxRound4;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UEditableTextBox* Combo1;

	UPROPERTY(EditAnywhere, meta = (BindWidget))
	class UEditableTextBox* Combo2;
#pragma endregion

#pragma region VariableWidget
	UPROPERTY(EditAnywhere, Blueprintable, Category= "Timer")
	float Timer = 0.0f;

	UPROPERTY(EditAnywhere, Blueprintable, Category= "Timer")
	bool IsActive;

#pragma endregion

#pragma region Functions

	UFUNCTION(BlueprintCallable)
	void StartTimer();

	UFUNCTION(BlueprintCallable)
	void SetTimer(float time);

	UFUNCTION(BlueprintCallable)
	void StopTimer();

#pragma endregion

	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry & MyGeometry, float InDeltaTime) override;

	virtual void SetChargePlayer(int Team, float Charge, float MaxCharge) override;
	virtual void SetHealthPlayer(int Team, float Health, float MaxHealth) override;
	virtual void SetRoundPlayer(int Team, int Win) override;
	virtual void SetComboHit(int Team, int Combo) override;

	FTimeOver OnTimeOver;

private:
	
	UFUNCTION()
	void UpdateTimer();
	void PauseTimer();

	FVector2D Combo1BaseScale = FVector2D(.5f, .5f);
	FVector2D Combo2BaseScale = FVector2D(.5f, .5f);
	
	UPROPERTY()
	float ScaleGoal1 = .5f;
	UPROPERTY()
	float ScaleGoal2 = .5f;
	UPROPERTY()
	bool Goal1 = false;
	UPROPERTY()
	bool Goal2 = false;
};
