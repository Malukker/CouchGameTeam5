// Fill out your copyright notice in the Description page of Project Settings.


#include "Tools/SlowMotion/SlowMotionTestRuntime.h"

#include "Kismet/GameplayStatics.h"
#include "Tools/ToolBoxFunctionLibrary.h"

void USlowMotionTestRuntime::SetSlowMotion()
{
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(),Scale);
	UToolBoxFunctionLibrary::SlateNotification(FText::FromString("SlowMotion  Start"),true);
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [&]()
	{
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1);
		UToolBoxFunctionLibrary::SlateNotification(FText::FromString("SlowMotion  End"),true);
	}, Duration/(Scale * 10), false);
}

void USlowMotionTestRuntime::SaveSlowMotion()
{
	
}
