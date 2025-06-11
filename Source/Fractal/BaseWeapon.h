#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SkeletalMeshComponent.h"
#include <Animation/BlendSpace1D.h>
#include <Sound/SoundCue.h>
#include <Components/AudioComponent.h>
#include "BaseWeapon.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnAmmoChanged, float, currentMagAmmo, float, currentStockAmmo);

UCLASS()
class FRACTAL_API ABaseWeapon : public AActor
{
	GENERATED_BODY()

public:
	ABaseWeapon();
	void Fire();
	void StopFire();
	void Reload();

public:
	UPROPERTY(BlueprintAssignable)
	FOnAmmoChanged onAmmoChanged;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UNiagaraComponent* muzzleFlash = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USkeletalMeshComponent* gunMesh = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UMaterialInterface* bulletDecal = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UTexture2D* gunSilouhette = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FName gunName = "";

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UAnimMontage* gunFire = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UAnimMontage* armFire = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UAnimMontage* armAimFire = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UAnimMontage* gunReload = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UAnimMontage* armReload = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UAnimMontage* equip = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USoundCue* hitmarker = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	bool fullAuto = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float fireRate = 0.2;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float maxStockAmmo = 32;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float maxMagAmmo = 8;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float currentStockAmmo = 32;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float currentMagAmmo = 8;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float gunRange = 20000.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float gunDamage = 20.f;

private:
	bool isReloading = false;
	bool isFiring = false;
	FTimerHandle timerHandle;
	UAudioComponent* audioComponent = nullptr;
};