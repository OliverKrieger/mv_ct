// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Kismet/GameplayStatics.h"

#include "OceanBody.h"

#include "OceanManager.generated.h"

UCLASS()
class MV_CT_CORE_API AOceanManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AOceanManager();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Environment")
	bool bUseExpensiveWater = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Environment")
	int32 GridSize = 3;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Environment")
	float PlaneSpacing = 20000.f;

	TSubclassOf<AActor> OceanBodyClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Environment")
	TArray<AOceanBody*> OceanGrid1D;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SpawnOceanGrid();
};
