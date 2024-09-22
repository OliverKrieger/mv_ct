#include "EndZone.h"

AEndZone::AEndZone()
{
	PrimaryActorTick.bCanEverTick = true;

    SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
    RootComponent = SceneRoot;

    Box1 = CreateDefaultSubobject<UBoxComponent>(TEXT("Box1"));
    Box2 = CreateDefaultSubobject<UBoxComponent>(TEXT("Box2"));
    Box3 = CreateDefaultSubobject<UBoxComponent>(TEXT("Box3"));
    Box4 = CreateDefaultSubobject<UBoxComponent>(TEXT("Box4"));

    Box1->SetupAttachment(RootComponent);
    Box2->SetupAttachment(RootComponent);
    Box3->SetupAttachment(RootComponent);
    Box4->SetupAttachment(RootComponent);

    bHasCollided[0] = false;
    bHasCollided[1] = false;
    bHasCollided[2] = false;
    bHasCollided[3] = false;

    TimerDuration = 5.0f;
    bHasFinished = false;
}

void AEndZone::BeginPlay()
{
	Super::BeginPlay();
	
    Box1->OnComponentBeginOverlap.AddDynamic(this, &AEndZone::OnBoxOverlap);
    Box2->OnComponentBeginOverlap.AddDynamic(this, &AEndZone::OnBoxOverlap);
    Box3->OnComponentBeginOverlap.AddDynamic(this, &AEndZone::OnBoxOverlap);
    Box4->OnComponentBeginOverlap.AddDynamic(this, &AEndZone::OnBoxOverlap);

    Box1->OnComponentEndOverlap.AddDynamic(this, &AEndZone::OnBoxEndOverlap);
    Box2->OnComponentEndOverlap.AddDynamic(this, &AEndZone::OnBoxEndOverlap);
    Box3->OnComponentEndOverlap.AddDynamic(this, &AEndZone::OnBoxEndOverlap);
    Box4->OnComponentEndOverlap.AddDynamic(this, &AEndZone::OnBoxEndOverlap);
}

void AEndZone::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEndZone::OnTimerFinish()
{
    if (!bHasFinished) {
        BP_OnTimerFinish();
        bHasFinished = true;
        UE_LOG(LogTemp, Warning, TEXT("Timer finished, condition triggered!"));
    }
    
}

void AEndZone::OnTimerStarter()
{
    if (!bHasFinished) {
        BP_OnTimerStarter();
        UE_LOG(LogTemp, Warning, TEXT("All boxes collided! Starting timer."));
    }
}

void AEndZone::OnTimerCancelled()
{
    if (!bHasFinished) {
        BP_OnTimerCancelled();
        UE_LOG(LogTemp, Warning, TEXT("Timer cancelled as a box is no longer collided!"));
    }
}

void AEndZone::OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor && OtherActor->IsA(APawn::StaticClass()))
    {
        if (OverlappedComponent == Box1 && !bHasCollided[0])
        {
            bHasCollided[0] = true;
            UE_LOG(LogTemp, Warning, TEXT("Box 1 collided!"));
        }
        else if (OverlappedComponent == Box2 && !bHasCollided[1])
        {
            bHasCollided[1] = true;
            UE_LOG(LogTemp, Warning, TEXT("Box 2 collided!"));
        }
        else if (OverlappedComponent == Box3 && !bHasCollided[2])
        {
            bHasCollided[2] = true;
            UE_LOG(LogTemp, Warning, TEXT("Box 3 collided!"));
        }
        else if (OverlappedComponent == Box4 && !bHasCollided[3])
        {
            bHasCollided[3] = true;
            UE_LOG(LogTemp, Warning, TEXT("Box 4 collided!"));
        }

        if (bHasCollided[0] && bHasCollided[1] && bHasCollided[2] && bHasCollided[3])
        {
            OnTimerStarter();
            GetWorld()->GetTimerManager().SetTimer(CollisionTimerHandle, this, &AEndZone::OnTimerFinish, TimerDuration, false);
        }
    }
}

void AEndZone::OnBoxEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if (OtherActor && OtherActor->IsA(APawn::StaticClass()))
    {
        if (OverlappedComponent == Box1)
        {
            bHasCollided[0] = false;
            UE_LOG(LogTemp, Warning, TEXT("Box 1 no longer collided!"));
        }
        else if (OverlappedComponent == Box2)
        {
            bHasCollided[1] = false;
            UE_LOG(LogTemp, Warning, TEXT("Box 2 no longer collided!"));
        }
        else if (OverlappedComponent == Box3)
        {
            bHasCollided[2] = false;
            UE_LOG(LogTemp, Warning, TEXT("Box 3 no longer collided!"));
        }
        else if (OverlappedComponent == Box4)
        {
            bHasCollided[3] = false;
            UE_LOG(LogTemp, Warning, TEXT("Box 4 no longer collided!"));
        }

        if (!(bHasCollided[0] && bHasCollided[1] && bHasCollided[2] && bHasCollided[3]))
        {
            GetWorld()->GetTimerManager().ClearTimer(CollisionTimerHandle);
            OnTimerCancelled();
        }
    }
}

