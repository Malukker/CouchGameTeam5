// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/States/RobotCharacterStateLoadingAttack.h"


// Sets default values for this component's properties
URobotCharacterStateLoadingAttack::URobotCharacterStateLoadingAttack()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void URobotCharacterStateLoadingAttack::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void URobotCharacterStateLoadingAttack::TickComponent(float DeltaTime, ELevelTick TickType,
                                                      FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void URobotCharacterStateLoadingAttack::StateInit(URobotCharacterStateMachine* InStateMachine)
{
	Super::StateInit(InStateMachine);
}

ERobotCharacterStateID URobotCharacterStateLoadingAttack::GetStateID()
{
	return Super::GetStateID();
}

void URobotCharacterStateLoadingAttack::StateEnter(ERobotCharacterStateID PreviousStateID)
{
	Super::StateEnter(PreviousStateID);
}

void URobotCharacterStateLoadingAttack::StateExit(ERobotCharacterStateID NextState)
{
	Super::StateExit(NextState);
}

void URobotCharacterStateLoadingAttack::StateTick(float DeltaTime)
{
	Super::StateTick(DeltaTime);
}

