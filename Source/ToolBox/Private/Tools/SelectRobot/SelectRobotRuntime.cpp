// Fill out your copyright notice in the Description page of Project Settings.


#include "Tools/SelectRobot/SelectRobotRuntime.h"

#include <string>

#include "EnhancedInputSubsystems.h"
#include "Kismet/GameplayStatics.h"
#include "Match/RobotGameInstance.h"
#include "Tools/ToolBoxFunctionLibrary.h"
#include "Tools/SelectRobot/SelectRobotActor.h"

void USelectRobotRuntime::NativeConstruct()
{
	Super::NativeConstruct();
	if ( (GI = Cast<URobotGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()))))
	{
		//GI->OnInputDeviceConnectionChange.AddDynamic(this,&USelectRobotRuntime::SetControllers);
	}
	ControllerStateChange.BindUObject(this,&USelectRobotRuntime::UpdateControllerComboBox);
}

void USelectRobotRuntime::SetControllers(EInputDeviceConnectionState NewConnectionState, FPlatformUserId PlatformUserId,
	FInputDeviceId InputDeviceId)
{
	if (NewConnectionState == EInputDeviceConnectionState::Connected)
	{
		Controllers.Add(InputDeviceId.GetId());
	}else
	{
		Controllers.Remove(InputDeviceId.GetId());
	}
	OnInputChange(InputDeviceId.GetId(),NewConnectionState);
}

void USelectRobotRuntime::DestroyActor()
{
	if (AActor* ShakeActor =UGameplayStatics::GetActorOfClass(GetWorld(),ASelectRobotActor::StaticClass()))
	{
		ShakeActor->Destroy();
	}
}

void USelectRobotRuntime::ChangeRobotController(int PlayerIndex, int Controller)
{
	ULocalPlayer* LP = GI->GetLocalPlayerByIndex(PlayerIndex);
	LP->SetControllerId(Controller);

	UToolBoxFunctionLibrary::SlateNotification(
		FText::FromString("Controller Reassigned !"),
		true
	);
	
}

void USelectRobotRuntime::UpdateControllerComboBox(EInputDeviceConnectionState NewConnectionState,
	FInputDeviceId InputDeviceId)
{
	if (NewConnectionState == EInputDeviceConnectionState::Connected)
	{
		ComboBox_Controller->AddOption(std::to_string(InputDeviceId.GetId()).data());
	}
	if (NewConnectionState == EInputDeviceConnectionState::Disconnected)
	{
		ComboBox_Controller->RemoveOption(std::to_string(InputDeviceId.GetId()).data());
	}
}




