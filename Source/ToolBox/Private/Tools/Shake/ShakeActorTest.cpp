// Fill out your copyright notice in the Description page of Project Settings.


#include "Tools/Shake/ShakeActorTest.h"

#include "HeadMountedDisplayTypes.h"
#include "Blueprint/UserWidget.h"
#include "Blueprint/WidgetBlueprintLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Tools/Shake/ShakeCameraTestRuntime.h"


// Sets default values
AShakeActorTest::AShakeActorTest()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AShakeActorTest::BeginPlay()
{
	Super::BeginPlay();
	if (UShakeCameraTestRuntime* Widget = CreateWidget<UShakeCameraTestRuntime>(GetWorld(), ShakeTestWidget))
	{
		Widget->AddToViewport();
		if (APlayerController* PlayerController = Cast<APlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0)))
		{
			PlayerController->SetInputMode(FInputModeUIOnly());
			PlayerController->SetShowMouseCursor(true);
		}
	}
}

// Called every frame
void AShakeActorTest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

