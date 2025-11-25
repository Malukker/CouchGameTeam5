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
	virtual void SetChargePlayer(int Team, float Charge, float MaxCharge) = 0;

	virtual void SetHealthPlayer(int Team, float Health, float MaxHealth) = 0;

	virtual void SetRoundPlayer(int Team, int Win) = 0;

	virtual void SetComboHit(int Team, int Combo) = 0;
};
