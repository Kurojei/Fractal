#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputMappingContext.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class FRACTAL_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	APlayerCharacter();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	void Move(const FInputActionValue& value);
	void Look(const FInputActionValue& value);

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UCameraComponent* cam;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USpringArmComponent* springArm;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UInputMappingContext* mappingContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UInputAction* jumpAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UInputAction* moveAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UInputAction* lookAction;
};