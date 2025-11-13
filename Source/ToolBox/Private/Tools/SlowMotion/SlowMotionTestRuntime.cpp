// Fill out your copyright notice in the Description page of Project Settings.


#include "Tools/SlowMotion/SlowMotionTestRuntime.h"

#include "MaterialHLSLTree.h"
#include "Kismet/GameplayStatics.h"
#include "Tools/ToolBoxFunctionLibrary.h"
#include "Tools/SlowMotion/SlowMotionActorTest.h"

void USlowMotionTestRuntime::SetSlowMotion()
{
	ASlowMotionActorTest* Actor = Cast<ASlowMotionActorTest>(
		UGameplayStatics::GetActorOfClass(GetWorld(), ASlowMotionActorTest::StaticClass()));
	Actor->StartSlowMotion.Execute(Scale,Duration);
}

void USlowMotionTestRuntime::SaveSlowMotion()
{
	
}

void USlowMotionTestRuntime::DestroyActor()
{
	if (AActor* ShakeActor =UGameplayStatics::GetActorOfClass(GetWorld(),ASlowMotionActorTest::StaticClass()))
	{
		ShakeActor->Destroy();
	}
}

void USlowMotionTestRuntime::Tick(float DeltaTime)
{

}
