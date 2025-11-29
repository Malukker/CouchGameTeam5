#pragma once

#include "CoreMinimal.h"
#include "AttackStruct.generated.h"


UENUM(BlueprintType)
enum class ERobotID	: uint8
{
	None = 0,
	Robot1 = 1,
	Robot2 = 2,
	Robot3 = 3,
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
	UAnimMontage* AnimMontage = nullptr;

	UPROPERTY(EditAnywhere)
	TArray<FName> ConcernedBones = {};

	UPROPERTY(EditAnywhere)
	float TraceRadius = 0.f;

	UPROPERTY(EditAnywhere)
	int Damage = 0;

	UPROPERTY(EditAnywhere)
	float AttackDuoBonusMultiplier = 0;
	
	UPROPERTY(EditAnywhere)
	float StunTimer = 0;

	UPROPERTY(EditAnywhere)
	FVector2D KnockBackVector = FVector2D::ZeroVector;
};
