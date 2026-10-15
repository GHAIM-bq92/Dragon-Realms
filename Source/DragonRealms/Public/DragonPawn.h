#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "DragonPawn.generated.h"

class USkeletalMeshComponent;
class UMountComponent;

UCLASS()
class DRAGONREALMS_API ADragonPawn : public APawn
{
    GENERATED_BODY()

public:
    ADragonPawn();

    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

    // Server input RPC: send local input to server for authoritative movement
    UFUNCTION(Server, Reliable)
    void Server_UpdateMovement(float Forward, float Right, float Ascend, float TurnInput, float PitchInput, bool bBoosting);

    // Replicated animation/state properties
    UPROPERTY(ReplicatedUsing=OnRep_IsFlying, BlueprintReadOnly, Category = "Animation")
    bool bIsFlying;

    UPROPERTY(ReplicatedUsing=OnRep_Speed, BlueprintReadOnly, Category = "Animation")
    float CurrentSpeed;

    UPROPERTY(ReplicatedUsing=OnRep_VerticalSpeed, BlueprintReadOnly, Category = "Animation")
    float VerticalSpeed;

    UFUNCTION()
    void OnRep_IsFlying();

    UFUNCTION()
    void OnRep_Speed();

    UFUNCTION()
    void OnRep_VerticalSpeed();

    // Skeletal mesh and mount component
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    USkeletalMeshComponent* SkeletalMeshComp;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UMountComponent* MountComp;

    // Socket name to attach rider
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mount")
    FName RiderSocketName;

    // Movement tuning
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float BaseFlightSpeed;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float BoostMultiplier;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float TurnRate;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Movement")
    float PitchRate;

protected:
    // Cached inputs on server
    float CachedForward;
    float CachedRight;
    float CachedAscend;
    float CachedTurn;
    float CachedPitch;
    bool bCachedBoosting;

    // Apply movement on server
    void ApplyServerMovement(float DeltaTime);

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
