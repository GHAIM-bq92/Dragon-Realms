#include "DragonAnimInstance.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/Character.h"

void UDragonAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
    Super::NativeUpdateAnimation(DeltaSeconds);

    APawn* OwningPawn = TryGetPawnOwner();
    if (OwningPawn)
    {
        Speed = OwningPawn->GetVelocity().Size();

        // Simple heuristic: if vertical speed is significant or pawn is not on ground -> consider flying
        const float VerticalSpeed = FMath::Abs(OwningPawn->GetVelocity().Z);
        bIsFlying = VerticalSpeed > 10.0f || !OwningPawn->GetMovementComponent() || !OwningPawn->GetMovementComponent()->IsMovingOnGround();
    }
}
