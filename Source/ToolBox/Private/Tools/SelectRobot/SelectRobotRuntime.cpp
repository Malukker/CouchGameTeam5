// Fill out your copyright notice in the Description page of Project Settings.


#include "Tools/SelectRobot/SelectRobotRuntime.h"
#include "GenericPlatform/GenericPlatformInputDeviceMapper.h"
#include <string>
#include "EnhancedInputSubsystems.h"
#include "GameFramework/InputDeviceSubsystem.h"
#include "Kismet/GameplayStatics.h"
#include "Match/RobotGameInstance.h"
#include "Tools/ToolBoxFunctionLibrary.h"
#include "Tools/SelectRobot/SelectRobotActor.h"

void USelectRobotRuntime::NativeConstruct()
{
	Super::NativeConstruct();
	GI = Cast<URobotGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	const IPlatformInputDeviceMapper& Mapper = IPlatformInputDeviceMapper::Get();
	
	Mapper.GetOnInputDeviceConnectionChange().AddUObject(this,&USelectRobotRuntime::UpdateControllerComboBox);
	
	//Put in the combobox UI all the connected devices
	TArray<FInputDeviceId> OutDevices;
	Mapper.GetAllConnectedInputDevices(OutDevices);
	for (auto Device : OutDevices)
	{
		ComboBox_Controller->AddOption(std::to_string(Device.GetId()).data());
	}
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
		FText::FromString("Controller : " + FString::FromInt(Controller) + " Reassigned to player "+FString::FromInt(PlayerIndex)),
		true
	);
	
}

void USelectRobotRuntime::UpdateControllerComboBox(EInputDeviceConnectionState NewConnectionState,FPlatformUserId PlateformUserID,
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








