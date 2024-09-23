// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Pawn.h"
#include "GameFramework/SpringArmComponent.h"

#include "Components/InputComponent.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "Components/PrimitiveComponent.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputAction.h"
#include "InputMappingContext.h"

#include "Camera/CameraComponent.h"

#include "OceanBody.h"
#include "ObstacleShip.h"
#include "Island.h"

#include "ShipPawn.generated.h"

UCLASS()
class MV_CT_CORE_API AShipPawn : public APawn
{
	GENERATED_BODY()

public:
	AShipPawn();

    // INPUT ACTIONS
    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Input")
    TObjectPtr<UInputMappingContext> InputMappingContext;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Input")
    TObjectPtr<UInputAction> MoveForwardInputAction;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Input")
    TObjectPtr<UInputAction> MoveRightInputAction;

    // COMPONENTS
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    UBoxComponent* ShipCollider;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    UStaticMeshComponent* ShipMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    UCameraComponent* Camera;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    USpringArmComponent* SpringArm;

    // Manual Movement Params
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float ThrottlePower = 2.5f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float TurnRate = 2.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float MaxThrottlePower = 2033.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float MaxTurnRate = 20.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float GravityForce = 980.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float LinearDampening = 1.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float AngularDampening = 1.0f;

    // Manual Size Params
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boat Size")
    float ShipMass = 145.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boat Size")
    float BoatLength = 500.f;
    
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boat Size")
    float BoatWidth = 300.f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Boat Size")
    float BoatHeight = 200.f;

    AOceanBody* Ocean;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    UFUNCTION()
    void OnPawnCollision(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

private:    
    // inputs
    void TriggerForwardInput(const FInputActionValue& Value);
    void TriggerTurnInput(const FInputActionValue& Value);
    void ReleaseForwardInput(const FInputActionValue& Value);
    void ReleaseTurnInput(const FInputActionValue& Value);
    
    // movement
    void UpdateForwardMovement();
    void UpdateTurningMovement();
    void UpdateMovement();

    float Delta;

    float LinearVelocity = 0.0f;
    float AngularVelocity = 0.0f;
    float VerticalVelocity = 0.0f;
    
    float ThrottleInput = 0.0f;
    float SteeringInput = 0.0f;

    UPROPERTY(VisibleAnywhere, Category = "Components")
    UPrimitiveComponent* MoveableComponent;
};
