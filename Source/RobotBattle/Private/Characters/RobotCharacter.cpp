// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/RobotCharacter.h"
#include "Characters/RobotCharacterStateMachine.h"
#include <EnhancedInputSubsystems.h>
#include "EnhancedInputComponent.h"
#include "Camera/CameraWorldSubsystem.h"
#include "Characters/RobotCharacterInputData.h"
#include "Characters/RobotCharacterPositionEnum.h"
#include "Characters/RobotCharacterSettings.h"

// Sets default values
ARobotCharacter::ARobotCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ARobotCharacter::BeginPlay()
{
	Super::BeginPlay();
	CreateStateMachine();

	InitStateMachine();
	GetWorld()->GetSubsystem<UCameraWorldSubsystem>()->AddFollowTarget(this);
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
	
	const URobotCharacterSettings* CharacterSettings = GetDefault<URobotCharacterSettings>();
	if (CharacterSettings == nullptr) return;
	
	InputSystem->AddMappingContext(CharacterSettings->InputMappingContextMenu.LoadSynchronous(), 1);
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

void ARobotCharacter::DoGuardTest()
{
}

void ARobotCharacter::OnInputMoveX(const FInputActionValue& InputActionValue)
{
}

void ARobotCharacter::OnInputRightDash(const FInputActionValue& InputActionValue)
{
}

void ARobotCharacter::OnInputLeftDash(const FInputActionValue& InputActionValue)
{
}

void ARobotCharacter::OnInputAttackDuo(const FInputActionValue& InputActionValue)
{
}

void ARobotCharacter::OnInputPause(const FInputActionValue& InputActionValue)
{
	
}

void ARobotCharacter::BindInputAndActions(UEnhancedInputComponent* EnhancedInputComponent) {
	if (InputData == nullptr) return;
	
	if (InputData->InputActionMoveX) {
		EnhancedInputComponent->BindAction(
			InputData->InputActionMoveX,
			ETriggerEvent::Started,
			this,
			&ARobotCharacter::OnInputMoveX
		);
		EnhancedInputComponent->BindAction(
			InputData->InputActionMoveX,
			ETriggerEvent::Completed,
			this,
			&ARobotCharacter::OnInputMoveX
		);
		EnhancedInputComponent->BindAction(
			InputData->InputActionMoveX,
			ETriggerEvent::Triggered,
			this,
			&ARobotCharacter::OnInputMoveX
		);
	}

	if (InputData->InputActionRightDash)
	{
		EnhancedInputComponent->BindAction(InputData->InputActionRightDash,ETriggerEvent::Started,this,&ARobotCharacter::OnInputRightDash);
	}
	
	if (InputData->InputActionLeftDash)
	{
		EnhancedInputComponent->BindAction(InputData->InputActionLeftDash,ETriggerEvent::Started,this,&ARobotCharacter::OnInputLeftDash);
	}
	
	if (InputData->InputActionAttackDuo)
	{
		EnhancedInputComponent->BindAction(InputData->InputActionAttackDuo, ETriggerEvent::Started, this, &ARobotCharacter::OnInputAttackDuo);
	}
	
	if (InputData->InputActionPause)
	{
		EnhancedInputComponent->BindAction(InputData->InputActionPause, ETriggerEvent::Started, this, &ARobotCharacter::OnInputPause);
	}
}

ERobotCharacterPositionEnum ARobotCharacter::GetPositionEnum()
{
	return ERobotCharacterPositionEnum::None;
}

ERobotCharacterDownChargeID ARobotCharacter::GetRobotCharacterDownChargeID()
{
	return  ERobotCharacterDownChargeID::None;
}

FVector ARobotCharacter::GetRobotLocation()
{
	return GetActorLocation();
}

void ARobotCharacter::ManageChargeEvent(bool CanAttack)
{
	CanAttackDuo = CanAttack;
}

void ARobotCharacter::AddDamageBonus()
{
	DamageBonus += 4;
}

int ARobotCharacter::GetDamageBonus()
{
	return DamageBonus;
}

void ARobotCharacter::ResetDamageBonus()
{
	DamageBonus = 0;
}

void ARobotCharacter::TakeDamageFromAttack(int Damage, float StunTime)
{
	HurtManagerEvent.Broadcast(Damage, StunTime);
	//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Blue, TEXT("DAMAGE!!"));
}
