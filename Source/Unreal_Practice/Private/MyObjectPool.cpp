#include "MyObjectPool.h"
#include "PooledObject.h"

void AMyObjectPool::BeginPlay()
{
    Super::BeginPlay();
    InitPool();
}

void AMyObjectPool::InitPool()
{
    Pools.Empty();
    FActorSpawnParameters SpawnParams;
    for (int32 PoolIndex = 0; PoolIndex < PooledObjectData.Num(); PoolIndex++)
    {
        FSingleObjectPool CurrentPoolIndex;
        SpawnParams.Name = FName(FString::Printf(TEXT("%s"), *PooledObjectData[PoolIndex].ActorName));
        SpawnParams.NameMode = FActorSpawnParameters::ESpawnActorNameMode::Requested;
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
        for (int32 ObjectIndex = 0; ObjectIndex < PooledObjectData[PoolIndex].PoolSize; ObjectIndex++)
        {
            AActor* SpawnedActor = GetWorld()->SpawnActor(PooledObjectData[PoolIndex].ActorTemplate, &FVector::ZeroVector, &FRotator::ZeroRotator, SpawnParams);
            SpawnedActor->SetActorLabel(SpawnedActor->GetName());
            UPooledObject* PoolComp = NewObject<UPooledObject>(SpawnedActor);
            PoolComp->RegisterComponent();
            SpawnedActor->AddInstanceComponent(PoolComp);
            PoolComp->Init(this);
            CurrentPoolIndex.PooledObjects.Add(PoolComp);
            SpawnedActor->SetActorHiddenInGame(true);
            SpawnedActor->SetActorEnableCollision(false);
            SpawnedActor->SetActorTickEnabled(false);
            SpawnedActor->AttachToActor(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
        }
        Pools.Add(CurrentPoolIndex);
    }
}

void AMyObjectPool::Broadcast_PoolerCleanup()
{
    OnPoolerCleanup.Broadcast();
}

AActor* AMyObjectPool::GetPooledActor(FString Name)
{
    int32 PoolCount = Pools.Num();
    int32 CurrentPoolIndex = -1;

    UE_LOG(LogTemp, Warning, TEXT("풀 개수: %d"), PoolCount);
    for (int32 i = 0; i < PoolCount; i++)
    {
        UE_LOG(LogTemp, Warning, TEXT("풀 이름 비교: [%s] vs [%s]"), *PooledObjectData[i].ActorName, *Name);
        if (PooledObjectData[i].ActorName == Name)
        {
            CurrentPoolIndex = i;
            break;
        }
    }

    if (CurrentPoolIndex == -1) { return nullptr; }

    int32 PooledObjectCount = Pools[CurrentPoolIndex].PooledObjects.Num();
    int32 FirstAvailable = -1;

    for (int32 i = 0; i < PooledObjectCount; i++)
    {
        if (Pools[CurrentPoolIndex].PooledObjects[i] != nullptr)
        {
            if (!Pools[CurrentPoolIndex].PooledObjects[i]->bIsPoolActive)
            {
                FirstAvailable = i;
                break;
            }
        }
        else
        {
            RegenItem(CurrentPoolIndex, i);
            FirstAvailable = i;
            break;
        }
    }

    if (FirstAvailable >= 0)
    {
        UPooledObject* ToReturn = Pools[CurrentPoolIndex].PooledObjects[FirstAvailable];
        ToReturn->bIsPoolActive = true;
        OnPoolerCleanup.AddUniqueDynamic(ToReturn, &UPooledObject::RecycleSelf);

        AActor* ToReturnActor = ToReturn->GetOwner();
        ToReturnActor->SetActorHiddenInGame(false);
        ToReturnActor->SetActorEnableCollision(true);
        ToReturnActor->SetActorTickEnabled(true);
        ToReturnActor->AttachToActor(nullptr, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

        return ToReturnActor;
    }

    if (!PooledObjectData[CurrentPoolIndex].CanGrow) { return nullptr; }

    FActorSpawnParameters SpawnParams;
    SpawnParams.Name = FName(FString::Printf(TEXT("%s"), *PooledObjectData[CurrentPoolIndex].ActorName));
    SpawnParams.NameMode = FActorSpawnParameters::ESpawnActorNameMode::Requested;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    AActor* SpawnedActor = GetWorld()->SpawnActor(PooledObjectData[CurrentPoolIndex].ActorTemplate, &FVector::ZeroVector, &FRotator::ZeroRotator, SpawnParams);
    SpawnedActor->SetActorLabel(SpawnedActor->GetName());
    UPooledObject* PoolComp = NewObject<UPooledObject>(SpawnedActor);
    PoolComp->RegisterComponent();
    SpawnedActor->AddInstanceComponent(PoolComp);
    PoolComp->Init(this);
    Pools[CurrentPoolIndex].PooledObjects.Add(PoolComp);
    PoolComp->bIsPoolActive = true;
    OnPoolerCleanup.AddUniqueDynamic(PoolComp, &UPooledObject::RecycleSelf);
    return SpawnedActor;
}

void AMyObjectPool::RecyclePooledObject(UPooledObject* PoolCompRef)
{
    OnPoolerCleanup.RemoveDynamic(PoolCompRef, &UPooledObject::RecycleSelf);
    PoolCompRef->bIsPoolActive = false;
    AActor* ReturningActor = PoolCompRef->GetOwner();
    ReturningActor->SetActorHiddenInGame(true);
    ReturningActor->SetActorEnableCollision(false);
    ReturningActor->SetActorTickEnabled(false);
    ReturningActor->AttachToActor(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
}

void AMyObjectPool::RecycleActor(AActor* PooledActor)
{
    if (UPooledObject* PoolCompRef = Cast<UPooledObject>(PooledActor->GetComponentByClass(UPooledObject::StaticClass())))
    {
        RecyclePooledObject(PoolCompRef);
    }
}

void AMyObjectPool::RegenItem(int32 PoolIndex, int32 PositionIndex)
{
    FActorSpawnParameters SpawnParams;
    SpawnParams.Name = FName(FString::Printf(TEXT("%s"), *PooledObjectData[PoolIndex].ActorName));
    SpawnParams.NameMode = FActorSpawnParameters::ESpawnActorNameMode::Requested;
    SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

    AActor* SpawnedActor = GetWorld()->SpawnActor(PooledObjectData[PoolIndex].ActorTemplate, &FVector::ZeroVector, &FRotator::ZeroRotator, SpawnParams);
    SpawnedActor->SetActorLabel(SpawnedActor->GetName());
    UPooledObject* PoolComp = NewObject<UPooledObject>(SpawnedActor);
    PoolComp->RegisterComponent();
    SpawnedActor->AddInstanceComponent(PoolComp);
    PoolComp->Init(this);
    Pools[PoolIndex].PooledObjects.Insert(PoolComp, PositionIndex);
    SpawnedActor->SetActorHiddenInGame(true);
    SpawnedActor->SetActorEnableCollision(false);
    SpawnedActor->SetActorTickEnabled(false);
    SpawnedActor->AttachToActor(this, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
}