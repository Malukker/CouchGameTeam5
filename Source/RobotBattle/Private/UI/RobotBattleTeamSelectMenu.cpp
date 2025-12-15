// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/RobotBattleTeamSelectMenu.h"
#include "Characters/MainMenu/PlayerMenuActor.h"
#include "Blueprint/WidgetTree.h"
#include "UI/RobotBattlePlayerCardWidget.h"
#include "Components/HorizontalBox.h"
#include "Components/VerticalBox.h"
#include "Components/CanvasPanel.h"
#include "Components/VerticalBoxSlot.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/Image.h"
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
		TempActor->InputValidateEvent.AddDynamic(this, &URobotBattleTeamSelectMenu::ChangeImageWhenReady);
		TempActor->InputCancelEvent.AddDynamic(this, &URobotBattleTeamSelectMenu::OnPlayerCancelInput);
	}
}

void URobotBattleTeamSelectMenu::NativeConstruct()
{
	Super::NativeConstruct();
	
	PlayerChangedImg.Init(false,4);
	GM = Cast<AMenuGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	
	for (int32 i = 0; i < 4; i++)
	{
		PlayerReadyState.Add(i, false);
	}

	if (IMG_ReadyPlayer1) IMG_ReadyPlayer1->SetVisibility(ESlateVisibility::Hidden);
	if (IMG_ReadyPlayer2) IMG_ReadyPlayer2->SetVisibility(ESlateVisibility::Hidden);
	if (IMG_ReadyPlayer3) IMG_ReadyPlayer3->SetVisibility(ESlateVisibility::Hidden);
	if (IMG_ReadyPlayer4) IMG_ReadyPlayer4->SetVisibility(ESlateVisibility::Hidden);
	if (CanvasPanel_Controls) CanvasPanel_Controls->SetVisibility(ESlateVisibility::Hidden);

	if (IMG_WaitingPlayer1) IMG_WaitingPlayer1->SetVisibility(ESlateVisibility::Visible);
	if (IMG_WaitingPlayer2) IMG_WaitingPlayer2->SetVisibility(ESlateVisibility::Visible);
	if (IMG_WaitingPlayer3) IMG_WaitingPlayer3->SetVisibility(ESlateVisibility::Visible);
	if (IMG_WaitingPlayer4) IMG_WaitingPlayer4->SetVisibility(ESlateVisibility::Visible);
}

void URobotBattleTeamSelectMenu::AddPlayer(int32 PlayerID)
{
	if (!PlayerCardClass)
	{
		UE_LOG(LogTemp, Error, TEXT("PlayerCardClass is NULL!"));
		return;
	}
	if (!Box_Center)
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
	
	NewCard->SetPlayerImage(PlayerID);
	UVerticalBoxSlot* CardCenter;

	CardCenter = Box_Center->AddChildToVerticalBox(NewCard);
	FSlateChildSize Size;
	CardCenter->SetSize(Size);

	PlayerCards.Add(PlayerID, NewCard);

	FString StartZone = "Center";
	CurrentZones.Add(PlayerID, StartZone);

	HasValidatedByPlayer.Add(PlayerID, false);
	
	UE_LOG(LogTemp, Log, TEXT("Added PlayerCard for Player %d"), PlayerID);
	UE_LOG(LogTemp, Log, TEXT("Box_CenterUp Children Count: %d"), Box_Center->GetChildrenCount());
}

void URobotBattleTeamSelectMenu::MovePlayerToZone(int32 PlayerID, const FString& ZoneName)
{
	if (!PlayerCards.Contains(PlayerID)) return;

	URobotBattlePlayerCardWidget* Card = PlayerCards[PlayerID];
	if (!Card) return;

	if (ZoneName.Contains("Center"))
	{
		UVerticalBox* TargetZone = nullptr;
		if (ZoneName.Equals("Center", ESearchCase::IgnoreCase))
		{
			TargetZone = Box_Center;
		}
		if (!TargetZone) return;

		Card->RemoveFromParent();
		UVerticalBoxSlot* CardSlot = TargetZone->AddChildToVerticalBox(Card);
		
		FSlateChildSize Size;
		CardSlot->SetSize(Size);
	}
	else
	{
		UHorizontalBox* TargetZone = GetZoneByName(ZoneName);
		if (!TargetZone) return;

		if (IsZoneOccupied(ZoneName))
		{
			UE_LOG(LogTemp, Warning, TEXT("Zone %s déjà occupée par Player %d. Déplacement annulé."), *ZoneName, PlayerID);
			return;
		}

		Card->RemoveFromParent();
		UHorizontalBoxSlot* CardSlot = TargetZone->AddChildToHorizontalBox(Card);

		FSlateChildSize Size;
		CardSlot->SetSize(Size);
	}
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
	return nullptr;
}

void URobotBattleTeamSelectMenu::ChangeImageWhenReady(APlayerController* controller)
{
	if (!TeamSelectionDone) return;
	switch (controller->GetLocalPlayer()->GetControllerId())
	{
		case 0:
			IMG_Player1->SetBrushFromTexture(IMG_Ready1);
			PlayerChangedImg[0] = true;
			break;
		case 1:
			IMG_Player2->SetBrushFromTexture(IMG_Ready2);
			PlayerChangedImg[1] = true;
			break;
		case 2:
			IMG_Player3->SetBrushFromTexture(IMG_Ready3);
			PlayerChangedImg[2] = true;
			break;
		case 3:
			IMG_Player4->SetBrushFromTexture(IMG_Ready4);
			PlayerChangedImg[3] = true;
			break;
		default:
			break;
	}
	CheckAllBoolAndGoToNextLevel();
}

void URobotBattleTeamSelectMenu::CheckAllBoolAndGoToNextLevel()
{
	for (bool Image : PlayerChangedImg )
	{
		if (!Image) return;
	}
	GM->StartSelectionCharacter();
	RemoveFromParent();
	TArray<AActor*> Players;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerMenuActor::StaticClass(), Players);

	for (AActor* Actor : Players)
	{
		APlayerMenuActor* TempActor = Cast<APlayerMenuActor>(Actor);
		TempActor->InputMoveEvent.RemoveDynamic(this, &URobotBattleTeamSelectMenu::OnPlayerMoveInput);
		TempActor->InputValidateEvent.RemoveDynamic(this, &URobotBattleTeamSelectMenu::OnPlayerValidateInput);
		TempActor->InputValidateEvent.RemoveDynamic(this, &URobotBattleTeamSelectMenu::ChangeImageWhenReady);
		TempActor->InputCancelEvent.RemoveDynamic(this, &URobotBattleTeamSelectMenu::OnPlayerCancelInput);
	}
}

void URobotBattleTeamSelectMenu::OnPlayerMoveInput(FVector2D Direction, APlayerController* Controller)
{
	int32 PlayerID = Controller->GetLocalPlayer()->GetLocalPlayerIndex();
	
	if (HasValidatedByPlayer[PlayerID]) { return; }

	FString* CurrentZonePtr = CurrentZones.Find(PlayerID);
	FString CurrentZone = CurrentZonePtr ? *CurrentZonePtr : TEXT("Center");

	FString NewZone = CurrentZone;

	if (Direction.X > 0)
	{
		if (NewZone.Contains("Home"))
			NewZone = TEXT("Center");
		else if (NewZone.Contains("Center"))
		{
			NewZone = TEXT("Away");
			if (IsZoneOccupied("AwayUp"))
				NewZone.Append(TEXT("Down"));
			else
				NewZone.Append(TEXT("Up"));
		}
	}
	else if (Direction.X < 0)
	{
		if (NewZone.Contains("Away"))
			NewZone = TEXT("Center");
		else if (NewZone.Contains("Center"))
		{
			NewZone = TEXT("Home");
			if (IsZoneOccupied("HomeUp"))
				NewZone.Append(TEXT("Down"));
			else
				NewZone.Append(TEXT("Up"));
		}
	}
	if (Direction.Y > 0)
	{
		if (NewZone.Contains("Down"))
			NewZone = NewZone.Replace(TEXT("Down"), TEXT("Up"));
	}
	else if (Direction.Y < 0)
	{
		if (NewZone.Contains("Up"))
			NewZone = NewZone.Replace(TEXT("Up"), TEXT("Down"));
	}
	
	if (NewZone != CurrentZone)
	{
		MovePlayerToZone(PlayerID, NewZone);
		if (IterationSoundTeamSelection.Num() > 0)
		{
			int32 RandomIndex = FMath::RandRange(0, IterationSoundTeamSelection.Num() - 1);
			UGameplayStatics::PlaySound2D(this, IterationSoundTeamSelection[RandomIndex]);
		}
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
	UGameplayStatics::PlaySound2D(this,ValidateSound);
	int32 PlayerID = Controller->GetLocalPlayer()->GetLocalPlayerIndex();

	if (CurrentZones[PlayerID] == "CenterUp" || CurrentZones[PlayerID] == "CenterDown")
	{
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("Validate input from %s"), *Controller->GetName())
	HasValidatedByPlayer[PlayerID] = true;

	FString Zone = CurrentZones[PlayerID];

	bool bZoneReady = true;
	for (const auto& Pair : CurrentZones)
	{
		if (Pair.Value == Zone && !HasValidatedByPlayer[Pair.Key])
		{
			bZoneReady = false;
			break;
		}
	}

	if (Zone == "HomeUp") SetBoxReady(ETeamBox::HomeUp, bZoneReady);
	else if (Zone == "HomeDown") SetBoxReady(ETeamBox::HomeDown, bZoneReady);
	else if (Zone == "AwayUp") SetBoxReady(ETeamBox::AwayUp, bZoneReady);
	else if (Zone == "AwayDown") SetBoxReady(ETeamBox::AwayDown, bZoneReady);
	
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
	
	if (CanvasPanel_TeamSelection) CanvasPanel_TeamSelection->SetVisibility(ESlateVisibility::Hidden);
	if (CanvasPanel_Controls) CanvasPanel_Controls->SetVisibility(ESlateVisibility::Visible);
	FTimerHandle TimerHandle;
	 GetWorld()->GetTimerManager().SetTimer(TimerHandle, [&]()
	 {
	 	TeamSelectionDone = true;
	 },0.1f , false);
}

void URobotBattleTeamSelectMenu::OnPlayerCancelInput(APlayerController* Controller)
{
	int32 PlayerID = Controller->GetLocalPlayer()->GetLocalPlayerIndex();
	HasValidatedByPlayer[PlayerID] = false;

	FString Zone = CurrentZones[PlayerID];

	bool bZoneReady = false;
	for (const auto& Pair : CurrentZones)
	{
		if (Pair.Value == Zone && HasValidatedByPlayer[Pair.Key])
		{
			bZoneReady = true;
			break;
		}
	}

	if (Zone == "HomeUp") SetBoxReady(ETeamBox::HomeUp, bZoneReady);
	else if (Zone == "HomeDown") SetBoxReady(ETeamBox::HomeDown, bZoneReady);
	else if (Zone == "AwayUp") SetBoxReady(ETeamBox::AwayUp, bZoneReady);
	else if (Zone == "AwayDown") SetBoxReady(ETeamBox::AwayDown, bZoneReady);


	UE_LOG(LogTemp, Log, TEXT("Cancel input from %s"), *Controller->GetName())
}

void URobotBattleTeamSelectMenu::SetBoxReady(ETeamBox Box, bool bIsReady)
{
	switch (Box)
	{
	case ETeamBox::HomeUp:
		IMG_ReadyPlayer1->SetVisibility(bIsReady ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
		IMG_WaitingPlayer1->SetVisibility(bIsReady ? ESlateVisibility::Hidden : ESlateVisibility::Visible);
		break;

	case ETeamBox::HomeDown:
		IMG_ReadyPlayer2->SetVisibility(bIsReady ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
		IMG_WaitingPlayer2->SetVisibility(bIsReady ? ESlateVisibility::Hidden : ESlateVisibility::Visible);
		break;

	case ETeamBox::AwayUp:
		IMG_ReadyPlayer3->SetVisibility(bIsReady ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
		IMG_WaitingPlayer3->SetVisibility(bIsReady ? ESlateVisibility::Hidden : ESlateVisibility::Visible);
		break;

	case ETeamBox::AwayDown:
		IMG_ReadyPlayer4->SetVisibility(bIsReady ? ESlateVisibility::Visible : ESlateVisibility::Hidden);
		IMG_WaitingPlayer4->SetVisibility(bIsReady ? ESlateVisibility::Hidden : ESlateVisibility::Visible);
		break;
	}
}
