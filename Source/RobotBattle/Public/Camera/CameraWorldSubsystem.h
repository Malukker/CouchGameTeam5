// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "CameraWorldSubsystem.generated.h"

class AWinManager;
class ARobotBoundsActor;
class UCameraSettings;
class UCameraComponent;
/**
 * 
 */
UCLASS()
class ROBOTBATTLE_API UCameraWorldSubsystem : public UTickableWorldSubsystem
{
	GENERATED_BODY()
	
	

	UPROPERTY()
	const UCameraSettings* CameraSettings;
#pragma region Subsystem Overrides
public:
	virtual void PostInitialize() override;

	virtual void OnWorldBeginPlay(UWorld& InWorld) override;

	virtual void Tick(float DeltaTime) override;

	virtual TStatId GetStatId() const override {return TStatId(); }
#pragma endregion

#pragma region Follow Targets
	void AddFollowTarget(UObject* FollowTarget);

	void RemoveFollowTarget(UObject* FollowTarget);

protected:
	UPROPERTY()
	TArray<UObject*> FollowTargets;

	void TickUpdateCameraPosition(float DeltaTime);

	FVector CalculateAveragePositionBetweenTargets();
	float GreatestHeightBetweenTargets();
#pragma endregion Follow Targets

#pragma region MainCamera
protected:
	UPROPERTY()
	TObjectPtr<UCameraComponent> CameraMain;

	float CalculateGreatestDistanceBetweenTargets();

	//void TickUpdateCameraZoom(float DeltaTime);

	UCameraComponent* FindCameraByTag(const FName& Tag) const;
#pragma endregion MainCamera
	
#pragma region Bounds
private:
	UPROPERTY()
	FVector2D CameraBoundsMin;

	UPROPERTY()
	FVector2D CameraBoundsMax;

	UPROPERTY()
	FVector2D RobotBoundsMin;

	UPROPERTY()
	FVector2D RobotBoundsMax;

	UPROPERTY()
	float CameraBoundsYProjectionCenter;

	AActor* FindBoundsActor(FName Tag);
	
	UPROPERTY()
	AActor* RobotBounds;

	ARobotBoundsActor* FindRobotBoundsActor();

	void InitCameraBounds(AActor* CameraBoundsActor);

	void InitRobotBounds(AActor* RobotBoundsActor);

	void ClampPositionIntoCameraBounds(FVector& Position);

	void SetRobotBounds();
	
	UFUNCTION(BlueprintCallable)
	void GetViewportBounds(FVector2D& OutViewportBoundsMin, FVector2D& OutViewportBoundsMax);

	UFUNCTION(BlueprintCallable)
	FVector CalculateWorldPositionFromViewportPosition(const FVector2D& ViewportPosition);
#pragma endregion Bounds

#pragma region CameraWinScene
	UPROPERTY()
	TObjectPtr<UCameraComponent> CameraWin;

	UPROPERTY()
	TObjectPtr<UCameraComponent> CameraZoomWin;

	UFUNCTION()
	void StartCameraZoom(float deltatime);

	UPROPERTY()
	AWinManager* WinManager = nullptr;

	bool GameOverUISetted = false;
#pragma endregion CameraWinScene
};
