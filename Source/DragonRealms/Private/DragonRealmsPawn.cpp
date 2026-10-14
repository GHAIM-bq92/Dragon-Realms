// Copyright Epic Games, Inc. All Rights Reserved.

#include "DragonRealmsPawn.h"

#include "Components/InputComponent.h"
#include "GameFramework/Controller.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "BaseMountActor.h"

ADragonRealmsPawn::ADragonRealmsPawn()
{
    PrimaryActorTick.bCanEverTick = true;
    AutoPossessPlayer = EAutoReceiveInput::Player0;

    RootScene = CreateDefaultSubobject<USceneComponent>(TEXT("RootScene"));
    RootComponent = RootScene;

    SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
    SpringArm->SetupAttachment(RootScene);
    SpringArm->TargetArmLength = 480.0f;
    SpringArm->bUsePawnControlRotation = true;
    SpringArm->bEnableCameraLag = true;
    SpringArm->CameraLagSpeed = 6.0f;

    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
    Camera->bUsePawnControlRotation = false;

    bUseControllerRotationYaw = true;
    bUseControllerRotationPitch = true;
    bUseControllerRotationRoll = true;
}

void ADragonRealmsPawn::BeginPlay()
{
    Super::BeginPlay();
    SetActorRotation(FRotator(0.0f, 0.0f, 0.0f));
}

void ADragonRealmsPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, &ADragonRealmsPawn::MoveForward);
    PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, &ADragonRealmsPawn::MoveRight);
    PlayerInputComponent->BindAxis(TEXT("Ascend"), this, &ADragonRealmsPawn::Ascend);
    PlayerInputComponent->BindAxis(TEXT("Turn"), this, &ADragonRealmsPawn::Turn);
    PlayerInputComponent->BindAxis(TEXT("LookUp"), this, &ADragonRealmsPawn::Pitch);
    PlayerInputComponent->BindAction(TEXT("ToggleFlight"), IE_Pressed, this, &ADragonRealmsPawn::ToggleFlightMode);
    PlayerInputComponent->BindAction(TEXT("Boost"), IE_Pressed, this, &ADragonRealmsPawn::Boost);
    PlayerInputComponent->BindAction(TEXT("Boost"), IE_Released, this, &ADragonRealmsPawn::Boost);

    PlayerInputComponent->BindAction(TEXT("Interact"), IE_Pressed, this, &ADragonRealmsPawn::Interact);
}

void ADragonRealmsPawn::MoveForward(float Value)
{
    ForwardInput = Value;
}

void ADragonRealmsPawn::MoveRight(float Value)
{
    RightInput = Value;
}

void ADragonRealmsPawn::Ascend(float Value)
{
    AscendInput = Value;
}

void ADragonRealmsPawn::Turn(float Value)
{
    TurnInput = Value;
}

void ADragonRealmsPawn::Pitch(float Value)
{
    PitchInput = Value;
}

void ADragonRealmsPawn::Roll(float Value)
{
    RollInput = Value;
}

void ADragonRealmsPawn::ToggleFlightMode()
{
    bIsFlying = !bIsFlying;
}

void ADragonRealmsPawn::Boost()
{
    bIsBoosting = !bIsBoosting;
}

void ADragonRealmsPawn::Interact()
{
    // Simple line trace from camera to find a mount actor within reach.
    APlayerController* PC = Cast<APlayerController>(Controller);
    if (!PC) return;

    FVector CamLoc;
    FRotator CamRot;
    PC->GetPlayerViewPoint(CamLoc, CamRot);

    const FVector Start = CamLoc;
    const FVector End = Start + CamRot.Vector() * 500.0f; // 500 units reach

    FHitResult Hit;
    FCollisionQueryParams Params;
    Params.AddIgnoredActor(this);

    if (GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params))
    {
        ABaseMountActor* Mount = Cast<ABaseMountActor>(Hit.GetActor());
        if (Mount)
        {
            // Ask the server to request mount on this target
            Server_RequestMountOnTarget(Mount);
        }
    }
}

void ADragonRealmsPawn::Server_RequestMountOnTarget_Implementation(ABaseMountActor* Target)
{
    if (Target)
    {
        Target->RequestMount(this);
    }
}

void ADragonRealmsPawn::Server_RequestDismountOnTarget_Implementation(ABaseMountActor* Target)
{
    if (Target)
    {
        Target->RequestDismount(this);
    }
}

void ADragonRealmsPawn::OnLocalMounted(AActor* MountActor)
{
    bIsMountedLocally = true;
    CurrentMountActor = MountActor;

    // Optionally adjust camera/spring arm for a better mounted view
    if (SpringArm)
    {
        SpringArm->TargetArmLength = 600.0f;
    }

    // TODO: disable certain pawn controls or visuals while mounted
}

void ADragonRealmsPawn::OnLocalDismounted()
{
    bIsMountedLocally = false;
    CurrentMountActor = nullptr;

    if (SpringArm)
    {
        SpringArm->TargetArmLength = 480.0f;
    }
}

void ADragonRealmsPawn::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (Controller == nullptr)
    {
        return;
    }

    // Detect local attachment to a mount and trigger local hooks
    AActor* Parent = GetAttachParentActor();
    if (Parent && !bIsMountedLocally)
    {
        OnLocalMounted(Parent);
    }
    else if (!Parent && bIsMountedLocally)
    {
        OnLocalDismounted();
    }

    // Yaw and pitch control for dragon flight.
    if (FMath::Abs(TurnInput) > 0.0f)
    {
        AddControllerYawInput(TurnInput * TurnRate * DeltaTime);
    }

    if (FMath::Abs(PitchInput) > 0.0f)
    {
        AddControllerPitchInput(PitchInput * PitchRate * DeltaTime);
    }

    if (FMath::Abs(RollInput) > 0.0f)
    {
        AddControllerRollInput(RollInput * RollRate * DeltaTime);
    }

    if (bIsFlying)
    {
        const float CurrentSpeed = BaseFlightSpeed * (bIsBoosting ? BoostMultiplier : 1.0f);
        const FVector MovementVector =
            (GetActorForwardVector() * ForwardInput) +
            (GetActorRightVector() * RightInput) +
            (GetActorUpVector() * AscendInput);

        const FVector FlightDelta = MovementVector.GetSafeNormal() * CurrentSpeed * DeltaTime;
        AddActorWorldOffset(FlightDelta, true);
    }
}
