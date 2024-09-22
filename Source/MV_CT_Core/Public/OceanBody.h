// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "OceanBody.generated.h"

UCLASS()
class MV_CT_CORE_API AOceanBody : public AActor
{
	GENERATED_BODY()
	
public:	
	AOceanBody();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Environment")
	float WaterDensity = 10.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Environment")
	float WaterLevel = 0.f;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;
};
