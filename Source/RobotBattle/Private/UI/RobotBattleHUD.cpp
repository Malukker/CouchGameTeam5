// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/RobotBattleHUD.h"

#include "Components/CheckBox.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "UI/RobotBattleMainMenu.h"

#pragma region Timer

void URobotBattleHUD::NativeConstruct()
{
	Super::NativeConstruct();
	elapsedTime = 0.0f;
	IsActive = false;
}

void URobotBattleHUD::NativeTick(const FGeometry & MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (IsActive && elapsedTime > 0.0f)
	{
		elapsedTime -= InDeltaTime;
		
		if (elapsedTime < 0.0f)
		{
			elapsedTime = 0.0f;
			IsActive = false;
			StopTimer();
		}
	}
	UpdateTimer();

	UpdateHealthText(0, 100.0, 200.0);

	
}

void URobotBattleHUD::UpdateTimer()
{
	if (TimerText)
	{
		int32 Minutes = FMath::FloorToInt(elapsedTime / 60.0f);
		int32 Seconds = FMath::FloorToInt(FMath::Fmod(elapsedTime, 60.0f));

		FString TextTimer = FString::Printf(TEXT("%02d:%02d"), Minutes, Seconds);
		TimerText->SetText(FText::FromString(TextTimer));
	}
}


void URobotBattleHUD::StartTimer(float time)
{
	elapsedTime = time;
	IsActive = true;
}

void URobotBattleHUD::StopTimer()
{
	if (HealthBarPlayer1->GetPercent() > HealthBarPlayer2->GetPercent())
	{
		GEngine->AddOnScreenDebugMessage(1,3,FColor::Green,"Player1 win");
	}

	if (HealthBarPlayer1->GetPercent() < HealthBarPlayer2->GetPercent())
	{
		GEngine->AddOnScreenDebugMessage(1, 3,FColor::Cyan,"Player2 Win");
	}

	if (HealthBarPlayer1->GetPercent() == HealthBarPlayer2->GetPercent())
	{
		GEngine->AddOnScreenDebugMessage(1, 3, FColor::Red, "Draw");
	}
}

void URobotBattleHUD::UpdateHealthText(int Team, float Health, float MaxHealth)
{
	if (Team == 0)
	{
		if (HealthBarPlayer1)
		{
			float NormalizePlayerHealthBar1 = HealthBarPlayer1->GetPercent();
			HealthPlayer1 = NormalizePlayerHealthBar1 * MaxHealth;
		}
	
		if (HealthTextPlayer1)
		{
			FString HealthString = FString::Printf(TEXT("%d"), FMath::RoundToInt(HealthPlayer1));
			HealthTextPlayer1->SetText(FText::FromString(HealthString));
		}
	}

	if (Team == 1)
	{
		if (HealthBarPlayer2)
		{
			float NormalizePlayerHealthBar2 = HealthBarPlayer2->GetPercent();
			HealthPlayer2 = NormalizePlayerHealthBar2 * MaxHealth;
		}

		if (HealthTextPlayer2)
		{
			FString HealthString = FString::Printf(TEXT("%d"), FMath::RoundToInt(HealthPlayer2));
			HealthTextPlayer2->SetText(FText::FromString(HealthString));
		} 
	}


}
#pragma endregion

void URobotBattleHUD::RoundIsCkecked()
{
	
}

