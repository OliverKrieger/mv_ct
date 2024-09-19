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

#include "ShipPawn.generated.h"

UCLASS()
class MV_CT_CORE_API AShipPawn : public APawn
{
	GENERATED_BODY()

public:
	AShipPawn();

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Input")
    TObjectPtr<UInputMappingContext> InputMappingContext;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Input")
    TObjectPtr<UInputAction> MoveForwardInputAction;

    UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Input")
    TObjectPtr<UInputAction> MoveRightInputAction;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    UBoxComponent* ShipCollider;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    UStaticMeshComponent* ShipMesh;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    UCameraComponent* Camera;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    USpringArmComponent* SpringArm;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float ThrustPower;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float TurnRate;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float GravityScale;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:    
    void MoveForward(const FInputActionValue& Value);
    void Turn(const FInputActionValue& Value);
    void ApplyGravity();
    bool IsInAir() const;

    UPROPERTY(VisibleAnywhere, Category = "Components")
    UPrimitiveComponent* MoveableComponent;
};
