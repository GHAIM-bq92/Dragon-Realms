#include "BaseMountActor.h"
#include "Components/StaticMeshComponent.h"
#include "MountComponent.h"

ABaseMountActor::ABaseMountActor()
{
    PrimaryActorTick.bCanEverTick = false;
    bReplicates = true;

    MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
    RootComponent = MeshComp;
    MeshComp->SetIsReplicated(true);

    MountComp = CreateDefaultSubobject<UMountComponent>(TEXT("MountComp"));
}

void ABaseMountActor::BeginPlay()
{
    Super::BeginPlay();
}

void ABaseMountActor::RequestMount(AActor* RequestingPawn)
{
    if (MountComp)
    {
        MountComp->Server_RequestMount(RequestingPawn);
    }
}

void ABaseMountActor::RequestDismount(AActor* RequestingPawn)
{
    if (MountComp)
    {
        MountComp->Server_RequestDismount(RequestingPawn);
    }
}
