// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/RobotBattleTeamSelectMenu.h"

#include "Blueprint/WidgetTree.h"
#include "UI/RobotBattlePlayerCardWidget.h"
#include "Components/HorizontalBox.h"
#include "Components/PanelWidget.h"

void URobotBattleTeamSelectMenu::NativeConstruct()
{
	Super::NativeConstruct();

	for (int32 i = 0; i < 4; i++)
	{
	
		URobotBattlePlayerCardWidget* NewCard = WidgetTree->ConstructWidget<URobotBattlePlayerCardWidget>(PlayerCardClass);
		
		NewCard->SetPlayerName(FString::Printf(TEXT("Player %d"), i + 1));

		if (i % 2 == 0)
			Box_CenterUp->AddChild(NewCard);
		else
			Box_CenterDown->AddChild(NewCard);

		PlayerCards.Add(i, NewCard);
	}
}

void URobotBattleTeamSelectMenu::AddPlayer(int32 PlayerID)
{
	if (!PlayerCardClass)
	{
		UE_LOG(LogTemp, Error, TEXT("PlayerCardClass is NULL!"));
		return;
	}
	if (!Box_CenterUp || !Box_CenterDown)
	{
		UE_LOG(LogTemp, Error, TEXT("Center boxes are NULL!"));
		return;
	}
	
	URobotBattlePlayerCardWidget* NewCard = WidgetTree->ConstructWidget<URobotBattlePlayerCardWidget>(PlayerCardClass);
	if (!NewCard)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to create PlayerCard widget!"));
		return;
	}
	
	NewCard->SetPlayerName(FString::Printf(TEXT("Player %d"), PlayerID + 1));

	if (PlayerID % 2 == 0)
		Box_CenterUp->AddChild(NewCard);
	else
		Box_CenterDown->AddChild(NewCard);

	PlayerCards.Add(PlayerID, NewCard);

	UE_LOG(LogTemp, Log, TEXT("Added PlayerCard for Player %d"), PlayerID);
	UE_LOG(LogTemp, Log, TEXT("Box_CenterUp Children Count: %d"), Box_CenterUp->GetChildrenCount());
	UE_LOG(LogTemp, Log, TEXT("Box_CenterDown Children Count: %d"), Box_CenterDown->GetChildrenCount());
}

void URobotBattleTeamSelectMenu::MovePlayerToZone(int32 PlayerID, const FString& ZoneName)
{
	if (!PlayerCards.Contains(PlayerID)) return;

	URobotBattlePlayerCardWidget* Card = PlayerCards[PlayerID];
	if (!Card) return;

	UHorizontalBox* TargetZone = GetZoneByName(ZoneName);
	if (!TargetZone) return;

	if (UWidget* Parent = Card->GetParent())
	{
		if (UPanelWidget* ParentPanel = Cast<UPanelWidget>(Parent))
		{
			ParentPanel->RemoveChild(Card);
		}
	}

	TargetZone->AddChild(Card);

	UE_LOG(LogTemp, Log, TEXT("Player %d moved to zone: %s"), PlayerID, *ZoneName);
}

UHorizontalBox* URobotBattleTeamSelectMenu::GetZoneByName(const FString& Name) const
{
	if (Name.Equals("HomeUp", ESearchCase::IgnoreCase)) return Box_HomeUp;
	if (Name.Equals("HomeDown", ESearchCase::IgnoreCase)) return Box_HomeDown;
	if (Name.Equals("AwayUp", ESearchCase::IgnoreCase)) return Box_AwayUp;
	if (Name.Equals("AwayDown", ESearchCase::IgnoreCase)) return Box_AwayDown;
	if (Name.Equals("CenterUp", ESearchCase::IgnoreCase)) return Box_CenterUp;
	if (Name.Equals("CenterDown", ESearchCase::IgnoreCase)) return Box_CenterDown;

	return nullptr;
}
