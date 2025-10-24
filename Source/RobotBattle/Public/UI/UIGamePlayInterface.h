// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "UIGamePlayInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UUIGamePlayInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class ROBOTBATTLE_API IUIGamePlayInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual int SetChargePlayer(int team) = 0;

	virtual int SetHealthPlayer(int team) = 0;
	
};
