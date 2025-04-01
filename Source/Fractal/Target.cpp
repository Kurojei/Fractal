#include "Target.h"

ATarget::ATarget()
{
	PrimaryActorTick.bCanEverTick = true;

	target = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Target"));
	target->SetupAttachment(RootComponent);
}

void ATarget::Hit()
{
	target->PlayAnimation(targetDown, false);

	float delay = FMath::FRandRange(minDelay, maxDelay);

	auto playTargetUp = [&]() 
	{
		target->PlayAnimation(targetUp, false);
	};

	FTimerHandle targetHandle;
	GetWorld()->GetTimerManager().SetTimer(targetHandle, playTargetUp, delay, false);
}