// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "DragonRealmsPawn.generated.h"

class ABaseMountActor;

UCLASS()
class DRAGONREALMS_API ADragonRealmsPawn : public APawn
{
    GENERATED_BODY()

public:
    ADragonRealmsPawn();

    virtual void Tick(float DeltaTime) override;
    virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
    virtual void BeginPlay() override;

    void MoveForward(float Value);
    void MoveRight(float Value);
    void Ascend(float Value);
    void Turn(float Value);
    void Pitch(float Value);
    void Roll(float Value);
    void ToggleFlightMode();
    void Boost();

    // Interaction
    void Interact();

    UFUNCTION(Server, Reliable)
    void Server_RequestMountOnTarget(ABaseMountActor* Target);

    UFUNCTION(Server, Reliable)
    void Server_RequestDismountOnTarget(ABaseMountActor* Target);

    // Called locally when we detect we are attached to a mount
    void OnLocalMounted(AActor* MountActor);
    void OnLocalDismounted();

public:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    TObjectPtr<class USceneComponent> RootScene;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    TObjectPtr<class USpringArmComponent> SpringArm;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    TObjectPtr<UCameraComponent> Camera;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flight")
    float BaseFlightSpeed = 1200.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flight")
    float BoostMultiplier = 1.9f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flight")
    float TurnRate = 120.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flight")
    float PitchRate = 90.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flight")
    float RollRate = 90.0f;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Flight")
    float LiftRate = 120.0f;

private:
    float ForwardInput = 0.0f;
    float RightInput = 0.0f;
    float AscendInput = 0.0f;
    float TurnInput = 0.0f;
    float PitchInput = 0.0f;
    float RollInput = 0.0f;
    bool bIsFlying = true;
    bool bIsBoosting = false;

    // Mounting state (client-side observation)
    bool bIsMountedLocally = false;
    TWeakObjectPtr<AActor> CurrentMountActor;
};
