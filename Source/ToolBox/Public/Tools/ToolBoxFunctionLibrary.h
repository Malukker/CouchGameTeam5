// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ToolBoxFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class TOOLBOX_API UToolBoxFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

	public:
	UFUNCTION(BlueprintCallable,Category="ToolBoxFunctionLibrary")
	static void SlateNotification(const FText& NotificationText, bool IsSuccess,const FText& SubText = FText());
	
};
