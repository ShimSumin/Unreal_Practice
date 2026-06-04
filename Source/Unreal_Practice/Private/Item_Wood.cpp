// Fill out your copyright notice in the Description page of Project Settings.


#include "Item_Wood.h"

#include "Kismet/GameplayStatics.h" // 스폰 함수 사용을 위해 필수
#include "Particles/ParticleSystem.h" //파티클 시스템 헤더

void AItem_Wood::OnFireDetected_Implementation(float Temperature, FVector HitLocation)
{
    if (FireEffect)
    {

        UGameplayStatics::SpawnEmitterAtLocation(
            GetWorld(),
            FireEffect,
            GetActorLocation(),
            GetActorRotation(),
            FVector(1.0f) 
        );
    }
}