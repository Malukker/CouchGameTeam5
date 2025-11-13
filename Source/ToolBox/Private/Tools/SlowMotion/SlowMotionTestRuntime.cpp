// Fill out your copyright notice in the Description page of Project Settings.


#include "Tools/SlowMotion/SlowMotionTestRuntime.h"

#include "Kismet/GameplayStatics.h"

void USlowMotionTestRuntime::SetSlowMotion()
{
	UGameplayStatics::SetGlobalTimeDilation(GetWorld(),Scale);
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [&]()
	{
		UGameplayStatics::SetGlobalTimeDilation(GetWorld(), 1);
	}, Duration/(Scale * 10), false);
}

void USlowMotionTestRuntime::SaveSlowMotion()
{
	
}
