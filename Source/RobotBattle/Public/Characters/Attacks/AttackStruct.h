#pragma once

#include "CoreMinimal.h"
#include "AttackStruct.generated.h"

class UNiagaraSystem;

UENUM(BlueprintType)
enum class ERobotID	: uint8
{
	None = 0,
	Robot1 = 1,
	Robot2 = 2
};

UENUM(BlueprintType)
enum class EAttackID	: uint8
{
	None = 0,
	Type1 = 1,
	Type2 = 2,
	Type3 = 3,
	Ultimate = 4,
};

USTRUCT(BlueprintType)
struct FAttackStruct
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FName StartSocket;

	UPROPERTY(EditAnywhere)
	FName EndSocket;

	UPROPERTY(EditAnywhere)
	float TraceRadius = 0.f;

	UPROPERTY(EditAnywhere)
	int Damage = 0;
	
	UPROPERTY(EditAnywhere)
	float StunTimer = 0;
	
	UPROPERTY(EditAnywhere)
	float BoostMultiplier = 1.f;

	UPROPERTY(EditAnywhere)
	EAttackID AttackType = EAttackID::None;

	UPROPERTY(EditAnywhere)
	UNiagaraSystem* Niagara;
};
