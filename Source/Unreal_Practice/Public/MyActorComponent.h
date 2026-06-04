// component.h

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MyActorComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FHealthDeadSignature, AController*, Instigator);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FHealthDamagedSignature, float, NewHealth, float, MaxHealth, float, HealthChange);


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UNREAL_PRACTICE_API UMyActorComponent : public UActorComponent
{
	GENERATED_BODY()

public:

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UMyActorComponent();

	UPROPERTY(BlueprintAssignable)
	FHealthDeadSignature OnHealthDead;

	UPROPERTY(BlueprintAssignable)
	FHealthDamagedSignature OnHealthDamaged;

	UFUNCTION(BlueprintCallable)
	float GetCurrentHealth() const { return _CurrentHealth; }

	UFUNCTION(BlueprintCallable)
	float GetMaxHealth() const { return _MaxHealth; }

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float _MaxHealth;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	float _CurrentHealth;


	virtual void BeginPlay() override;

private:

	UFUNCTION()
	void DamageTake(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* Instigator, AActor* Causer);

};
