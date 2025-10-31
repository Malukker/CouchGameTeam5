// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerMenuActor.h"

#include "EnhancedInputComponent.h"

void APlayerMenuActor::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (!PlayerInputComponent) { return; }

	BindActions(Cast<UEnhancedInputComponent>(PlayerInputComponent));
}

void APlayerMenuActor::BindActions(UEnhancedInputComponent* EnhancedInputComponent)
{
	if (InputActionMoveInMenu)
	{
		EnhancedInputComponent->BindAction(
			InputActionMoveInMenu,
			ETriggerEvent::Triggered,
			this,
			&APlayerMenuActor::OnInputMoveRecord
			);
		
		EnhancedInputComponent->BindAction(
			InputActionMoveInMenu,
			ETriggerEvent::Completed,
			this,
			&APlayerMenuActor::OnInputMoveMenu
		);
	}

	if (InputActionValidate)
	{
		EnhancedInputComponent->BindAction(
			InputActionValidate,
			ETriggerEvent::Started,
			this,
			&APlayerMenuActor::OnInputValidate
		);
	}

	if (InputActionCancel)
	{
		EnhancedInputComponent->BindAction(
			InputActionCancel,
			ETriggerEvent::Started,
			this,
			&APlayerMenuActor::OnInputCancel
		);
	}
}

void APlayerMenuActor::OnInputMoveRecord(const FInputActionValue& InputActionValue)
{
	FVector2D Vector = InputActionValue.Get<FVector2D>();
	if (Vector.Length() > .2f)
	
	if (abs(Vector.X) > abs(Vector.Y))
	{
		if (FMath::Sign(Vector.X) > 0)
		{
			Direction = EPlayerMenuInputDirection::Right;
		}
		else 
		{
			Direction = EPlayerMenuInputDirection::Left;
		}
	}
	else
	{
		if (FMath::Sign(Vector.X) > 0)
		{
			Direction = EPlayerMenuInputDirection::Up;
		}
		else 
		{
			Direction = EPlayerMenuInputDirection::Down;
		}
	}
}

void APlayerMenuActor::OnInputMoveMenu(const FInputActionValue& InputActionValue)
{
	InputMoveEvent.Broadcast(Direction, SelfPlayerController);
}

void APlayerMenuActor::OnInputValidate(const FInputActionValue& InputActionValue)
{
	InputValidateEvent.Broadcast(SelfPlayerController);
}

void APlayerMenuActor::OnInputCancel(const FInputActionValue& InputActionValue)
{
	InputCancelEvent.Broadcast(SelfPlayerController);
}
