#pragma once

#include "CoreMinimal.h"
#include "BaseMountActor.h"
#include "DragonMountActor.generated.h"

class USkeletalMeshComponent;

UCLASS()
class DRAGONREALMS_API ADragonMountActor : public ABaseMountActor
{
    GENERATED_BODY()

public:
    ADragonMountActor();

    virtual void BeginPlay() override;

    // Skeletal mesh for the dragon (set in Blueprint)
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    USkeletalMeshComponent* SkeletalMeshComp;

    // Socket name where the rider will be attached
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mount")
    FName RiderSocketName;
};
