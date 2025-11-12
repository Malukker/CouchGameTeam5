// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HUDGameplay.h"

#include "Blueprint/UserWidget.h"

TSharedPtr<SWidget> AHUDGameplay::SpawnUIPause()
{
	if (!UIPause) return nullptr;
	UIPause->AddToViewport();
	return TSharedPtr<SWidget>(UIPause->TakeWidget());
}

void AHUDGameplay::BeginPlay()
{
	Super::BeginPlay();
	UIPause = CreateWidget<UUserWidget>(GetWorld(), UIPauseClass);
	UIPause->SetOwningPlayer(GetOwningPlayerController());
	SetTickableWhenPaused(true);
}
