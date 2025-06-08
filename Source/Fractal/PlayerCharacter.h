#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputMappingContext.h"
#include "BaseWeapon.h"
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

	void StartAiming();
	void StopAiming();

	void Move(const FInputActionValue& value);
	void Look(const FInputActionValue& value);

	class UHealthComponent* GetHealthComponent() { return healthComponent; };

	UFUNCTION(BlueprintCallable)
	bool GetIsAiming() { return isAiming; };

	UFUNCTION()
	void OnDeath();

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USkeletalMeshComponent* gun;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UCameraComponent* cam;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USpringArmComponent* springArm;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UInputMappingContext* mappingContext = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UInputAction* jumpAction = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UInputAction* moveAction = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UInputAction* lookAction = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UInputAction* aimAction = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float lookSensitivity = 1;

private:
	bool isAiming = false;
	TArray<ABaseWeapon> guns;
	class UHealthComponent* healthComponent;
};