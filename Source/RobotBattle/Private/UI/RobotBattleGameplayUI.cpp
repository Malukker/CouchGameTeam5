// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/RobotBattleGameplayUI.h"

#include "Components/CheckBox.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Components/EditableTextBox.h"
#include "Kismet/GameplayStatics.h"

#pragma region Timer

void URobotBattleGameplayUI::NativeConstruct()
{
	Super::NativeConstruct();
	Timer = 0.0f;
	IsActive = false;
}

void URobotBattleGameplayUI::NativeTick(const FGeometry & MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

	if (UGameplayStatics::IsGamePaused(GetWorld()))
		return;
	
	if (IsActive && Timer > 0.0f)
	{
		Timer -= InDeltaTime;
		
		if (Timer < 0.0f)
		{
			Timer = 0.0f;
			IsActive = false;
			OnTimeOver.Broadcast();
			StopTimer();
		}
	}
	UpdateTimer();

	if (Goal1 && Combo1BaseScale.X < ScaleGoal1 + .3f)
	{
		Combo1BaseScale += InDeltaTime * FVector2D{5.f,5.f};
		Combo1->SetRenderScale(Combo1BaseScale);
	}
	else if (Goal1)
	{
		Goal1 = false;
	}
	if (!Goal1 && Combo1BaseScale.X > ScaleGoal1)
	{
		Combo1BaseScale -= InDeltaTime * FVector2D{5.f,5.f};
		Combo1->SetRenderScale(Combo1BaseScale);
	}
	
	if (Goal2 && Combo2BaseScale.X < ScaleGoal2 + .3f)
	{
		Combo2BaseScale += InDeltaTime * FVector2D{5.f,5.f};
		Combo2->SetRenderScale(Combo2BaseScale);
	}
	else if (Goal2)
	{
		Goal2 = false;
	}
	if (!Goal2 && Combo2BaseScale.X > ScaleGoal2)
	{
		Combo2BaseScale -= InDeltaTime * FVector2D{5.f,5.f};
		Combo2->SetRenderScale(Combo2BaseScale);
	}

	if (HealthBarPlayer2CD ->GetPercent() > HealthBarPlayer2->GetPercent())
	{
		float Current = HealthBarPlayer2CD->GetPercent();
		float Target  = HealthBarPlayer2->GetPercent();

		Current -= LifeBareDecrease/100 * InDeltaTime;
		Current = FMath::Max(Current, Target);
 
		HealthBarPlayer2CD->SetPercent(Current);
	}

	if (HealthBarPlayer1CD ->GetPercent() > HealthBarPlayer1->GetPercent())
	{
		float Current = HealthBarPlayer1CD->GetPercent();
		float Target  = HealthBarPlayer1->GetPercent();

		Current -= LifeBareDecrease/100 * InDeltaTime;
		Current = FMath::Max(Current, Target);

		HealthBarPlayer1CD->SetPercent(Current);
	}
}

void URobotBattleGameplayUI::UpdateTimer()
{
	if (TimerText)
	{
		//int32 Minutes = FMath::FloorToInt(elapsedTime / 60.0f);
		int32 Seconds = FMath::FloorToInt(Timer);

		FString TextTimer = FString::Printf(TEXT("%02d"),Seconds);
		TimerText->SetText(FText::FromString(TextTimer));
	}
	
}


void URobotBattleGameplayUI::StartTimer()
{
	IsActive = true;
}


void URobotBattleGameplayUI::SetTimer(float time)
{
	Timer = time;
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
			FString HealthString = FString::Printf(TEXT("%d"), FMath::RoundToInt(Health));
			HealthTextPlayer1->SetText(FText::FromString(HealthString));
			if(Health == MaxHealth) HealthBarPlayer1CD->SetPercent(Health / MaxHealth);
		}
		
		if (LifeBarCritical >= HealthBarPlayer1->GetPercent()*100)
		{
			if (HealthBarPlayer1 && FillImage)
			{
				FProgressBarStyle NewStyle = HealthBarPlayer1->WidgetStyle;

				FSlateBrush NewFillImage;
				NewFillImage.SetResourceObject(FillImage);
				NewFillImage.ImageSize = FVector2D(
					FillImage->GetSizeX(),
					FillImage->GetSizeY()
					);

				NewStyle.FillImage = NewFillImage;
				HealthBarPlayer1->SetWidgetStyle(NewStyle);
			}
		}
	}
	if (Team == 1)
	{
		if (HealthBarPlayer2)
		{
			HealthBarPlayer2->SetPercent(Health / MaxHealth);
			FString HealthString = FString::Printf(TEXT("%d"), FMath::RoundToInt(Health));
			HealthTextPlayer2->SetText(FText::FromString(HealthString));
			if(Health == MaxHealth) HealthBarPlayer2CD->SetPercent(Health / MaxHealth);
		}
		
		if (LifeBarCritical >= HealthBarPlayer2->GetPercent()*100)
		{
			if (HealthBarPlayer2 && FillImage)
			{
				FProgressBarStyle NewStyle = HealthBarPlayer2->WidgetStyle;

				FSlateBrush NewFillImage;
				NewFillImage.SetResourceObject(FillImage);
				NewFillImage.ImageSize = FVector2D(
					FillImage->GetSizeX(),
					FillImage->GetSizeY()
					);

				NewStyle.FillImage = NewFillImage;
				HealthBarPlayer2->SetWidgetStyle(NewStyle);
			}
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
			if (Charge == MaxCharge) PlayAnimCharge(0);
			else if (Charge == 0) StopAnimCharge(0);
		}
		if (ChargeBarPlayer1->GetPercent()*100 == 100)
		{
			if (!bIsSoundPlay && ChargeBarPlayer1->GetPercent() >= 1.0f)
			UGameplayStatics::PlaySound2D(this, SpecialSound);
			bIsSoundPlay = true;
		}
		if (ChargeBarPlayer1->GetPercent()*100 < 1.0f)
		{
			bIsSoundPlay = false;
		}
	}
	if (Team == 1)
	{
		if (ChargeBarPlayer2)
		{
			ChargeBarPlayer2->SetPercent(Charge / MaxCharge);
			if (Charge == MaxCharge) PlayAnimCharge(1);
			else if (Charge == 0) StopAnimCharge(1);
		}

		if (ChargeBarPlayer2->GetPercent()*100 == 100)
		{
			if (!bIsSoundPlay && ChargeBarPlayer2->GetPercent() >= 1.0f)
				UGameplayStatics::PlaySound2D(this, SpecialSound);
			bIsSoundPlay = true;
		}
		if (ChargeBarPlayer2->GetPercent()*100 < 1.0f)
		{
			bIsSoundPlay = false;
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

void URobotBattleGameplayUI::SetComboHit(int Team, int Combo)
{
	if (Team == 0)
	{
		if (Combo > 1)
		{
			FString ComboString = FString::Printf(TEXT("x%d"));
			Combo1->SetText(FText::FromString(ComboString));
			Combo1->SetVisibility(ESlateVisibility::Visible);
			Goal1 = true;
			if (Combo >= 3)
			{
				float ScaleFactor = (Combo - 2) * 0.01f;
				ScaleGoal1 += ScaleFactor;
			}
		}else
		{
			ScaleGoal1 = 1.0f;
			Combo1BaseScale = FVector2D(.5f, .5f);
			Combo1->SetRenderScale(Combo1BaseScale);
			Combo1->SetVisibility(ESlateVisibility::Hidden);
		}
	}
	if (Team == 1)
	{
		if (Combo > 1)
		{
			FString ComboString = FString::Printf(TEXT("x%d"));
			
			Combo2->SetText(FText::FromString(ComboString));
			Combo2->SetVisibility(ESlateVisibility::Visible);
			Goal2 = true;
			if (Combo >= 3)
			{
				float ScaleFactor = (Combo - 2) * 0.01f;
				ScaleGoal2 += ScaleFactor;
			}
		}else
		{
			ScaleGoal2 = 1.0f;
			Combo2BaseScale = FVector2D(.5f, .5f);
			Combo2->SetRenderScale(Combo2BaseScale);
			Combo2->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

void URobotBattleGameplayUI::SetFillImage()
{
	
	if (HealthBarPlayer1 && FillImage)
	{
		FProgressBarStyle NewStyle = HealthBarPlayer1->WidgetStyle;

		FSlateBrush NewFillImage;
		NewFillImage.SetResourceObject(FillImage);
		NewFillImage.ImageSize = FVector2D(
			FillImage->GetSizeX(),
			FillImage->GetSizeY()
			);

		NewStyle.FillImage = NewFillImage;
		HealthBarPlayer1->SetWidgetStyle(NewStyle);
	}

	if (HealthBarPlayer2->GetPercent() <= HealthBarPlayer2->GetPercent()/LifeBarCritical)
	{
		if (HealthBarPlayer2 && FillImage)
		{
			FProgressBarStyle NewStyle = HealthBarPlayer2->WidgetStyle;

			FSlateBrush NewFillImage;
			NewFillImage.SetResourceObject(FillImage);
			NewFillImage.ImageSize = FVector2D(
				FillImage->GetSizeX(),
				FillImage->GetSizeY()
				);

			NewStyle.FillImage = NewFillImage;
			HealthBarPlayer2->SetWidgetStyle(NewStyle);
		}
	}

		


}

