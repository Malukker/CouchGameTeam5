// Fill out your copyright notice in the Description page of Project Settings.


#include "Camera/CameraWorldSubsystem.h"
#include "Camera/CameraComponent.h"
#include "Camera/CameraSettings.h"
#include "Camera/RobotBoundsActor.h"
#include "Characters/Interface/Robot.h"
#include "Kismet/GameplayStatics.h"
#include "Components/BoxComponent.h"



void UCameraWorldSubsystem::PostInitialize()
{
	Super::PostInitialize();
}

void UCameraWorldSubsystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	//if (CameraMain == nullptr) return;
	TickUpdateCameraPosition(DeltaTime);
	if (RobotBounds)
	{
		SetRobotBounds();
	}
	
	//TickUpdateCameraZoom(DeltaTime);
}

void UCameraWorldSubsystem::AddFollowTarget(UObject* FollowTarget)
{
	TScriptInterface<IRobot> Robot = FollowTarget;
	if (Robot != nullptr)
	{
		FollowTargets.Add(FollowTarget);
	}
}

void UCameraWorldSubsystem::RemoveFollowTarget(UObject* FollowTarget)
{
	FollowTargets.Remove(FollowTarget);
}

void UCameraWorldSubsystem::TickUpdateCameraPosition(float DeltaTime)
{
	if (CameraMain == nullptr) { return; }
	FVector Barycentre = FMath::VInterpTo(CameraMain->GetOwner()->GetActorLocation(),
	                                      CalculateAveragePositionBetweenTargets(), DeltaTime,
	                                      CameraSettings->PositionDampingFactor);
	ClampPositionIntoCameraBounds(Barycentre);
	CameraMain->GetOwner()->SetActorLocation(Barycentre);
}

FVector UCameraWorldSubsystem::CalculateAveragePositionBetweenTargets()
{
	FVector NewLocation = FVector::ZeroVector;
	uint8 NumOfTargets = 0;
	for (UObject* FollowTarget : FollowTargets)
	{
		TScriptInterface<IRobot> ICameraTarget = FollowTarget;
		if (ICameraTarget->IsTargetFollowable())
		{
			FVector TargetPosition = ICameraTarget->GetRobotLocation();
			NewLocation += TargetPosition;
			NumOfTargets++;
		}
	}
	NewLocation /= NumOfTargets;
	NewLocation = FVector(NewLocation.X, CameraMain->GetOwner()->GetActorLocation().Y, GreatestHeightBetweenTargets()-CameraSettings->HeightOffset);
	return NewLocation;
}

float UCameraWorldSubsystem::GreatestHeightBetweenTargets()
{

	float GreatestHeight = 0.f;
	for (UObject* FollowTarget : FollowTargets)
	{
		TScriptInterface<IRobot> ICameraTarget = FollowTarget;
		GreatestHeight = FMath::Max(GreatestHeight,ICameraTarget->GetRobotLocation().Z);
	}
	return GreatestHeight;
}

UCameraComponent* UCameraWorldSubsystem::FindCameraByTag(const FName& Tag) const
{
	TArray<AActor*> Cameras;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), Tag, Cameras);
	if (Cameras.Num() == 0) { return nullptr; }
	UCameraComponent* CameraComp = Cameras[0]->FindComponentByClass<UCameraComponent>();
	return CameraComp;
}

AActor* UCameraWorldSubsystem::FindBoundsActor(FName Tag)
{
	TArray<AActor*> CamerasBounds;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), Tag, CamerasBounds);
	if (CamerasBounds.Num() == 0) { return nullptr; }
	return CamerasBounds[0];
}

ARobotBoundsActor* UCameraWorldSubsystem::FindRobotBoundsActor()
{
	TArray<AActor*> CamerasBounds;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), CameraSettings->RobotBoundsActorTag, CamerasBounds);
	ARobotBoundsActor* RobotBoundsActor = Cast<ARobotBoundsActor>(CamerasBounds[0]);
	return RobotBoundsActor;
}

void UCameraWorldSubsystem::InitCameraBounds(AActor* CameraBoundsActor)
{
	FVector BoundsCenter;
	FVector BoundsExtents;
	CameraBoundsActor->GetActorBounds(false, BoundsCenter, BoundsExtents);
	CameraBoundsMin = FVector2D(BoundsCenter.X - BoundsExtents.X, BoundsCenter.Z - BoundsExtents.Z);
	CameraBoundsMax = FVector2D(BoundsCenter.X + BoundsExtents.X, BoundsCenter.Z + BoundsExtents.Z);
	CameraBoundsYProjectionCenter = BoundsCenter.Y;
}

void UCameraWorldSubsystem::InitRobotBounds(AActor* RobotBoundsActor)
{
	FVector BoundsCenter;
	FVector BoundsExtents;
	RobotBoundsActor->GetActorBounds(false, BoundsCenter, BoundsExtents);
	RobotBoundsMin = FVector2D(BoundsCenter.X - BoundsExtents.X, BoundsCenter.Z - BoundsExtents.Z);
	RobotBoundsMax = FVector2D(BoundsCenter.X + BoundsExtents.X, BoundsCenter.Z + BoundsExtents.Z);
	
}

void UCameraWorldSubsystem::ClampPositionIntoCameraBounds(FVector& Position)
{
	FVector2D ViewportBoundsMin, ViewportBoundsMax;

	GetViewportBounds(ViewportBoundsMin, ViewportBoundsMax);
	
	FVector WorldBoundsMin = CalculateWorldPositionFromViewportPosition(ViewportBoundsMin);
	FVector WorldBoundsMax = CalculateWorldPositionFromViewportPosition(ViewportBoundsMax);

	float Width = (WorldBoundsMax.X-WorldBoundsMin.X)/2;
    float Height = FMath::Abs((WorldBoundsMax.Z-WorldBoundsMin.Z)/2);

	
	FVector WorldArenaBoundsMin = FVector(CameraBoundsMin.X+Width,0,0);
	FVector WorldArenaBoundsMax = FVector(CameraBoundsMax.X-Width,0,0);
	
	if ( WorldArenaBoundsMin.X > WorldArenaBoundsMax.X)
	{
		Position.X = (CameraBoundsMin.X + CameraBoundsMax.X) * 0.5f;
	}
	else 
	{
		Position.X = FMath::Clamp(Position.X, WorldArenaBoundsMin.X, WorldArenaBoundsMax.X);
	}
}

void UCameraWorldSubsystem::SetRobotBounds()
{
	FVector2D ViewportBoundsMin, ViewportBoundsMax;
	UGameViewportClient* ViewportClient = GetWorld()->GetGameViewport();
	if (ViewportClient == nullptr) return;

	GetViewportBounds(ViewportBoundsMin, ViewportBoundsMax);

	
	FVector WorldBoundsMin = CalculateWorldPositionFromViewportPosition(ViewportBoundsMin);
	FVector WorldBoundsMax = CalculateWorldPositionFromViewportPosition(ViewportBoundsMax);

	ARobotBoundsActor* RobotBoundsActor = FindRobotBoundsActor();
	if (!RobotBoundsActor) return;
	UBoxComponent* BoxLeft=RobotBoundsActor->Box_Left;
	UBoxComponent* BoxRight=RobotBoundsActor->Box_Right;

	float WorldY =  RobotBoundsActor->GetActorLocation().Y;
	float WorldZ =  RobotBoundsActor->GetActorLocation().Z;
	
	
	WorldBoundsMin.X = FMath::Clamp(WorldBoundsMin.X,RobotBoundsMin.X, RobotBoundsMax.X);
	BoxLeft->SetWorldLocation(FVector(WorldBoundsMin.X,WorldY,WorldZ));
	BoxLeft->SetBoxExtent(FVector(1.f,1.f , RobotBoundsMax.Y));

	WorldBoundsMax.X = FMath::Clamp(WorldBoundsMax.X,RobotBoundsMin.X, RobotBoundsMax.X);
	BoxRight->SetWorldLocation(FVector(WorldBoundsMax.X,WorldY,WorldZ));
	BoxRight->SetBoxExtent(FVector(1.f,1.f , RobotBoundsMax.Y));
}

float UCameraWorldSubsystem::CalculateGreatestDistanceBetweenTargets()
{
	float GreatestDistance = 0.f;

	for (int i = 0; i < FollowTargets.Num() - 1; i++)
	{
		TScriptInterface<IRobot> ICameraFirstTarget = FollowTargets[i];
		for (int j = i + 1; j < FollowTargets.Num(); j++)
		{
			TScriptInterface<IRobot> ICameraSecondTarget = FollowTargets[j];
			if (ICameraFirstTarget && ICameraSecondTarget)
			{
				float CurrentDistance = FMath::Abs(
					ICameraFirstTarget->GetRobotLocation().X - ICameraSecondTarget->GetRobotLocation().X);
				if (CurrentDistance > GreatestDistance)
				{
					GreatestDistance = CurrentDistance;
				}
			}
		}
	}
	return GreatestDistance;
}



void UCameraWorldSubsystem::GetViewportBounds(FVector2D& OutViewportBoundsMin, FVector2D& OutViewportBoundsMax)
{
	//Find Viewport
	UGameViewportClient* ViewportClient = GetWorld()->GetGameViewport();
	if (ViewportClient == nullptr) return;

	FViewport* Viewport = ViewportClient->Viewport;
	if (Viewport == nullptr) return;

	//Calculate Viewport Rect according to Camera Aspect Ratio and Viewport ViewRect
	FIntRect ViewRect(Viewport->GetInitialPositionXY(), Viewport->GetInitialPositionXY() + Viewport->GetSizeXY());
	FIntRect ViewportRect = Viewport->CalculateViewExtents(CameraMain->AspectRatio, ViewRect);
	

	//Fill Output parameters with ViewportRect
	OutViewportBoundsMin.X = ViewportRect.Min.X;
	OutViewportBoundsMin.Y = ViewportRect.Min.Y;

	OutViewportBoundsMax.X = ViewportRect.Max.X;
	OutViewportBoundsMax.Y = ViewportRect.Max.Y;

	
}

FVector UCameraWorldSubsystem::CalculateWorldPositionFromViewportPosition(const FVector2D& ViewportPosition)
{
	if (CameraMain == nullptr) return FVector::Zero();

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PlayerController == nullptr) return FVector::Zero();
	

	FVector CameraWorldProjectDir;
	FVector WorldPosition;

	UGameplayStatics::DeprojectScreenToWorld(PlayerController, ViewportPosition, WorldPosition, CameraWorldProjectDir);

	
	float t = (CameraBoundsYProjectionCenter - WorldPosition.Y) / CameraWorldProjectDir.Y;
	WorldPosition += CameraWorldProjectDir*t;

	return WorldPosition;

}



void UCameraWorldSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);
	InWorld.GetTimerManager().SetTimerForNextTick([this, &InWorld]()
	{
		CameraSettings = GetDefault<UCameraSettings>();
		CameraMain = FindCameraByTag(CameraSettings->CameraMainTag);
		if (CameraMain == nullptr) { return; }
		AActor* CameraBoundsActor = FindBoundsActor(CameraSettings->CameraBoundsTag);
		if (CameraBoundsActor)
		{
			InitCameraBounds(CameraBoundsActor);
		}
		 RobotBounds = FindBoundsActor(CameraSettings->RobotBoundsTag);
		if (RobotBounds)
		{
			InitRobotBounds(RobotBounds);
		}
		
	});
}
