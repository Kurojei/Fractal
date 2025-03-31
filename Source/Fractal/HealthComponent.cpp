#include "HealthComponent.h"
#include "PlayerCharacter.h"

UHealthComponent::UHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}


void UHealthComponent::BeginPlay()
{
	Super::BeginPlay();

}

void UHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UHealthComponent::ApplyDamage(ACharacter* instigator, float amount)
{
	health -= amount;
	if (health <= 0)
	{
		onDeath.Broadcast();
	}
}

void UHealthComponent::SetHealth(float amount)
{
	health = amount;
	//GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, FString::Printf(TEXT("Health is now %f."), health));
}
