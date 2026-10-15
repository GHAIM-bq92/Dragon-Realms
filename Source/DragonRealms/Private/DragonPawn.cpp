#include "DragonPawn.h"
#include "Components/SkeletalMeshComponent.h"
#include "MountComponent.h"
#include "Net/UnrealNetwork.h"

ADragonPawn::ADragonPawn()
{
    PrimaryActorTick.bCanEverTick = true;
    bReplicates = true;

    SkeletalMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMeshComp"));
    RootComponent = SkeletalMeshComp;
    SkeletalMeshComp->SetIsReplicated(true);

    MountComp = CreateDefaultSubobject<UMountComponent>(TEXT("MountComp"));

    RiderSocketName = FName(TEXT("RiderSocket"));

    BaseFlightSpeed = 1200.0f;
    BoostMultiplier = 1.9f;
    TurnRate = 120.0f;
    PitchRate = 90.0f;

    bIsFlying = false;
    CurrentSpeed = 0.0f;
    VerticalSpeed = 0.0f;

    CachedForward = CachedRight = CachedAscend = CachedTurn = CachedPitch = 0.0f;
    bCachedBoosting = false;
}

void ADragonPawn::BeginPlay()
{
    Super::BeginPlay();
}

void ADragonPawn::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (HasAuthority())
    {
        ApplyServerMovement(DeltaTime);
    }
}

void ADragonPawn::ApplyServerMovement(float DeltaTime)
{
    // Simple flight movement driven by cached inputs (server authoritative)
    const float Speed = BaseFlightSpeed * (bCachedBoosting ? BoostMultiplier : 1.0f);
    FVector MovementVector = (GetActorForwardVector() * CachedForward) + (GetActorRightVector() * CachedRight) + (GetActorUpVector() * CachedAscend);
    if (!MovementVector.IsNearlyZero())
    {
        FVector Delta = MovementVector.GetSafeNormal() * Speed * DeltaTime;
        AddActorWorldOffset(Delta, true);
    }

    // Rotation from cached turn/pitch
    if (FMath::Abs(CachedTurn) > KINDA_SMALL_NUMBER)
    {
        AddActorLocalRotation(FRotator(0.0f, CachedTurn * TurnRate * DeltaTime, 0.0f));
    }
    if (FMath::Abs(CachedPitch) > KINDA_SMALL_NUMBER)
    {
        AddActorLocalRotation(FRotator(CachedPitch * PitchRate * DeltaTime, 0.0f, 0.0f));
    }

    // Update replicated animation state
    CurrentSpeed = GetVelocity().Size();
    VerticalSpeed = GetVelocity().Z;
    bIsFlying = !GetMovementComponent() || !GetMovementComponent()->IsMovingOnGround() || FMath::Abs(VerticalSpeed) > 5.0f;
}

void ADragonPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
    Super::SetupPlayerInputComponent(PlayerInputComponent);

    // Bindings will be handled by controller after possession; implement generic movement forwarding
    PlayerInputComponent->BindAxis(TEXT("MoveForward"), this, [this](float Val){ if (IsLocallyControlled()) Server_UpdateMovement(Val, CachedRight, CachedAscend, CachedTurn, CachedPitch, bCachedBoosting); });
    PlayerInputComponent->BindAxis(TEXT("MoveRight"), this, [this](float Val){ if (IsLocallyControlled()) Server_UpdateMovement(CachedForward, Val, CachedAscend, CachedTurn, CachedPitch, bCachedBoosting); });
    PlayerInputComponent->BindAxis(TEXT("Ascend"), this, [this](float Val){ if (IsLocallyControlled()) Server_UpdateMovement(CachedForward, CachedRight, Val, CachedTurn, CachedPitch, bCachedBoosting); });
    PlayerInputComponent->BindAxis(TEXT("Turn"), this, [this](float Val){ if (IsLocallyControlled()) Server_UpdateMovement(CachedForward, CachedRight, CachedAscend, Val, CachedPitch, bCachedBoosting); });
    PlayerInputComponent->BindAxis(TEXT("LookUp"), this, [this](float Val){ if (IsLocallyControlled()) Server_UpdateMovement(CachedForward, CachedRight, CachedAscend, CachedTurn, Val, bCachedBoosting); });

    PlayerInputComponent->BindAction(TEXT("Boost"), IE_Pressed, this, [this](){ if (IsLocallyControlled()) Server_UpdateMovement(CachedForward, CachedRight, CachedAscend, CachedTurn, CachedPitch, true); });
    PlayerInputComponent->BindAction(TEXT("Boost"), IE_Released, this, [this](){ if (IsLocallyControlled()) Server_UpdateMovement(CachedForward, CachedRight, CachedAscend, CachedTurn, CachedPitch, false); });
}

void ADragonPawn::Server_UpdateMovement_Implementation(float Forward, float Right, float Ascend, float TurnInput, float PitchInput, bool bBoosting)
{
    // Store inputs on server; movement applied in Tick
    CachedForward = FMath::Clamp(Forward, -1.0f, 1.0f);
    CachedRight = FMath::Clamp(Right, -1.0f, 1.0f);
    CachedAscend = FMath::Clamp(Ascend, -1.0f, 1.0f);
    CachedTurn = TurnInput;
    CachedPitch = PitchInput;
    bCachedBoosting = bBoosting;
}

void ADragonPawn::OnRep_IsFlying()
{
    // client-side hook
}

void ADragonPawn::OnRep_Speed()
{
    // client-side hook
}

void ADragonPawn::OnRep_VerticalSpeed()
{
    // client-side hook
}

void ADragonPawn::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ADragonPawn, bIsFlying);
    DOREPLIFETIME(ADragonPawn, CurrentSpeed);
    DOREPLIFETIME(ADragonPawn, VerticalSpeed);
}
