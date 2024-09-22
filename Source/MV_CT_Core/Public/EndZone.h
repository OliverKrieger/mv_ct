// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "Components/SceneComponent.h"
#include "Components/BoxComponent.h"

#include "EndZone.generated.h"

UCLASS()
class MV_CT_CORE_API AEndZone : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEndZone();

	UPROPERTY(VisibleAnywhere)
	USceneComponent* SceneRoot;

	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* Box1;

	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* Box2;

	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* Box3;

	UPROPERTY(VisibleAnywhere)
	class UBoxComponent* Box4;

	bool bHasCollided[4];
	FTimerHandle CollisionTimerHandle;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Timing")
	float TimerDuration;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Reload")
	bool bHasFinished = false;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintImplementableEvent)
	void BP_OnTimerFinish();
	
	UFUNCTION(BlueprintImplementableEvent)
	void BP_OnTimerStarter();

	UFUNCTION(BlueprintImplementableEvent)
	void BP_OnTimerCancelled();
	
	void OnTimerFinish();
	void OnTimerStarter();
	void OnTimerCancelled();

	UFUNCTION()
	void OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
