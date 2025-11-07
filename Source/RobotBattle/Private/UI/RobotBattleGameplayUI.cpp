// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/RobotBattleGameplayUI.h"

#include "Components/CheckBox.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"

#pragma region Timer

void URobotBattleGameplayUI::NativeConstruct()
{
	Super::NativeConstruct();
	elapsedTime = 0.0f;
	IsActive = false;
}

void URobotBattleGameplayUI::NativeTick(const FGeometry & MyGeometry, float InDeltaTime)
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
	
}
void URobotBattleGameplayUI::UpdateTimer()
{
	if (TimerText)
	{
		int32 Minutes = FMath::FloorToInt(elapsedTime / 60.0f);
		int32 Seconds = FMath::FloorToInt(FMath::Fmod(elapsedTime, 60.0f));

		FString TextTimer = FString::Printf(TEXT("%02d:%02d"), Minutes, Seconds);
		TimerText->SetText(FText::FromString(TextTimer));
	}
}


void URobotBattleGameplayUI::StartTimer(float time)
{
	elapsedTime = time;
	IsActive = true;
}

void URobotBattleGameplayUI::StopTimer()
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
#pragma endregion

void URobotBattleGameplayUI::SetHealthPlayer(int Team, float Health, float MaxHealth)
{
	if (Team == 0)
	{
		if (HealthBarPlayer1)
		{
			HealthBarPlayer1->SetPercent(Health / MaxHealth);
		}
		if (HealthTextPlayer1)
		{
			FString HealthString = FString::Printf(TEXT("%d"), FMath::RoundToInt(Health));
			HealthTextPlayer1->SetText(FText::FromString(HealthString));
		}
	}
	if (Team == 1)
	{
		if (HealthBarPlayer2)
		{
			HealthBarPlayer2->SetPercent(Health / MaxHealth);
		}
		if (HealthTextPlayer2)
		{
			FString HealthString = FString::Printf(TEXT("%d"), FMath::RoundToInt(Health));
			HealthTextPlayer2->SetText(FText::FromString(HealthString));
		} 
	}
}

void URobotBattleGameplayUI::SetChargePlayer(int Team, float Charge, float MaxCharge)
{
	if (Team == 0)
	{
		if (ChargeBarPlayer1)
		{
			ChargeBarPlayer1->SetPercent(Charge / MaxCharge);
		}
	}
	if (Team == 1)
	{
		if (ChargeBarPlayer2)
		{
			ChargeBarPlayer2->SetPercent(Charge / MaxCharge);
		}
	}
}

void URobotBattleGameplayUI::SetRoundPlayer(int Team, int Win)
{
	if (Team == 0)
	{
		if (Win == 1)
		{
			CheckBoxRound1->SetIsChecked(true);
		}
		else if (Win == 2)
		{
			CheckBoxRound2->SetIsChecked(true);
		}
	}
	else if (Team == 1)
	{
		if (Win == 1)
		{
			CheckBoxRound3->SetIsChecked(true);
		}
		else if (Win == 2)
		{
			CheckBoxRound4->SetIsChecked(true);
		}
	}
}