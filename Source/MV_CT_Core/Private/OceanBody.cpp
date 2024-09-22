// Fill out your copyright notice in the Description page of Project Settings.


#include "OceanBody.h"

AOceanBody::AOceanBody()
{
	// Does not need
	PrimaryActorTick.bCanEverTick = false;

	// Set variables
	WaterLevel = GetActorLocation().Z;
}

void AOceanBody::BeginPlay()
{
	Super::BeginPlay();
}

void AOceanBody::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

