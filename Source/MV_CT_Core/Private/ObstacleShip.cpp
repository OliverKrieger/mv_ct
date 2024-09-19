// Fill out your copyright notice in the Description page of Project Settings.


#include "ObstacleShip.h"
#include "MV_CoreLogCategory.h"

// Sets default values
AObstacleShip::AObstacleShip()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
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

// Called when the game starts or when spawned
void AObstacleShip::BeginPlay()
{
	Super::BeginPlay();

	SplineLength = PathSpline->GetSplineLength();
}

// Called every frame
void AObstacleShip::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateMeshPosition(DeltaTime);

}

void AObstacleShip::UpdateMeshPosition(float DeltaTime)
{
	CurrentDistance += MovementSpeed * DeltaTime;
	UE_LOG(MV_CoreLogCategory, Log, TEXT("Obstacle at distance %f"), CurrentDistance);
	UE_LOG(MV_CoreLogCategory, Log, TEXT("Spline Length Total %f"), SplineLength);

	// Ensure that the mesh loops or stops at the end of the spline
	if (CurrentDistance > SplineLength)
	{
		UE_LOG(MV_CoreLogCategory, Log, TEXT("Obstacle reached end"));
		CurrentDistance = 0.0f;  // Restart at the beginning for looping
	}

	// Get the location, rotation, and scale at the current distance along the spline
	FVector Location = PathSpline->GetLocationAtDistanceAlongSpline(CurrentDistance, ESplineCoordinateSpace::World);
	FRotator Rotation = PathSpline->GetRotationAtDistanceAlongSpline(CurrentDistance, ESplineCoordinateSpace::World);
	FVector Scale = PathSpline->GetScaleAtDistanceAlongSpline(CurrentDistance);

	// Update the mesh's transform
	ShipRoot->SetWorldLocation(Location);
	ShipRoot->SetWorldRotation(Rotation);
	ShipRoot->SetWorldScale3D(Scale);
}