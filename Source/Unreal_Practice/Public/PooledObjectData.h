#pragma once

#include "CoreMinimal.h"
#include "PooledObjectData.generated.h"

USTRUCT(BlueprintType)
struct FPooledObjectData
{
	GENERATED_BODY()

	FPooledObjectData()
	{
		ActorTemplate = nullptr;
		PoolSize = 1;
		CanGrow = false;
		ActorName = "default";
	}


	UPROPERTY(EditAnywhere)
	TSubclassOf<AActor> ActorTemplate;

	UPROPERTY(EditAnywhere)
	int PoolSize;

	UPROPERTY(EditAnywhere)
	bool CanGrow;

	UPROPERTY(EditAnywhere)
	FString ActorName;

};