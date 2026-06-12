#include "PoolManagerComponent.h"

void UPoolManagerComponent::BeginPlay()
{
    Super::BeginPlay();

    Pool = GetWorld()->SpawnActor<AMyObjectPool>();
    if (Pool)
    {
        Pool->PooledObjectData = PooledObjectData;
        UE_LOG(LogTemp, Warning, TEXT("Pool 생성 성공, 데이터 개수: %d"), PooledObjectData.Num());
        Pool->InitPool();
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("Pool 생성 실패"));
    }
}

AActor* UPoolManagerComponent::GetPooledActor(FString Name)
{
    if (!Pool)
    {
        UE_LOG(LogTemp, Error, TEXT("Pool이 없음"));
        return nullptr;
    }
    AActor* Result = Pool->GetPooledActor(Name);
    if (Result)
    {
        UE_LOG(LogTemp, Warning, TEXT("액터 꺼내기 성공: %s"), *Result->GetName());
    }
    else
    {
        UE_LOG(LogTemp, Error, TEXT("액터 꺼내기 실패, 요청한 이름: %s"), *Name);
    }
    return Result;
}

void UPoolManagerComponent::RecyclePooledObject(UPooledObject* PoolCompRef)
{
    if (!Pool) { return; }
    Pool->RecyclePooledObject(PoolCompRef);
}

void UPoolManagerComponent::RecycleActor(AActor* PooledActor)
{
    if (!Pool) { return; }
    Pool->RecycleActor(PooledActor);
}

void UPoolManagerComponent::Broadcast_PoolerCleanup()
{
    if (!Pool) { return; }
    Pool->Broadcast_PoolerCleanup();
}