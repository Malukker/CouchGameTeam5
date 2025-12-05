// Fill out your copyright notice in the Description page of Project Settings.


#include "Match/RobotGameInstance.h"
#include "Characters/Attacks/AttackStruct.h"
#include "Kismet/GameplayStatics.h"


void URobotGameInstance::SetChangeCharacter(bool value)
{
	ChangeCharacter = value;
}

ERobotID URobotGameInstance::GetRobotID(int Number)
{
	return RobotID[Number];
}

void URobotGameInstance::SetRobotID(int Number, ERobotID ID)
{
	RobotID[Number] = ID;
}

uint8 URobotGameInstance::GetPlayerPos(int Number)
{
	return PlayersPos[Number];
}

void URobotGameInstance::SetPlayerPos(int Number, uint8 Pos)
{
	PlayersPos[Number] = Pos;
}
//
// void URobotGameInstance::Init()
// {
// 	Super::Init();
// 	OnInputDeviceConnectionChange.AddDynamic(this,&URobotGameInstance::AddControllerOnList);
// }
//
// void URobotGameInstance::AddControllerOnList(EInputDeviceConnectionState NewConnectionState,
// 	FPlatformUserId PlatformUserId, FInputDeviceId InputDeviceId)
// {
// 	if (NewConnectionState == EInputDeviceConnectionState::Connected)
// 	{
// 		Controllers.Add(InputDeviceId.GetId());
// 	}else
// 	{
// 		Controllers.Remove(InputDeviceId.GetId());
// 	}
// 	//OnInputChange(InputDeviceId.GetId(),NewConnectionState);
// 	if (ASelectRobotActor* Actor = Cast<ASelectRobotActor>(UGameplayStatics::GetActorOfClass(GetWorld(), ASelectRobotActor::StaticClass())))
// 	{
// 		Actor->WidgetTest->ControllerStateChange.Execute(NewConnectionState,InputDeviceId);
// 	}
// }