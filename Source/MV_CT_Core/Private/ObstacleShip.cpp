// Fill out your copyright notice in the Description page of Project Settings.


#include "ObstacleShip.h"
#include "MV_CoreLogCategory.h"

AObstacleShip::AObstacleShip()
{
	PrimaryActorTick.bCanEverTick = true;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	RootComponent = SceneRoot;

	ShipRoot = CreateDefaultSubobject<USceneComponent>(TEXT("ShipRoot"));
	ShipRoot->SetupAttachment(RootComponent);

	ShipMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShipMesh"));
	ShipMesh->SetupAttachment(ShipRoot);

	PathSpline = CreateDefaultSubobject<USplineComponent>(TEXT("PathSpline"));
	PathSpline->SetupAttachment(RootComponent);
}

void AObstacleShip::BeginPlay()
{
	Super::BeginPlay();

	SplineLength = PathSpline->GetSplineLength();
}

void AObstacleShip::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (FollowSpline) {
		UpdateMeshPosition(DeltaTime);
	}
}

void AObstacleShip::UpdateMeshPosition(float DeltaTime)
{
	CurrentDistance += MovementSpeed * DeltaTime;

	if (CurrentDistance > SplineLength)
	{
		CurrentDistance = 0.0f;
	}

	FVector Location = PathSpline->GetLocationAtDistanceAlongSpline(CurrentDistance, ESplineCoordinateSpace::World);
	FRotator Rotation = PathSpline->GetRotationAtDistanceAlongSpline(CurrentDistance, ESplineCoordinateSpace::World);
	FVector Scale = PathSpline->GetScaleAtDistanceAlongSpline(CurrentDistance);

	ShipRoot->SetWorldLocation(Location);
	ShipRoot->SetWorldRotation(Rotation);
	ShipRoot->SetWorldScale3D(Scale);
}