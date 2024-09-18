// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "GameFramework/Pawn.h"
#include "GameFramework/SpringArmComponent.h"

#include "Components/InputComponent.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"

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

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    USceneComponent* SceneRootComponent;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Components")
    UBoxComponent* ShipCollider;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float ThrustPower;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float TurnRate;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:    

    UPROPERTY(VisibleAnywhere, Category = "Components")
    UStaticMeshComponent* ShipMesh;

    UPROPERTY(VisibleAnywhere, Category = "Components")
    UCameraComponent* Camera;

    UPROPERTY(VisibleAnywhere, Category = "Components")
    USpringArmComponent* SpringArm;

    void MoveForward(const FInputActionValue& Value);
    void Turn(const FInputActionValue& Value);

};
