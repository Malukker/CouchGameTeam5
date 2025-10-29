// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/RobotBattleTeamSelectMenu.h"
#include "UI/RobotBattlePlayerCardWidget.h"
#include "Components/HorizontalBox.h"
#include "Components/PanelWidget.h"

void URobotBattleTeamSelectMenu::NativeConstruct()
{
	Super::NativeConstruct();

	for (int32 i = 0; i < 4; i++)
	{
		AddPlayer(i);
	}
}

void URobotBattleTeamSelectMenu::AddPlayer(int32 PlayerID)
{
	if (!PlayerCardClass) return;
	if (!Box_CenterUp || !Box_CenterDown) return;

	URobotBattlePlayerCardWidget* NewCard = CreateWidget<URobotBattlePlayerCardWidget>(GetWorld(), PlayerCardClass);
	if (!NewCard) return;

	NewCard->SetPlayerName(FString::Printf(TEXT("Player %d"), PlayerID + 1));

	if (PlayerID % 2 == 0)
		Box_CenterUp->AddChild(NewCard);
	else
		Box_CenterDown->AddChild(NewCard);

	PlayerCards.Add(PlayerID, NewCard);
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

UHorizontalBox* URobotBattleTeamSelectMenu::GetZoneByName(const FString& Name)
{
	if (Name.Equals("HomeUp", ESearchCase::IgnoreCase)) return Box_HomeUp;
	if (Name.Equals("HomeDown", ESearchCase::IgnoreCase)) return Box_HomeDown;
	if (Name.Equals("AwayUp", ESearchCase::IgnoreCase)) return Box_AwayUp;
	if (Name.Equals("AwayDown", ESearchCase::IgnoreCase)) return Box_AwayDown;
	if (Name.Equals("CenterUp", ESearchCase::IgnoreCase)) return Box_CenterUp;
	if (Name.Equals("CenterDown", ESearchCase::IgnoreCase)) return Box_CenterDown;

	return nullptr;
}
