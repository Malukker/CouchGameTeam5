// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/MainMenu/PlayerMenuActor.h"

#include "EnhancedInputComponent.h"

void APlayerMenuActor::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (!PlayerInputComponent) { return; }

	BindActions(Cast<UEnhancedInputComponent>(PlayerInputComponent));
}

void APlayerMenuActor::BeginPlay()
{
	Super::BeginPlay();
	SelfPlayerController = Cast<APlayerController>(Controller);
}

void APlayerMenuActor::BindActions(UEnhancedInputComponent* EnhancedInputComponent)
{
	if (InputActionMoveUp)
	{		
		EnhancedInputComponent->BindAction(
			InputActionMoveUp,
			ETriggerEvent::Started,
			this,
			&APlayerMenuActor::OnInputMoveUp
		);
	}

	if (InputActionMoveDown)
	{		
		EnhancedInputComponent->BindAction(
			InputActionMoveDown,
			ETriggerEvent::Started,
			this,
			&APlayerMenuActor::OnInputMoveDown
		);
	}

	if (InputActionMoveLeft)
	{		
		EnhancedInputComponent->BindAction(
			InputActionMoveLeft,
			ETriggerEvent::Started,
			this,
			&APlayerMenuActor::OnInputMoveLeft
		);
	}

	if (InputActionMoveRight)
	{		
		EnhancedInputComponent->BindAction(
			InputActionMoveRight,
			ETriggerEvent::Started,
			this,
			&APlayerMenuActor::OnInputMoveRight
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

	if (InputAnyKey)
	{
		EnhancedInputComponent->BindAction(
			InputAnyKey,
			ETriggerEvent::Started,
			this,
			&APlayerMenuActor::OnInputAnyKey
		);
	}
}

void APlayerMenuActor::OnInputMoveUp(const FInputActionValue& InputActionValue)
{
	OnInputMoveMenu(EPlayerMenuInputDirection::Up);
}

void APlayerMenuActor::OnInputMoveDown(const FInputActionValue& InputActionValue)
{
	OnInputMoveMenu(EPlayerMenuInputDirection::Down);
}

void APlayerMenuActor::OnInputMoveLeft(const FInputActionValue& InputActionValue)
{
	OnInputMoveMenu(EPlayerMenuInputDirection::Left);
}

void APlayerMenuActor::OnInputMoveRight(const FInputActionValue& InputActionValue)
{
	OnInputMoveMenu(EPlayerMenuInputDirection::Right);
}

void APlayerMenuActor::OnInputMoveMenu(EPlayerMenuInputDirection Direction)
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

void APlayerMenuActor::OnInputAnyKey(const FInputActionValue& InputActionValue)
{
	GEngine->AddOnScreenDebugMessage(
		0,
		5.f,
		FColor::Red,
		TEXT("HELLO"));
	OnInput.Broadcast();
}
