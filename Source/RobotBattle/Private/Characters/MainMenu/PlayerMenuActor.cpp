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
	if (InputActionMove)
	{		
		EnhancedInputComponent->BindAction(
			InputActionMove,
			ETriggerEvent::Triggered,
			this,
			&APlayerMenuActor::OnInputMove
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

void APlayerMenuActor::OnInputMove(const FInputActionValue& InputActionValue)
{
	InputMoveEvent.Broadcast(InputActionValue.Get<FVector2D>(), SelfPlayerController);
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
	OnInput.Broadcast();
}
