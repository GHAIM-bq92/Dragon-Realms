#include "MountComponent.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/Actor.h"
#include "DragonMountActor.h"
#include "Components/SkeletalMeshComponent.h"

UMountComponent::UMountComponent()
{
    SetIsReplicatedByDefault(true);
    bIsMounted = false;
    RiderActor = nullptr;
}

void UMountComponent::BeginPlay()
{
    Super::BeginPlay();
}

void UMountComponent::Server_RequestMount_Implementation(AActor* Rider)
{
    if (GetOwner()->HasAuthority() && !bIsMounted && Rider)
    {
        Mount(Rider);
    }
}

void UMountComponent::Server_RequestDismount_Implementation(AActor* Rider)
{
    if (GetOwner()->HasAuthority() && bIsMounted && Rider && RiderActor.Get() == Rider)
    {
        Dismount();
    }
}

void UMountComponent::Mount(AActor* NewRider)
{
    if (!GetOwner()->HasAuthority()) return;

    bIsMounted = true;
    RiderActor = NewRider;

    // Attach the rider to the mount actor on the server
    if (NewRider && GetOwner())
    {
        // If the mount is a DragonMountActor, attach to its SkeletalMesh socket for a precise rider transform
        ADragonMountActor* Dragon = Cast<ADragonMountActor>(GetOwner());
        if (Dragon && Dragon->SkeletalMeshComp)
        {
            FName SocketName = Dragon->RiderSocketName;
            NewRider->AttachToComponent(Dragon->SkeletalMeshComp, FAttachmentTransformRules::SnapToTargetNotIncludingScale, SocketName);
        }
        else
        {
            // Fallback: attach to the actor root
            FAttachmentTransformRules Rules(EAttachmentRule::SnapToTarget, true);
            NewRider->AttachToActor(GetOwner(), Rules, NAME_None);
        }
    }

    Multicast_OnMounted(NewRider);
}

void UMountComponent::Dismount()
{
    if (!GetOwner()->HasAuthority()) return;

    AActor* OldRider = RiderActor.Get();
    bIsMounted = false;
    RiderActor = nullptr;

    if (OldRider)
    {
        OldRider->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
    }

    Multicast_OnDismounted();
}

void UMountComponent::OnRep_IsMounted()
{
    // Client-side hooks can go here (play animation, disable input, etc.)
}

void UMountComponent::Multicast_OnMounted_Implementation(AActor* NewRider)
{
    // Play mount animations / VFX on all clients
    // Example: notify an AnimInstance or blueprint event
}

void UMountComponent::Multicast_OnDismounted_Implementation()
{
    // Play dismount animations / VFX on all clients
}

void UMountComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(UMountComponent, bIsMounted);
    DOREPLIFETIME(UMountComponent, RiderActor);
}
