// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/RobotCharacter.h"
#include "Characters/RobotCharacterStateMachine.h"
#include <EnhancedInputSubsystems.h>
#include "EnhancedInputComponent.h"
#include "Characters/RobotCharacterPositionEnum.h"

// Sets default values
ARobotCharacter::ARobotCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

void ARobotCharacter::TakeDamageFromAttack(int Damage, float StunTime)
{
	HurtManagerEvent.Broadcast(Damage, StunTime);
	//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Blue, TEXT("DAMAGE!!"));
}

// Called when the game starts or when spawned
void ARobotCharacter::BeginPlay()
{
	Super::BeginPlay();
	CreateStateMachine();

	InitStateMachine();
}

// Called every frame
void ARobotCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	TickStateMachine(DeltaTime);
	RotateMeshUsingOrientX();
}

// Called to bind functionality to input
void ARobotCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	SetupMappingContextIntoController();

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (EnhancedInputComponent == nullptr) return;

	BindInputAndActions(EnhancedInputComponent);
}

float ARobotCharacter::GetOrientX() const
{
	return OrientX;
}

void ARobotCharacter::SetOrientX(float NewOrientX) 
{
	OrientX = NewOrientX;
}

void ARobotCharacter::RotateMeshUsingOrientX() const
{
	FRotator Rotation = GetMesh()->GetRelativeRotation();
	Rotation.Yaw = -90.f * OrientX;
	GetMesh()->SetRelativeRotation(Rotation);
}

void ARobotCharacter::CreateStateMachine() {
	StateMachine = NewObject<URobotCharacterStateMachine>(this);
}

void ARobotCharacter::InitStateMachine() {
	if (StateMachine == nullptr) return;
	StateMachine->Init(this);
}

void ARobotCharacter::TickStateMachine(float DeltaTime) const {
	if (StateMachine == nullptr) return;
	StateMachine->Tick(DeltaTime);
}

void ARobotCharacter::SetupMappingContextIntoController() const {
	APlayerController* PlayerController = Cast<APlayerController>(Controller);
	if (PlayerController == nullptr) return;

	ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer();
	if (LocalPlayer == nullptr)return;

	UEnhancedInputLocalPlayerSubsystem* InputSystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	if (InputSystem == nullptr)return;

	InputSystem->AddMappingContext(InputMappingContext, 0);
}

float ARobotCharacter::GetInputMoveX() const {
	return InputMoveX;
}

EAttackID ARobotCharacter::GetCurrentTypeAttack() const
{
	return CurrentTypeAttack;
}


float ARobotCharacter::GetStunTimer() const
{
	return StunTimer;
}

void ARobotCharacter::SetStunTimer(float StunTime)
{
	StunTimer = StunTime;
}

void ARobotCharacter::UseDash()
{
	CanDash = false;
}

void ARobotCharacter::ResetDash()
{
	CanDash = true;
}

void ARobotCharacter::SetRobotBodyID(ERobotID Robot)
{
	RobotID = Robot;
}

ERobotID ARobotCharacter::GetRobotBodyID() const
{
	return RobotID;
}

int ARobotCharacter::GetDashDirectionX() const
{
	return DashDirectionX;
}

void ARobotCharacter::BindInputAndActions(UEnhancedInputComponent* EnhancedInputComponent) {
	if (InputData == nullptr) return;
	
}

ERobotCharacterPositionEnum ARobotCharacter::GetPositionEnum()
{
	return ERobotCharacterPositionEnum::None;
}

ERobotCharacterDownChargeID ARobotCharacter::GetRobotCharacterDownChargeID()
{
	return  ERobotCharacterDownChargeID::None;
}

void ARobotCharacter::ManageChargeEvent()
{
}

