#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputMappingContext.h"
#include "BaseWeapon.h"
#include "PlayerCharacter.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAim, bool, isAiming);

UCLASS()
class FRACTAL_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	APlayerCharacter();
	virtual void BeginPlay() override;
	virtual void Tick(float deltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	void StartAiming();
	void StopAiming();
	void Fire();
	void StopFire();
	void Reload();
	void SpawnAndAttachWeapon(TSubclassOf<ABaseWeapon> weaponToSpawn);

	void Move(const FInputActionValue& value);
	void Look(const FInputActionValue& value);

	class UHealthComponent* GetHealthComponent() { return healthComponent; };
	class UCameraComponent* GetCameraComponent() { return cam; };

	UFUNCTION(BlueprintCallable)
	bool GetIsAiming() { return isAiming; };

	UFUNCTION(BlueprintCallable)
	class ABaseWeapon* GetCurrentWeapon() { return currentWeapon; };

	UFUNCTION()
	void OnDeath();

public:
	UPROPERTY(BlueprintAssignable)
	FOnAim onAim;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TSubclassOf<UUserWidget> playerHUDReference = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<ABaseWeapon> startingWeapon = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USpringArmComponent* springArm = nullptr;

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
	UInputAction* fireAction = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UInputAction* reloadAction = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float lookSensitivity = 1;

private:
	bool isAiming = false;
	UUserWidget* playerHUD = nullptr;
	TArray<ABaseWeapon*> weapons;
	ABaseWeapon* currentWeapon = nullptr;
	UCameraComponent* cam = nullptr;
	class UHealthComponent* healthComponent = nullptr;
};