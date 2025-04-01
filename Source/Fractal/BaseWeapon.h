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
	void Reload();

public:
	UPROPERTY(BlueprintAssignable)
	FOnAmmoChanged onAmmoChanged;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USkeletalMeshComponent* armMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	class UNiagaraComponent* muzzleFlash;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USkeletalMeshComponent* gunMesh;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UMaterialInterface* bulletDecal;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UTexture2D* gunSilouhette;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	FName gunName = "";

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UAnimMontage* gunFire;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UAnimMontage* armFire;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UAnimMontage* gunReload;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UAnimMontage* armReload;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	UAnimMontage* equip;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	USoundCue* hitmarker;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	bool fullAuto = false;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float fireRate = 0.2;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float maxStockAmmo;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float maxMagAmmo;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float currentStockAmmo = 32;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float currentMagAmmo = 8;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float gunRange = 20000.f;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float gunDamage = 20.f;

private:
	class APlayerCharacter* owner;
	bool bIsReloading = false;
	bool bIsFiring = false;
	float hitAmount = 10.f;
	FTimerHandle fullAutoHandle;
	FTimerHandle reloadHandle;
	UAudioComponent* audioComponent;
};