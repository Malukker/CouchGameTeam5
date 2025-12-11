// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RobotBoundsActor.generated.h"



UCLASS()
class ROBOTBATTLE_API ARobotBoundsActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ARobotBoundsActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, Category = "BoxParameters")
	float LineThickness;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<class UBoxComponent> Box_Right;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<class UBoxComponent> Box_Left;
	
	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<class UBoxComponent> Box_Top;

	UPROPERTY(EditDefaultsOnly)
	TObjectPtr<class USceneComponent> Root;

	UPROPERTY(EditAnywhere, Category = "BoxTopParameters")
	float BoxTopHeight;

	UPROPERTY(EditAnywhere, Category = "BoxTopParameters")
	FVector BoxTopExtent;

	UPROPERTY(EditAnywhere, Category = "BoxSidesParameter")
	FVector BoxSidesExtent;

	
private:
	virtual void OnConstruction(const FTransform& Transform) override;


};
