// Fill out your copyright notice in the Description page of Project Settings.


#include "MyTorchlight.h"
#include "TestMyInterface.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
AMyTorchlight::AMyTorchlight()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AMyTorchlight::BeginPlay()
{
	Super::BeginPlay();

	for (const TWeakObjectPtr<AActor>& Item : Items)
	{
		if (UKismetSystemLibrary::DoesImplementInterface(Item.Get(), UTestMyInterface::StaticClass()))
		{
			ITestMyInterface::Execute_OnFireDetected(Item.Get(), 100.0f, FVector::ZeroVector);
		}


	}

}

// Called every frame
void AMyTorchlight::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

