// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/RobotCharacter.h"
#include "Characters/RobotCharacterStateMachine.h"
#include <EnhancedInputSubsystems.h>
#include "EnhancedInputComponent.h"
#include "Camera/CameraWorldSubsystem.h"
#include "Characters/RobotCharacterInputData.h"
#include "Characters/RobotCharacterPositionEnum.h"
#include "Characters/RobotCharacterStateID.h"
#include "Components/AudioComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Kismet/GameplayStatics.h"
#include "UI/HUDGameplay.h"

// Sets default values
ARobotCharacter::ARobotCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BoxComponent = CreateDefaultSubobject<UBoxComponent>(FName("BoxComponent"));
	BoxComponent->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);

	AudioComponent = CreateDefaultSubobject<UAudioComponent>(FName("Audio"));
	AudioComponent->SetupAttachment(GetRootComponent());
}

// Called when the game starts or when spawned
void ARobotCharacter::BeginPlay()
{
	Super::BeginPlay();
	CreateStateMachine();

	InitStateMachine();
	GetWorld()->GetSubsystem<UCameraWorldSubsystem>()->AddFollowTarget(this);
	if (GetRobotBodyID() == ERobotID::None)
	{
		UE_LOG(LogTemp, Error, TEXT("ROBOT ID NONE !"));
	}
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

	SetupMappingContextIntoController(false);

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (EnhancedInputComponent == nullptr) return;

	BindInputAndActions(EnhancedInputComponent);
}

UBoxComponent* ARobotCharacter::GetCollision() const
{
	return BoxComponent;
}

int ARobotCharacter::GetTeam()
{
	return Team;
}

void ARobotCharacter::SetTeam(int NewTeam)
{
	Team = NewTeam;
}

int ARobotCharacter::GetLife()
{
	return  Life;
}

int ARobotCharacter::GetGuard()
{
	return Guard;
}

int ARobotCharacter::GetInvinsibilityFrames()
{
	return InvinsibilityFrames;
}

float ARobotCharacter::GetNerfStatsMultiplier()
{
	return NerfStatsMultiplier;
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
	if (MeshMirror) Rotation.Yaw = 90.f * OrientX;
	else Rotation.Yaw = -90.f * OrientX;
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

void ARobotCharacter::ResetStateMachine()
{
	StateMachine->ChangeState(ERobotCharacterStateID::Idle);
}

void ARobotCharacter::SetupMappingContextIntoController(bool bMenu) const {
	APlayerController* PlayerController = Cast<APlayerController>(Controller);
	if (PlayerController == nullptr) return;

	ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer();
	if (LocalPlayer == nullptr)return;

	UEnhancedInputLocalPlayerSubsystem* InputSystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	if (InputSystem == nullptr)return;

	if (bMenu)
	{
		InputSystem->AddMappingContext(InputMappingContextMenu, 0);
	}
	else
	{
		InputSystem->AddMappingContext(InputMappingContextGameplay, 0);
	}
}

float ARobotCharacter::GetInputMoveX() const {
	return InputMoveX;
}

bool ARobotCharacter::IsWalkingForward() const
{
	if (FMath::Abs(GetInputMoveX()) <= .3f) return true;
	return FMath::Sign(GetOrientX()) == FMath::Sign(GetInputMoveX());
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

bool ARobotCharacter::DoHaveEnergy()
{
	return HaveEnergy;
}

bool ARobotCharacter::DoWantSwitch()
{
	return WantSwitch;
}

void ARobotCharacter::SwitchEnergy()
{
	WantSwitch = false;
	HaveEnergy = !HaveEnergy;
	GetMesh()->SetRenderCustomDepth(HaveEnergy);
	EnergyEvent.Broadcast();
}

void ARobotCharacter::SetEnergy(bool Value)
{
	HaveEnergy = Value;
	GetMesh()->SetRenderCustomDepth(HaveEnergy);
	EnergyEvent.Broadcast();
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


void ARobotCharacter::OnInputMoveX(const FInputActionValue& InputActionValue)
{
	if (UGameplayStatics::IsGamePaused(GetWorld())) return;
	InputMoveX = InputActionValue.Get<float>();
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

void ARobotCharacter::OnInputEnergy(const FInputActionValue& InputActionValue)
{
	if (UGameplayStatics::IsGamePaused(GetWorld()) || !DoHaveEnergy()) return;
	ERobotCharacterStateID State = StateMachine->GetCurrentStateID();
	if (State == ERobotCharacterStateID::Attack
		|| State == ERobotCharacterStateID::LoadingAttack
		|| State == ERobotCharacterStateID::Dash)
	{
		WantSwitch = true;
		return;
	}
	EnergyManagerEvent.Broadcast();
}

void ARobotCharacter::OnInputPause(const FInputActionValue& InputActionValue)
{
	APlayerController* PlayerController = Cast<APlayerController>(Controller);
	if (PlayerController == nullptr) return;

	if (!HUDGameplay)
	{
		HUDGameplay = Cast<AHUDGameplay>(PlayerController->MyHUD);
		if (!HUDGameplay) return;
	}
	SetupMappingContextIntoController(true);
	FInputModeUIOnly InputMode;
	InputMode.SetWidgetToFocus(HUDGameplay->SpawnUIPause());
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::LockOnCapture);
	PlayerController->SetInputMode(InputMode);
	
	UGameplayStatics::SetGamePaused(GetWorld(), true);
}

void ARobotCharacter::BindInputAndActions(UEnhancedInputComponent* EnhancedInputComponent) {
	if (InputDataGameplay == nullptr) return;
	
	if (InputDataGameplay->InputActionMoveX) {
		EnhancedInputComponent->BindAction(
			InputDataGameplay->InputActionMoveX,
			ETriggerEvent::Started,
			this,
			&ARobotCharacter::OnInputMoveX
		);
		EnhancedInputComponent->BindAction(
			InputDataGameplay->InputActionMoveX,
			ETriggerEvent::Completed,
			this,
			&ARobotCharacter::OnInputMoveX
		);
		EnhancedInputComponent->BindAction(
			InputDataGameplay->InputActionMoveX,
			ETriggerEvent::Triggered,
			this,
			&ARobotCharacter::OnInputMoveX
		);
	}

	if (InputDataGameplay->InputActionRightDash)
	{
		EnhancedInputComponent->BindAction(InputDataGameplay->InputActionRightDash,ETriggerEvent::Started,this,&ARobotCharacter::OnInputRightDash);
	}
	
	if (InputDataGameplay->InputActionLeftDash)
	{
		EnhancedInputComponent->BindAction(InputDataGameplay->InputActionLeftDash,ETriggerEvent::Started,this,&ARobotCharacter::OnInputLeftDash);
	}
	
	if (InputDataGameplay->InputActionAttackDuo)
	{
		EnhancedInputComponent->BindAction(InputDataGameplay->InputActionAttackDuo, ETriggerEvent::Started, this, &ARobotCharacter::OnInputAttackDuo);
	}
	
	if (InputDataGameplay->InputActionEnergy)
	{
		EnhancedInputComponent->BindAction(InputDataGameplay->InputActionEnergy, ETriggerEvent::Started, this, &ARobotCharacter::OnInputEnergy);
	}
	
	if (InputDataGameplay->InputActionPause)
	{
		EnhancedInputComponent->BindAction(InputDataGameplay->InputActionPause, ETriggerEvent::Started, this, &ARobotCharacter::OnInputPause);
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

bool ARobotCharacter::IsTargetFollowable()
{
	return IsFollowable;
}

void ARobotCharacter::SetCanAttackDuo(bool CanAttack)
{
	CanAttackDuo = CanAttack;
}

bool ARobotCharacter::StartAttackDuo()
{
	return false;
}

void ARobotCharacter::AddDamageBonus()
{
	DamageBonus ++;
}

int ARobotCharacter::GetDamageBonus()
{
	return DamageBonus;
}

void ARobotCharacter::ResetDamageBonus()
{
	DamageBonus = 0;
}

int ARobotCharacter::GetCharge()
{
	return Charge;
}

void ARobotCharacter::TakeDamageFromAttack(int Damage, float StunTime)
{
	HurtManagerEvent.Broadcast(Damage, StunTime);
	//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Blue, TEXT("DAMAGE!!"));
}

void ARobotCharacter::KnockBackFromNotify(FVector2D Velocity)
{
	KnockBackEvent.Broadcast(Velocity);
}
