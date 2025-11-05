// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MenuPauseRobotBattle.h"

#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"
#include "UI/RobotBattleGameOverMenu.h"

void UMenuPauseRobotBattle::NativeConstruct()
{
	Super::NativeConstruct();

	ButtonList = {ResumeBtn, OptionBtn, MainMenuBtn};

	if (ButtonList.Num() > 0)
	{
		CurrentIndex = 0;
		UpdateButtonFocus();
	}
}


bool UMenuPauseRobotBattle::Initialize()
{
	Super::Initialize();

	if (ResumeBtn)
	{
		ResumeBtn->OnClicked.AddDynamic(this, &UMenuPauseRobotBattle::ResumeGame);
	}

	if (MainMenuBtn)
	{
		MainMenuBtn->OnClicked.AddDynamic(this, &UMenuPauseRobotBattle::MainMenuGame);
	}

	if (OptionBtn)
	{
		OptionBtn->OnClicked.AddDynamic(this, &UMenuPauseRobotBattle::OptionGame);
	}

	return true;
}

void UMenuPauseRobotBattle::ResumeGame()
{
	UGameplayStatics::SetGamePaused(GetWorld(), false);
}

void UMenuPauseRobotBattle::MainMenuGame()
{
	UGameplayStatics::OpenLevel(this,FName(""));
}

void UMenuPauseRobotBattle::OptionGame()
{
	RemoveFromParent();
	
	if (!SettingwidgetInstance)
	{
		SettingwidgetInstance = CreateWidget<UUserWidget>(GetWorld(),OptionMenu);
	}

	if (SettingwidgetInstance && !SettingwidgetInstance->IsInViewport())
	{
		SettingwidgetInstance->AddToViewport();
	}
}

FReply UMenuPauseRobotBattle::NativeOnKeyDown(const FGeometry& InGeometry, const FKeyEvent& InKeyEvent)
{
	const FKey Key = InKeyEvent.GetKey();
	if (Key == EKeys::Gamepad_DPad_Down || Key == EKeys::Down)
	{
		CurrentIndex = (CurrentIndex + 1) % ButtonList.Num();
		UpdateButtonFocus();
		return FReply::Handled();
	}

	if (Key == EKeys::Gamepad_DPad_Up || Key == EKeys::Up)
	{
		CurrentIndex = (CurrentIndex - 1 + ButtonList.Num()) % ButtonList.Num();
		UpdateButtonFocus();
		return FReply::Handled();
	}

	if (Key == EKeys::Gamepad_FaceButton_Bottom || Key == EKeys::Enter)
	{
		if (ButtonList.IsValidIndex(CurrentIndex))
		{
			ButtonList[CurrentIndex]->OnClicked.Broadcast();
		}
		return FReply::Handled();
	}
	return Super::NativeOnKeyDown(InGeometry,InKeyEvent);
}


void UMenuPauseRobotBattle::UpdateButtonFocus()
{
	for (int32 i = 0; i < ButtonList.Num(); i++)
	{
		if (ButtonList[i])
		{
			ButtonList[i]->SetKeyboardFocus();
			if (i == CurrentIndex)
			{
				ButtonList[i]->SetBackgroundColor(FLinearColor::Black);
			}
			else
			{
				ButtonList[i]->SetBackgroundColor(FLinearColor::White);
			}
		}
	}
}
