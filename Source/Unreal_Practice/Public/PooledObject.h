#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PooledObject.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNREAL_PRACTICE_API UPooledObject : public UActorComponent
{
	GENERATED_BODY()

public:

	void Init(class AMyObjectPool* Owner);

	UFUNCTION(BlueprintCallable)
	void RecycleSelf();

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly)
	bool bIsPoolActive;

private:

	TObjectPtr<class AMyObjectPool> ObjectPool;

	virtual void OnComponentDestroyed(bool bDestroyingHierarchy) override;
};
