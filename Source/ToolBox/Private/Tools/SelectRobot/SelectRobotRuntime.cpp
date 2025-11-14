// Fill out your copyright notice in the Description page of Project Settings.


#include "Tools/SelectRobot/SelectRobotRuntime.h"

#include "Kismet/GameplayStatics.h"
#include "Match/RobotGameInstance.h"
#include "Tools/SelectRobot/SelectRobotActor.h"

void USelectRobotRuntime::NativeConstruct()
{
	Super::NativeConstruct();
	if (URobotGameInstance* GI = Cast<URobotGameInstance>(UGameplayStatics::GetGameInstance(GetWorld())))
	{
		GI->OnInputDeviceConnectionChange.AddDynamic(this,&USelectRobotRuntime::SetControllers);
	}
	
}

void USelectRobotRuntime::SetControllers(EInputDeviceConnectionState NewConnectionState, FPlatformUserId PlatformUserId,
	FInputDeviceId InputDeviceId)
{
	if (NewConnectionState == EInputDeviceConnectionState::Connected)
	{
		Controllers.Add(InputDeviceId.GetId());
		return;
	}else
	{
		Controllers.Remove(InputDeviceId.GetId());
	}
	
}

void USelectRobotRuntime::DestroyActor()
{
	if (AActor* ShakeActor =UGameplayStatics::GetActorOfClass(GetWorld(),ASelectRobotActor::StaticClass()))
	{
		ShakeActor->Destroy();
	}
}


