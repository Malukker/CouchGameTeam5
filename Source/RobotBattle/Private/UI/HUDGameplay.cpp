// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUDGameplay.h"

#include "Blueprint/UserWidget.h"

void AHUDGameplay::SpawnUIPause()
{
	if (!UIPause) return;
	UIPause->AddToViewport();
}

void AHUDGameplay::BeginPlay()
{
	Super::BeginPlay();
	UIPause = CreateWidget<UUserWidget>(GetWorld(), UIPauseClass);
	UIPause->SetOwningPlayer(GetOwningPlayerController());
	SetTickableWhenPaused(true);
}
