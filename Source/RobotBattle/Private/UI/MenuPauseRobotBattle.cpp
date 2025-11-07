// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MenuPauseRobotBattle.h"

#include "Characters/RobotCharacter.h"
#include "Characters/MainMenu/PlayerMenuActor.h"
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
	}

	NavigationMenuUp();
	NavigationMenuDown();
	NavigationMenuValidate();
	NavigationMenuFocus();

	
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

void UMenuPauseRobotBattle::NavigationMenuUp()
{
	CurrentIndex = (CurrentIndex - 1 + ButtonList.Num()) % ButtonList.Num();
	NavigationMenuFocus();
}

void UMenuPauseRobotBattle::NavigationMenuDown()
{
	CurrentIndex = (CurrentIndex + 1) % ButtonList.Num();
	NavigationMenuFocus();
}

void UMenuPauseRobotBattle::NavigationMenuValidate()
{
	if (ButtonList.IsValidIndex(CurrentIndex))
	{
		ButtonList[CurrentIndex]->OnClicked.Broadcast();
	}
}

void UMenuPauseRobotBattle::NavigationMenuFocus()
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

void UMenuPauseRobotBattle::BindToPlayerMenuActor(APlayerMenuActor* PlayerMenuActor)
{
	if (!PlayerMenuActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("BindToPlayerMenuActor: PlayerMenuActor is null"));
		return;
	}
	
	PlayerMenuActor->InputMoveEvent.AddDynamic(this, &UMenuPauseRobotBattle::OnMenuMoveInput);
	
	PlayerMenuActor->InputValidateEvent.AddDynamic(this, &UMenuPauseRobotBattle::OnMenuValidateInput);
}

void UMenuPauseRobotBattle::OnMenuMoveInput(EPlayerMenuInputDirection Direction, APlayerController* Controller)
{
	switch (Direction)
	{
	case EPlayerMenuInputDirection::Up:
		NavigationMenuUp();
		break;
	case EPlayerMenuInputDirection::Down:
		NavigationMenuDown();
		break;
	default:
		break;
	}
}

void UMenuPauseRobotBattle::OnMenuValidateInput(APlayerController* Controller)
{
	NavigationMenuValidate();
}


