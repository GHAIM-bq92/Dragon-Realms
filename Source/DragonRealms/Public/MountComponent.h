#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MountComponent.generated.h"

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class DRAGONREALMS_API UMountComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    UMountComponent();

    // Request mount (called by client) -> runs on server
    UFUNCTION(Server, Reliable)
    void Server_RequestMount(AActor* Rider);

    // Request dismount (called by client) -> runs on server
    UFUNCTION(Server, Reliable)
    void Server_RequestDismount(AActor* Rider);

    // Whether this mount is currently occupied
    UPROPERTY(ReplicatedUsing=OnRep_IsMounted)
    bool bIsMounted;

    // The actor riding this mount (usually a player pawn)
    UPROPERTY(Replicated)
    TWeakObjectPtr<AActor> RiderActor;

protected:
    virtual void BeginPlay() override;

    UFUNCTION()
    void OnRep_IsMounted();

    // Called on all clients when mount state changes
    UFUNCTION(NetMulticast, Reliable)
    void Multicast_OnMounted(AActor* NewRider);

    UFUNCTION(NetMulticast, Reliable)
    void Multicast_OnDismounted();

public:
    // Attach the rider to the mount on the server (authoritative)
    void Mount(AActor* NewRider);
    void Dismount();

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
