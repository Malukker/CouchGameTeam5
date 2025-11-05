// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/RobotBattleTeamSelectMenu.h"

#include "Characters/MainMenu/PlayerMenuActor.h"
#include "GameFramework/PlayerController.h"
#include "Blueprint/WidgetTree.h"
#include "UI/RobotBattlePlayerCardWidget.h"
#include "Components/HorizontalBox.h"
#include "Components/PanelWidget.h"
#include "GameFramework/GameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Match/RobotGameInstance.h"
#include "UI/MenuGameMode.h"

void URobotBattleTeamSelectMenu::CustomConstruct()
{
	for (int32 i = 0; i < 4; i++)
	{
		AddPlayer(i);
	}
	
	TArray<AActor*> Players;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerMenuActor::StaticClass(), Players);

	for (AActor* Actor : Players)
	{
		APlayerMenuActor* TempActor = Cast<APlayerMenuActor>(Actor);
		TempActor->InputMoveEvent.AddDynamic(this, &URobotBattleTeamSelectMenu::OnPlayerMoveInput);
		TempActor->InputValidateEvent.AddDynamic(this, &URobotBattleTeamSelectMenu::OnPlayerValidateInput);
		TempActor->InputCancelEvent.AddDynamic(this, &URobotBattleTeamSelectMenu::OnPlayerCancelInput);
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

	FString StartZone = (PlayerID % 2 == 0) ? "CenterUp" : "CenterDown";
	CurrentZones.Add(PlayerID, StartZone);

	HasValidatedByPlayer.Add(PlayerID, false);
	
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

	if (ZoneName != "CenterUp" && ZoneName != "CenterDown" && IsZoneOccupied(ZoneName))
	{
		UE_LOG(LogTemp, Warning, TEXT("Zone %s déjà occupée par Player %d. Déplacement annulé."), *ZoneName, PlayerID);
		return;
	}

	Card->RemoveFromParent();
	TargetZone->AddChild(Card);
	CurrentZones[PlayerID] = ZoneName;

	UE_LOG(LogTemp, Log, TEXT("Player %d moved to zone: %s"), PlayerID, *ZoneName);
}

bool URobotBattleTeamSelectMenu::IsZoneOccupied(const FString& ZoneName) const
{
	for (const TPair<int32, FString>& Pair : CurrentZones)
	{
		if (Pair.Value.Equals(ZoneName, ESearchCase::IgnoreCase))
		{
			UE_LOG(LogTemp, Warning, TEXT("Zone %s déjà occupée par Player %d"), *ZoneName, Pair.Key);
			return true;
		}
	}
	return false;
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

void URobotBattleTeamSelectMenu::OnPlayerMoveInput(EPlayerMenuInputDirection Direction, APlayerController* Controller)
{
	UE_LOG(LogTemp, Log, TEXT("Move input: %d"), (int32)Direction);

	int32 PlayerID = Controller->GetLocalPlayer()->GetLocalPlayerIndex();
	
	if (HasValidatedByPlayer[PlayerID]) { return; }

	FString* CurrentZonePtr = CurrentZones.Find(PlayerID);
	FString CurrentZone = CurrentZonePtr ? *CurrentZonePtr : TEXT("CenterUp");

	FString NewZone = CurrentZone;

	if (Direction == EPlayerMenuInputDirection::Up)
	{
		if (CurrentZone.Contains("Down"))
			NewZone = CurrentZone.Replace(TEXT("Down"), TEXT("Up"));
	}
	else if (Direction == EPlayerMenuInputDirection::Down)
	{
		if (CurrentZone.Contains("Up"))
			NewZone = CurrentZone.Replace(TEXT("Up"), TEXT("Down"));
	}
	else if (Direction == EPlayerMenuInputDirection::Left)
	{
		if (CurrentZone.Contains("Away"))
			NewZone = CurrentZone.Replace(TEXT("Away"), TEXT("Center"));
		else if (CurrentZone.Contains("Center"))
			NewZone = CurrentZone.Replace(TEXT("Center"), TEXT("Home"));
	}
	else if (Direction == EPlayerMenuInputDirection::Right)
	{
		if (CurrentZone.Contains("Home"))
			NewZone = CurrentZone.Replace(TEXT("Home"), TEXT("Center"));
		else if (CurrentZone.Contains("Center"))
			NewZone = CurrentZone.Replace(TEXT("Center"), TEXT("Away"));
	}
	
	if (NewZone != CurrentZone)
	{
		MovePlayerToZone(PlayerID, NewZone);
		UE_LOG(LogTemp, Log, TEXT("Player %d moved from %s to %s"), PlayerID, *CurrentZone, *NewZone);
	}
}

int32 URobotBattleTeamSelectMenu::GetControllerIndexForZone(const FString& ZoneName)
{
	if (CurrentZones.FindKey(ZoneName) == nullptr) return -1;
	return *CurrentZones.FindKey(ZoneName);
}

void URobotBattleTeamSelectMenu::OnPlayerValidateInput(APlayerController* Controller)
{
	int32 PlayerID = Controller->GetLocalPlayer()->GetLocalPlayerIndex();

	if (CurrentZones[PlayerID] == "CenterUp" || CurrentZones[PlayerID] == "CenterDown")
	{
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("Validate input from %s"), *Controller->GetName())
	HasValidatedByPlayer[PlayerID] = true;

	for (const auto Pair : HasValidatedByPlayer)
	{
		if (!Pair.Value)
		{
			return;
		} 
	}

	URobotGameInstance* GI = Cast<URobotGameInstance>(GetGameInstance());
	GI->SetPlayerPos(0, GetControllerIndexForZone("HomeDown"));
	GI->SetPlayerPos(1, GetControllerIndexForZone("HomeUp"));
	GI->SetPlayerPos(2, GetControllerIndexForZone("AwayDown"));
	GI->SetPlayerPos(3, GetControllerIndexForZone("AwayUp"));
	UE_LOG(LogTemp, Log, TEXT("Selection Team Finish !"))

	AMenuGameMode* GM = Cast<AMenuGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	RemoveFromParent();
	GM->StartSelectionCharacter();
}

void URobotBattleTeamSelectMenu::OnPlayerCancelInput(APlayerController* Controller)
{
	int32 PlayerID = Controller->GetLocalPlayer()->GetLocalPlayerIndex();
	HasValidatedByPlayer[PlayerID] = false;

	UE_LOG(LogTemp, Log, TEXT("Cancel input from %s"), *Controller->GetName())
}