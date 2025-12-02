// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "SoundListData.generated.h"

/**
 * 
 */
UCLASS()
class ROBOTBATTLE_API USoundListData : public UDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	TArray<USoundBase*> Sounds;
};
