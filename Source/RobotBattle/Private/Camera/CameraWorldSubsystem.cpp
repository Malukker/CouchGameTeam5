// Fill out your copyright notice in the Description page of Project Settings.


#include "Camera/CameraWorldSubsystem.h"
#include "Camera/CameraComponent.h"
#include "Camera/CameraSettings.h"
#include "Kismet/GameplayStatics.h"


void UCameraWorldSubsystem::PostInitialize()
{
	Super::PostInitialize();
	CameraSettings = GetDefault<UCameraSettings>();
	
}

void UCameraWorldSubsystem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	TickUpdateCameraPosition(DeltaTime);
	TickUpdateCameraZoom(DeltaTime);
}

void UCameraWorldSubsystem::AddFollowTarget(UObject* FollowTarget)
{
	TScriptInterface<ICameraFollowTarget> CameraFollowTarget = FollowTarget;
	if (CameraFollowTarget!=nullptr)
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
	FVector Barycentre=FMath::VInterpTo(CameraMain->GetOwner()->GetActorLocation(),CalculateAveragePositionBetweenTargets(),DeltaTime,CameraSettings->PositionDampingFactor);
	ClampPositionIntoCameraBounds(Barycentre);
	CameraMain->GetOwner()->SetActorLocation(Barycentre);
}

FVector UCameraWorldSubsystem::CalculateAveragePositionBetweenTargets()
{
	FVector NewLocation = FVector::ZeroVector;
	uint8 Index = 0;
	for (UObject* FollowTarget : FollowTargets)
	{
		TScriptInterface<ICameraFollowTarget> ICameraTarget = FollowTarget;
		if (ICameraTarget->IsFollowable())
		{
			FVector TargetPosition = ICameraTarget->GetFollowPosition();
			NewLocation += TargetPosition;
			Index++;
		}
		
	}
	 NewLocation /= Index;
	NewLocation = FVector(NewLocation.X,CameraMain->GetOwner()->GetActorLocation().Y,NewLocation.Z);
	return  NewLocation;
}

UCameraComponent* UCameraWorldSubsystem::FindCameraByTag(const FName& Tag) const
{
	TArray<AActor*> Cameras; 
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), Tag,Cameras);
	UCameraComponent* CameraComp = Cameras[0]->FindComponentByClass<UCameraComponent>();
	return CameraComp;
	
}

AActor* UCameraWorldSubsystem::FindCameraBoundsActor()
{
	TArray<AActor*> CamerasBounds; 
	UGameplayStatics::GetAllActorsWithTag(GetWorld(),CameraSettings->CameraBoundsTag,CamerasBounds);
	return CamerasBounds[0];
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

void UCameraWorldSubsystem::ClampPositionIntoCameraBounds(FVector& Position)
{
	FVector2D ViewportBoundsMin,ViewportBoundsMax;
	
	GetViewportBounds(ViewportBoundsMin, ViewportBoundsMax);
	float Width = (CameraBoundsMax.X - CameraBoundsMin.X)*0.5f;;
	float Height = (CameraBoundsMax.Y - CameraBoundsMin.Y)*0.5f;

	ViewportBoundsMin.X += Width;
	ViewportBoundsMax.X -= Width;
	ViewportBoundsMin.Y += Height;
	ViewportBoundsMax.Y -= Height;
	FVector WorldBoundsMin = CalculateWorldPositionFromViewportPosition(ViewportBoundsMin);
	FVector WorldBoundsMax = CalculateWorldPositionFromViewportPosition(ViewportBoundsMax);
	

	FVector PositionToClampMinViewport( FMath::Min(WorldBoundsMin.X, WorldBoundsMax.X),Position.Y,FMath::Min(WorldBoundsMin.Z, WorldBoundsMax.Z));
	FVector PositionToClampMaxViewport(  FMath::Max(WorldBoundsMin.X, WorldBoundsMax.X),Position.Y,FMath::Max(WorldBoundsMin.Z, WorldBoundsMax.Z));
	//Position=ClampVector(Position,PositionToClampMinViewport,PositionToClampMaxViewport);


	float MarginX = (CameraBoundsMax.X - CameraBoundsMin.X) * 0.4f;
	float MarginZ = (CameraBoundsMax.Y - CameraBoundsMin.Y) * 0.4f;

	FVector PositionToClampMin(
		CameraBoundsMin.X + MarginX,
		Position.Y,
		CameraBoundsMin.Y + MarginZ
	);

	FVector PositionToClampMax(
		CameraBoundsMax.X - MarginX,
		Position.Y,
		CameraBoundsMax.Y - MarginZ
	);

	Position = ClampVector(Position, PositionToClampMin, PositionToClampMax);
}

float UCameraWorldSubsystem::CalculateGreatestDistanceBetweenTargets()
{
	float GreatestDistance = 0.f;
	
	for (int i = 0; i< FollowTargets.Num()-1; i++)
	{
		TScriptInterface<ICameraFollowTarget> ICameraFirstTarget = FollowTargets[i];
		for (int j = i+1; j< FollowTargets.Num(); j++)
		{
			TScriptInterface<ICameraFollowTarget> ICameraSecondTarget = FollowTargets[j];
			if (ICameraFirstTarget && ICameraSecondTarget)
			{
				float CurrentDistance = FMath::Abs(ICameraFirstTarget->GetFollowPosition().X - ICameraSecondTarget->GetFollowPosition().X);
				if (CurrentDistance > GreatestDistance)
				{
					GreatestDistance = CurrentDistance;
				}
			}
		}
	}
	return GreatestDistance;
}

void UCameraWorldSubsystem::TickUpdateCameraZoom(float DeltaTime)
{
	if (CameraMain==nullptr) return;
	float GreatestDistanceBetweenTargets = CalculateGreatestDistanceBetweenTargets();
	float CurrentPercent = (GreatestDistanceBetweenTargets-CameraSettings->DistanceBetweenTargetsMin) / (CameraSettings->DistanceBetweenTargetsMax-CameraSettings->DistanceBetweenTargetsMin);
	CurrentPercent=FMath::Clamp(CurrentPercent, 0.f, 1.f);
	float ZoomDistance = FMath::Lerp(CameraZoomYMin, CameraZoomYMax, CurrentPercent);
	FVector NewLocation(CameraMain->GetOwner()->GetActorLocation().X,ZoomDistance,CameraMain->GetOwner()->GetActorLocation().Z);
	NewLocation = FMath::VInterpTo(CameraMain->GetOwner()->GetActorLocation(),NewLocation,DeltaTime,CameraSettings->SizeDampingFactor);
	CameraMain->GetOwner()->SetActorLocation(NewLocation);
}


void UCameraWorldSubsystem::GetViewportBounds(FVector2D& OutViewportBoundsMin, FVector2D& OutViewportBoundsMax)
{
	//Find Viewport
	UGameViewportClient* ViewportClient = GetWorld()->GetGameViewport();
	if (ViewportClient==nullptr) return;

	FViewport* Viewport= ViewportClient->Viewport;
	if (Viewport==nullptr) return;

	//Calculate Viewport Rect according to Camera Aspect Ratio and Viewport ViewRect
	FIntRect ViewRect(Viewport->GetInitialPositionXY(),Viewport->GetInitialPositionXY()+Viewport->GetSizeXY());
	FIntRect ViewportRect = Viewport->CalculateViewExtents(CameraMain->AspectRatio,ViewRect);

	//Fill Output parameters with ViewportRect
	OutViewportBoundsMin.X = ViewportRect.Min.X;
	OutViewportBoundsMin.Y = ViewportRect.Min.Y;
	
	OutViewportBoundsMax.X = ViewportRect.Max.X;
	OutViewportBoundsMax.Y = ViewportRect.Max.Y;
}

FVector UCameraWorldSubsystem::CalculateWorldPositionFromViewportPosition(const FVector2D& ViewportPosition)
{
	if (CameraMain==nullptr) return FVector::Zero();

	APlayerController* PlayerController = UGameplayStatics::GetPlayerController(GetWorld(),0);
	if (PlayerController==nullptr) return FVector::Zero();

	float YDistanceToCenter = CameraMain->GetOwner()->GetActorLocation().Y - CameraBoundsYProjectionCenter;

	FVector CameraWorldProjectDir;
	FVector WorldPosition;

	UGameplayStatics::DeprojectScreenToWorld(PlayerController,ViewportPosition,WorldPosition,CameraWorldProjectDir);

	WorldPosition+=CameraWorldProjectDir*YDistanceToCenter;

	return WorldPosition;
}

void UCameraWorldSubsystem::InitCameraZoomParameters()
{
	UCameraComponent* CameraDistanceMin=FindCameraByTag(CameraSettings->CameraDistanceMinTag);
	UCameraComponent* CameraDistanceMax=FindCameraByTag(CameraSettings->CameraDistanceMaxTag);

	if (CameraDistanceMin ==nullptr && CameraDistanceMax==nullptr) return;
	CameraZoomYMin=CameraDistanceMin->GetComponentLocation().Y;
	CameraZoomYMax=CameraDistanceMax->GetComponentLocation().Y;
	
}

void UCameraWorldSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);
	CameraMain = FindCameraByTag(CameraSettings->CameraMainTag);

	AActor* CameraBoundsActor = FindCameraBoundsActor();
	if (CameraBoundsActor!=nullptr)
	{
		InitCameraBounds(CameraBoundsActor);
	}

	InitCameraZoomParameters();
}
