// Fill out your copyright notice in the Description page of Project Settings.


#include "ShipPawn.h"

#include "Kismet/GameplayStatics.h"

#include "MV_CoreLogCategory.h"

DEFINE_LOG_CATEGORY(MV_CoreLogCategory);

// Sets default values
AShipPawn::AShipPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	// Ship
	ShipCollider = CreateDefaultSubobject<UBoxComponent>(TEXT("ShipCollider"));
	ShipCollider->SetBoxExtent(FVector(50.0f, 50.0f, 50.0f));
	ShipCollider->SetNotifyRigidBodyCollision(true);
	ShipCollider->SetCollisionProfileName(TEXT("Pawn"));
	RootComponent = ShipCollider;
	MoveableComponent = ShipCollider;

	ShipMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ShipMesh"));
	ShipMesh->SetupAttachment(RootComponent);

	// Camera
	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArm->SetupAttachment(ShipMesh);
	SpringArm->TargetArmLength = 1000.0f;
	SpringArm->SocketOffset = FVector(0, 0, 300.0f);
	SpringArm->bEnableCameraLag = true;
	SpringArm->CameraLagSpeed = 3.0f;

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
	Camera->bUsePawnControlRotation = false;

	// Find Inputs
	static ConstructorHelpers::FObjectFinder<UInputAction> MoveActionAsset(TEXT("/Game/Inputs/IA_MoveForward"));
	if (MoveActionAsset.Succeeded())
	{
		MoveForwardInputAction = MoveActionAsset.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> TurnActionAsset(TEXT("/Game/Inputs/IA_TurnRight"));
	if (TurnActionAsset.Succeeded())
	{
		MoveRightInputAction = TurnActionAsset.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputMappingContext> MappingContextAsset(TEXT("/Game/Inputs/IMC_PlayerControls"));
	if (MappingContextAsset.Succeeded())
	{
		InputMappingContext = MappingContextAsset.Object;
	}
}

void AShipPawn::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(MV_CoreLogCategory, Log, TEXT("Ship Has Spawned"));

	// Setup Input Mapping
	if (APlayerController* PC = Cast<APlayerController>(GetController()))
	{
		UEnhancedInputLocalPlayerSubsystem* InputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer());

		if (InputMappingContext)
		{
			InputSubsystem->AddMappingContext(InputMappingContext, 1);
		}
	}

	// Setup ocean refrence
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AOceanBody::StaticClass(), FoundActors);
	if (FoundActors.Num() > 0)
	{
		Ocean = Cast<AOceanBody>(FoundActors[0]); // Get first actor of ocean (should always only be 1)
		if (!Ocean)
		{
			UE_LOG(MV_CoreLogCategory, Error, TEXT("Failed to cast FoundActor to AOceanBody!"));
		}
	}
	else
	{
		UE_LOG(MV_CoreLogCategory, Warning, TEXT("No AOceanBody actors found in the level!"));
	}

	// Setup collision checks
	if (ShipCollider)
	{
		ShipCollider->OnComponentHit.AddDynamic(this, &AShipPawn::OnPawnCollision);
	}
}

void AShipPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	Delta = DeltaTime;

	UpdateMovement();
}

void AShipPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveForwardInputAction, ETriggerEvent::Triggered, this, &AShipPawn::TriggerForwardInput);
		EnhancedInputComponent->BindAction(MoveRightInputAction, ETriggerEvent::Triggered, this, &AShipPawn::TriggerTurnInput);
		EnhancedInputComponent->BindAction(MoveForwardInputAction, ETriggerEvent::Completed, this, &AShipPawn::ReleaseForwardInput);
		EnhancedInputComponent->BindAction(MoveRightInputAction, ETriggerEvent::Completed, this, &AShipPawn::ReleaseTurnInput);
	}
}

void AShipPawn::OnPawnCollision(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// handles linear velocity on collision
	if (OtherActor->IsA(AObstacleShip::StaticClass()) || OtherActor->IsA(AIsland::StaticClass()))
	{
		UE_LOG(MV_CoreLogCategory, Log, TEXT("Collided with Obstacle Ship!"));

		FVector HitNormal = Hit.ImpactNormal;
		FVector ShipForwardVector = GetActorForwardVector();

		HitNormal.Normalize();
		ShipForwardVector.Normalize();

		float HitAngle = FMath::Abs(FVector::DotProduct(ShipForwardVector, HitNormal));

		LinearVelocity = -LinearVelocity * HitAngle;
	}
}

/* **************************** */
/* INPUTS */
/* **************************** */

void AShipPawn::TriggerForwardInput(const FInputActionValue& Value)
{
	ThrottleInput = Value.Get<float>();
}

void AShipPawn::TriggerTurnInput(const FInputActionValue& Value)
{
	SteeringInput = Value.Get<float>();
}

void AShipPawn::ReleaseForwardInput(const FInputActionValue& Value)
{
	ThrottleInput = 0.0f;
}

void AShipPawn::ReleaseTurnInput(const FInputActionValue& Value)
{
	SteeringInput = 0.0f;
}

/* **************************** */
/* HORIZONTAL FORCES */
/* **************************** */

void AShipPawn::UpdateForwardMovement()
{
	
	if (ThrottleInput != 0.0f) {
		LinearVelocity = FMath::FInterpTo(LinearVelocity, MaxThrottlePower * ThrottleInput, Delta, ThrottlePower);
	}
	else {
		LinearVelocity = FMath::FInterpTo(LinearVelocity, 0.0f, Delta, LinearDampening);
	}

	if (PreviousFowardSpeed != LinearVelocity) { // so we do not needlessly perform the calculations
		//UE_LOG(MV_CoreLogCategory, Log, TEXT("Throttle Input: %f | Forward Speed: %f"), ThrottleInput, LinearVelocity);
		FVector ForwardVector = GetActorForwardVector();
		FVector Movement = ForwardVector * LinearVelocity * Delta;
		AddActorWorldOffset(Movement, true);
		PreviousFowardSpeed = LinearVelocity;
	}
}

void AShipPawn::UpdateTurningMovement()
{
	if (SteeringInput != 0.0f)
	{
		AngularVelocity = FMath::FInterpTo(AngularVelocity, MaxTurnRate * SteeringInput, Delta, TurnRate);
	}
	else {
		AngularVelocity = FMath::FInterpTo(AngularVelocity, 0.0f, Delta, AngularDampening);
	}

	if (PreviousTurnSpeed != AngularVelocity) {
		//UE_LOG(MV_CoreLogCategory, Log, TEXT("Steering Input: %f | Rotation Amount: %f"), SteeringInput, AngularVelocity);
		float RotationAmount = AngularVelocity * Delta;
		FRotator YawRotation = FRotator(0.0f, RotationAmount, 0.0f);
		AddActorWorldRotation(YawRotation);
		PreviousTurnSpeed = AngularVelocity;
	}
}

void AShipPawn::UpdateMovement()
{
	UpdateForwardMovement();
	UpdateTurningMovement();
}

/* **************************** */
/* LINE TRACES */
/* **************************** */
bool AShipPawn::IsInAir()
{
	FVector Start = GetActorLocation();
	FVector End = Start - FVector(0.f, 0.f, 1.f);

	FHitResult HitResult;
	FCollisionQueryParams CollisionParams;

	bool bHit = GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, CollisionParams);

	return !bHit;
}

bool AShipPawn::isSubmerged()
{
	FVector StartPoint = GetActorLocation();
	FVector EndPoint = StartPoint - FVector(0.0f, 0.0f, 20.0f);

	FHitResult HitResult;
	FCollisionQueryParams TraceParams(FName(TEXT("WaterTrace")), true, this);

	bool bHit = GetWorld()->LineTraceSingleByChannel(
		HitResult,
		StartPoint,
		EndPoint,
		ECC_Visibility,
		TraceParams
	);

	if (bHit)
	{
		Ocean = Cast<AOceanBody>(HitResult.GetActor());
		if (Ocean)
		{
			UE_LOG(MV_CoreLogCategory, Log, TEXT("Boat is colliding with AOceanBody"));
			return true;
		}
	}
	return false;
}

