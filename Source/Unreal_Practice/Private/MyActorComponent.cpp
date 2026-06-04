#include "MyActorComponent.h"

UMyActorComponent::UMyActorComponent()
{
    PrimaryComponentTick.bCanEverTick = true;
    _MaxHealth = 100.f;
}

void UMyActorComponent::BeginPlay()
{
    Super::BeginPlay();
    _CurrentHealth = _MaxHealth;
    GetOwner()->OnTakeAnyDamage.AddDynamic(this, &UMyActorComponent::DamageTake);
}

void UMyActorComponent::DamageTake(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* Instigator, AActor* Causer)
{
    float FinalDamage = FMath::Min(Damage, _CurrentHealth);
    _CurrentHealth -= FinalDamage;

    OnHealthDamaged.Broadcast(_CurrentHealth, _MaxHealth, FinalDamage);

    if (_CurrentHealth == 0.f)
    {
        OnHealthDead.Broadcast(Instigator);
    }
}

void UMyActorComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
    Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
    GEngine->AddOnScreenDebugMessage(-1, 0.f, FColor::Green, FString::Printf(TEXT("HP : %f"), _CurrentHealth));
}