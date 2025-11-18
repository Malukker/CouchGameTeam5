// Fill out your copyright notice in the Description page of Project Settings.


#include "Tools/SelectRobot/SelectRobotActor.h"

#include "Kismet/GameplayStatics.h"


// Sets default values
ASelectRobotActor::ASelectRobotActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	static ConstructorHelpers::FClassFinder<USelectRobotRuntime> WidgetBPClass(TEXT("/Game/Tools/Widgets/WBP_SelectRobotRuntime.WBP_SelectRobotRuntime_C"));
	
	if (WidgetBPClass.Succeeded())
	{
		SelectRobotWidget = WidgetBPClass.Class;
	}
}

// Called when the game starts or when spawned
void ASelectRobotActor::BeginPlay()
{
	Super::BeginPlay();
	if (USelectRobotRuntime* Widget = CreateWidget<USelectRobotRuntime>(GetWorld(), SelectRobotWidget))
	{
		WidgetTest = Widget;
		Widget->AddToViewport();
		
		if (APlayerController* PlayerController = Cast<APlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0)))
		{
			PlayerController->SetInputMode(FInputModeUIOnly());
			PlayerController->SetShowMouseCursor(true);
		}
	}
}

// Called every frame
void ASelectRobotActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASelectRobotActor::Destroyed()
{
	Super::Destroyed();
	if (APlayerController* PlayerController = Cast<APlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0)))
	{
		PlayerController->SetInputMode(FInputModeGameOnly());
		PlayerController->SetShowMouseCursor(false);
		WidgetTest->RemoveFromParent();
	}
}

