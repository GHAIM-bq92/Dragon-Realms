#include "DragonMountActor.h"
#include "Components/SkeletalMeshComponent.h"

ADragonMountActor::ADragonMountActor()
{
    // Create skeletal mesh and attach to root (BaseMountActor already creates a static MeshComp)
    SkeletalMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComp"));
    if (SkeletalMeshComp)
    {
        SkeletalMeshComp->SetupAttachment(RootComponent);
        SkeletalMeshComp->SetIsReplicated(true);
        SkeletalMeshComp->SetCollisionProfileName(TEXT("Pawn"));
    }

    RiderSocketName = FName(TEXT("RiderSocket"));
}

void ADragonMountActor::BeginPlay()
{
    Super::BeginPlay();
    // Additional initialization for dragon-specific behavior can go here
}
