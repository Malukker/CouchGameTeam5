// Fill out your copyright notice in the Description page of Project Settings.


#include "Camera/RobotBoundsActor.h"


#include "Components/BoxComponent.h"


// Sets default values
ARobotBoundsActor::ARobotBoundsActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	SetRootComponent(Root);
	
	Box_Right = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxRight"));
	Box_Right->SetupAttachment(Root);
	
	Box_Left = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxLeft"));
	Box_Left->SetupAttachment(Root);

	Box_Top = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxTop"));
	Box_Top->SetupAttachment(Root);
	
	
}

// Called when the game starts or when spawned
void ARobotBoundsActor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ARobotBoundsActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ARobotBoundsActor::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	Box_Top->SetWorldLocation(FVector(Box_Top->GetComponentLocation().X, Box_Top->GetComponentLocation().Y, BoxTopHeight));
	Box_Top->SetBoxExtent(BoxTopExtent);
	Box_Top->SetLineThickness(LineThickness);
	Box_Left->SetLineThickness(LineThickness);
	Box_Right->SetLineThickness(LineThickness);
	Box_Left->SetBoxExtent(BoxSidesExtent);
	Box_Right->SetBoxExtent(BoxSidesExtent);
}

