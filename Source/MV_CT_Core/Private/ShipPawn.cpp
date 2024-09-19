// Fill out your copyright notice in the Description page of Project Settings.


#include "ShipPawn.h"
#include "MV_CoreLogCategory.h"

DEFINE_LOG_CATEGORY(MV_CoreLogCategory);

// Sets default values
AShipPawn::AShipPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	ShipCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("ShipCollider"));
	ShipCollider->SetBoxExtent(FVector(50.0f, 50.0f, 50.0f));
	ShipCollider->SetCollisionProfileName(TEXT("Pawn"));
	RootComponent = ShipCollider;
	MoveableComponent = ShipCollider;

	ShipMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShipMesh"));
	ShipMesh->SetupAttachment(RootComponent);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(ShipMesh);
	SpringArm->TargetArmLength = 16000.0f;
	SpringArm->SocketOffset = FVector(0, 0, 5200.0f);
	SpringArm->bEnableCameraLag = true;
	SpringArm->CameraLagSpeed = 3.0f;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false;

	// Ship weight recognised as 2487627kg
	// Movement suggested as following
	MoveableComponent->SetSimulatePhysics(true);
	ThrustPower = 5000000.f;
	TurnRate = 5000000.f;
	MoveableComponent->SetLinearDamping(0.3f);
	MoveableComponent->SetAngularDamping(1.0f);
	MoveableComponent->SetEnableGravity(true);
}

void AShipPawn::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(MV_CoreLogCategory, Log, TEXT("Ship Has Spawned"));

	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		UEnhancedInputLocalPlayerSubsystem* InputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer());

		if (InputMappingContext)
		{
			InputSubsystem->AddMappingContext(InputMappingContext, 1);
		}
	}
	
}

void AShipPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AShipPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveForwardInputAction, ETriggerEvent::Triggered, this, &AShipPawn::MoveForward);
		EnhancedInputComponent->BindAction(MoveRightInputAction, ETriggerEvent::Triggered, this, &AShipPawn::Turn);
	}
}

void AShipPawn::MoveForward(const FInputActionValue& Value)
{
	float MoveValue = Value.Get<float>();

	if (MoveValue != 0.0f)
	{
		UE_LOG(MV_CoreLogCategory, Log, TEXT("Trusting"));
		FVector Force = GetActorForwardVector() * MoveValue * ThrustPower;
		MoveableComponent->AddForce(Force);
	}
}

void AShipPawn::Turn(const FInputActionValue& Value)
{
	float MoveValue = Value.Get<float>();

	if (MoveValue != 0.0f)
	{
		UE_LOG(MV_CoreLogCategory, Log, TEXT("Turning"));
		FVector TurnValue = FVector(0.f, 0.f, MoveValue * TurnRate);
		MoveableComponent->AddTorqueInRadians(TurnValue);
	}
}