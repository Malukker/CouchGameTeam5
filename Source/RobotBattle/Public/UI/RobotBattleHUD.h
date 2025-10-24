// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "RobotBattleHUD.generated.h"

/**
 * 
 */
UCLASS()
class ROBOTBATTLE_API URobotBattleHUD : public UUserWidget
{
	GENERATED_BODY()

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
	
#pragma endregion

#pragma region VariableWidget
	
	UPROPERTY(EditAnywhere, Blueprintable, Category= "Player")
	float HealthPlayer1;

	UPROPERTY(EditAnywhere, Blueprintable, Category= "Player")
	float HealthPlayer2;
	
	UPROPERTY(EditAnywhere, Blueprintable, Category= "Timer")
	float elapsedTime = 0.0f;

	UPROPERTY(EditAnywhere, Blueprintable, Category= "Timer")
	bool IsActive;

#pragma endregion

#pragma region Functions

	UFUNCTION(BlueprintCallable)
	void StartTimer(float time);

	UFUNCTION(BlueprintCallable)
	void StopTimer();

	

#pragma endregion

	virtual void NativeConstruct() override;
	virtual void NativeTick(const FGeometry & MyGeometry, float InDeltaTime) override;

private:
	
	UFUNCTION()
	void UpdateTimer();

	UFUNCTION()
	void UpdateHealthText();
	
	UFUNCTION()
	void RoundIsCkecked();
	
};
