// Fill out your copyright notice in the Description page of Project Settings.


#include "ShipPawn.h"
#include "MV_CoreLogCategory.h"

DEFINE_LOG_CATEGORY(MV_CoreLogCategory);

// Sets default values
AShipPawn::AShipPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	SceneRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRootComponent"));
	RootComponent = SceneRootComponent;

	ShipMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShipMesh"));
	ShipMesh->SetupAttachment(RootComponent);

	ShipCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("ShipCollider"));
	ShipCollider->SetBoxExtent(FVector(50.0f, 50.0f, 50.0f));
	ShipCollider->SetCollisionProfileName(TEXT("Pawn"));
	ShipCollider->SetupAttachment(ShipMesh);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(ShipMesh);
	SpringArm->TargetArmLength = 400.0f;
	SpringArm->bEnableCameraLag = true;
	SpringArm->CameraLagSpeed = 3.0f;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false;

	ThrustPower = 1000.f;
	TurnRate = 50.f;

}

// Called when the game starts or when spawned
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

// Called every frame
void AShipPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AShipPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveForwardInputAction, ETriggerEvent::Triggered, this, &AShipPawn::MoveForward);
		EnhancedInputComponent->BindAction(MoveRightInputAction, ETriggerEvent::Triggered, this, &AShipPawn::Turn);
	}
}

// Function to move the pawn forward
void AShipPawn::MoveForward(const FInputActionValue& Value)
{
	float MoveValue = Value.Get<float>();
	UE_LOG(MV_CoreLogCategory, Log, TEXT("MoveForward Called"));

	if (MoveValue != 0.0f)
	{
		UE_LOG(MV_CoreLogCategory, Log, TEXT("Trusting"));
		FVector Force = GetActorForwardVector() * MoveValue * ThrustPower;
		ShipMesh->AddForce(Force);
	}
}

// Function to turn the pawn
void AShipPawn::Turn(const FInputActionValue& Value)
{
	float MoveValue = Value.Get<float>();
	UE_LOG(MV_CoreLogCategory, Log, TEXT("Turn Called"));

	if (MoveValue != 0.0f)
	{
		UE_LOG(MV_CoreLogCategory, Log, TEXT("Turning"));
		FRotator Rotation = FRotator(0.f, MoveValue * TurnRate, 0.f);
		ShipMesh->AddTorqueInRadians(Rotation.Vector() * 1000.f);
	}
}