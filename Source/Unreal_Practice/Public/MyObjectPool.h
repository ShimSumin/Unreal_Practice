#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PooledObjectData.h"
#include "MyObjectPool.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPoolerCleanupSignature);

USTRUCT(BlueprintType)
struct FSingleObjectPool
{
    GENERATED_BODY()

    UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly)
    TArray<TObjectPtr<class UPooledObject>> PooledObjects;
};

UCLASS()
class UNREAL_PRACTICE_API AMyObjectPool : public AActor
{
    GENERATED_BODY()

protected:
    virtual void BeginPlay() override;

public:
    UPROPERTY()
    FPoolerCleanupSignature OnPoolerCleanup;

    UFUNCTION(BlueprintCallable)
    void Broadcast_PoolerCleanup();

    UFUNCTION(BlueprintCallable)
    AActor* GetPooledActor(FString Name);

    UFUNCTION(BlueprintCallable)
    void RecyclePooledObject(class UPooledObject* PoolCompRef);

    UFUNCTION(BlueprintCallable)
    void RecycleActor(AActor* PooledActor);

    UFUNCTION()
    void InitPool();

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    TArray<FPooledObjectData> PooledObjectData;

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
    TArray<FSingleObjectPool> Pools;

private:
    void RegenItem(int32 PoolIndex, int32 PositionIndex);
};