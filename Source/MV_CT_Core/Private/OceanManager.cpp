#include "OceanManager.h"

AOceanManager::AOceanManager()
{
	PrimaryActorTick.bCanEverTick = true;

    static ConstructorHelpers::FObjectFinder<UBlueprint> BlueprintObj(TEXT("Blueprint'/Game/Blueprints/BP_OceanBody.BP_OceanBody'"));
    if (BlueprintObj.Succeeded())
    {
        OceanBodyClass = (UClass*)BlueprintObj.Object->GeneratedClass;
    }
}

void AOceanManager::BeginPlay()
{
	Super::BeginPlay();
	
    if (bUseExpensiveWater) {
        SpawnOceanGrid();
    }
}

void AOceanManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AOceanManager::SpawnOceanGrid()
{
    TArray<TArray<AOceanBody*>> OceanGrid2D;
    OceanGrid2D.SetNum(GridSize);
    OceanGrid1D.SetNum(GridSize * GridSize);
    FVector GridOrigin = GetActorLocation();

    for (int32 Row = 0; Row < GridSize; ++Row)
    {
        OceanGrid2D[Row].SetNum(GridSize);
        for (int32 Col = 0; Col < GridSize; ++Col)
        {
            FVector SpawnLocation = GridOrigin + FVector(Row * PlaneSpacing, Col * PlaneSpacing, 0.0f);
            AOceanBody* SpawnedPlane = GetWorld()->SpawnActor<AOceanBody>(OceanBodyClass, SpawnLocation, FRotator::ZeroRotator);

            if (SpawnedPlane)
            {
                OceanGrid2D[Row][Col] = SpawnedPlane;
                OceanGrid1D[Row * GridSize + Col] = SpawnedPlane;
            }
        }
    }
}

