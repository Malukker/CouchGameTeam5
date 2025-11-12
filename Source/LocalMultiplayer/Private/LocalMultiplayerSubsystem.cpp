// Fill out your copyright notice in the Description page of Project Settings.


#include "LocalMultiplayerSubsystem.h"
#include "Kismet/GameplayStatics.h"

void ULocalMultiplayerSubsystem::CreatePlayers() {
	for (int i = 0; i < 4; i++)
	{
		if (Controllers.Num() <= i && !UGameplayStatics::GetPlayerController(GetWorld(), i))
		{
			Controllers.Add(UGameplayStatics::CreatePlayer(GetWorld(), i));
		}
		else if (Controllers.Num() <= i) 
		{
			Controllers.Add(UGameplayStatics::GetPlayerController(GetWorld(), i));
		}
	}
}