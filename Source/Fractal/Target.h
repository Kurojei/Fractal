#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Target.generated.h"

UCLASS()
class FRACTAL_API ATarget : public AActor
{
	GENERATED_BODY()
	
public:	
	ATarget();
	void Hit();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	USkeletalMeshComponent* target;
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UAnimSequence* targetDown;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UAnimSequence* targetUp;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)  
	float minDelay = 5.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float maxDelay = 15.f;
};