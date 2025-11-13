// Fill out your copyright notice in the Description page of Project Settings.


#include "Tools/SlowMotion/SlowMotionActorTest.h"

#include "Kismet/GameplayStatics.h"
#include "Tools/ToolBoxFunctionLibrary.h"
#include "Tools/SlowMotion/SlowMotionTestRuntime.h"


// Sets default values
ASlowMotionActorTest::ASlowMotionActorTest()
{
	PrimaryActorTick.bCanEverTick = true;
	static ConstructorHelpers::FClassFinder<USlowMotionTestRuntime> WidgetBPClass(TEXT("/Game/Tools/Widgets/WBP_SlowMotionTestRuntime.WBP_SlowMotionTestRuntime_C"));
	
	if (WidgetBPClass.Succeeded())
	{
		ShakeTestWidget = WidgetBPClass.Class;
	}
}


void ASlowMotionActorTest::OnEventStartSlowMotion(float Scale, float Duration)
{
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(),Scale);
	UToolBoxFunctionLibrary::SlateNotification(FText::FromString("SlowMotion  Start"),true);
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [&]()
	{
		
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1);
		UToolBoxFunctionLibrary::SlateNotification(FText::FromString("SlowMotion  End"),true);
	}, Duration/Scale, false);
}

void ASlowMotionActorTest::Destroyed()
{
	Super::Destroyed();
	StartSlowMotion.Unbind();
	if (APlayerController* PlayerController = Cast<APlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0)))
	{
		PlayerController->SetInputMode(FInputModeGameOnly());
		PlayerController->SetShowMouseCursor(false);
		WidgetTest->RemoveFromParent();
	}
}


void ASlowMotionActorTest::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	
}

// Called when the game starts or when spawned
void ASlowMotionActorTest::BeginPlay()
{
	Super::BeginPlay();
	StartSlowMotion.BindUObject(this,&ASlowMotionActorTest::OnEventStartSlowMotion);
	if (USlowMotionTestRuntime* Widget = CreateWidget<USlowMotionTestRuntime>(GetWorld(), ShakeTestWidget))
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




