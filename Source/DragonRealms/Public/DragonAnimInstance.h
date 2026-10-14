#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "DragonAnimInstance.generated.h"

UCLASS(Transient, Blueprintable)
class DRAGONREALMS_API UDragonAnimInstance : public UAnimInstance
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintReadOnly, Category = "Animation")
    float Speed;

    UPROPERTY(BlueprintReadOnly, Category = "Animation")
    bool bIsFlying;

    virtual void NativeUpdateAnimation(float DeltaSeconds) override;
};
