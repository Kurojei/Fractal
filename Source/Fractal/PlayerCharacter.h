#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputMappingContext.h"
#include "BaseWeapon.h"
#include "PlayerCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnAiming);

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
	class UHealthComponent* GetHealthComponent() { return healthComponent; };

	UFUNCTION()
	void OnDeath();

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USkeletalMeshComponent* gun;

	UPROPERTY(BlueprintAssignable)
	FOnAiming onAiming;

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

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int lookSensitivity = 2;

private:
	TArray<ABaseWeapon> guns;
	class UHealthComponent* healthComponent;
};