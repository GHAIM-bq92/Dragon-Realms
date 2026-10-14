#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseMountActor.generated.h"

class UStaticMeshComponent;
class UMountComponent;

UCLASS()
class DRAGONREALMS_API ABaseMountActor : public AActor
{
    GENERATED_BODY()

public:
    ABaseMountActor();

    virtual void BeginPlay() override;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UStaticMeshComponent* MeshComp;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    UMountComponent* MountComp;

    // Called by a player pawn to request mounting this actor
    UFUNCTION(BlueprintCallable, Category = "Mount")
    void RequestMount(AActor* RequestingPawn);

    UFUNCTION(BlueprintCallable, Category = "Mount")
    void RequestDismount(AActor* RequestingPawn);
};
