#pragma once
#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MyObjectPool.h"
#include "PooledObjectData.h"
#include "PoolManagerComponent.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UNREAL_PRACTICE_API UPoolManagerComponent : public UActorComponent
{
    GENERATED_BODY()

public:
    virtual void BeginPlay() override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FPooledObjectData> PooledObjectData;

    UFUNCTION(BlueprintCallable)
    AActor* GetPooledActor(FString Name);

    UFUNCTION(BlueprintCallable)
    void RecyclePooledObject(class UPooledObject* PoolCompRef);

    UFUNCTION(BlueprintCallable)
    void RecycleActor(AActor* PooledActor);

    UFUNCTION(BlueprintCallable)
    void Broadcast_PoolerCleanup();

private:
    UPROPERTY()
    TObjectPtr<AMyObjectPool> Pool;
};
