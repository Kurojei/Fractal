#include "BaseWeapon.h"
#include "HealthComponent.h"
#include "PlayerCharacter.h"
#include <DrawDebugHelpers.h>
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Target.h"

ABaseWeapon::ABaseWeapon() 
{
	PrimaryActorTick.bCanEverTick = true;

	gunMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Gun"));
	gunMesh->SetupAttachment(RootComponent);

	audioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	audioComponent->SetupAttachment(RootComponent);
}

void ABaseWeapon::TryFire()
{
	float currentTime = GetWorld()->GetTimeSeconds();
	if (isFiring && currentTime - lastFireTime >= fireRate) {
		lastFireTime = currentTime;
		Fire();
	}
}

void ABaseWeapon::Fire() 
{
	if (isReloading) return;
	if (currentMagAmmo == 0) {
		Reload(); 
		return;
	}

	currentMagAmmo--;
	onAmmoChanged.Broadcast(currentMagAmmo, currentStockAmmo);

	APlayerCharacter* owner = GetOwner<APlayerCharacter>();
	owner->GetMesh()->GetAnimInstance()->Montage_Play(owner->GetIsAiming() ? armAimFire : armFire);
	gunMesh->GetAnimInstance()->Montage_Play(gunFire);

	const FVector start = GetOwner<APlayerCharacter>()->GetCameraComponent()->GetComponentLocation();
	const FVector end = GetOwner<APlayerCharacter>()->GetCameraComponent()->GetForwardVector() * gunRange + start;
	FCollisionQueryParams params;
	params.AddIgnoredActor(GetOwner<APlayerCharacter>());
	FHitResult outHit;
	
	if (GetWorld()->LineTraceSingleByChannel(outHit, start, end, ECC_Visibility, params)) {
		if (ATarget* target = Cast<ATarget>(outHit.GetActor())) {
			target->Hit();
			audioComponent->SetSound(hitmarker);
			audioComponent->Play();
		}
	}

	float decalSize = FMath::FRandRange(1.5f, 7.f);
	//UGameplayStatics::SpawnDecalAtLocation(GetWorld(), bulletDecal, FVector(decalSize, decalSize, decalSize), outHit.Location, outHit.ImpactNormal.Rotation() * -1, 100.f);

	if (fullAuto) {
		GetWorld()->GetTimerManager().SetTimer(timerHandle, this, &ABaseWeapon::TryFire, fireRate, true);
	} 
}

void ABaseWeapon::Reload() 
{
	if (!gunReload || !armReload || isReloading) {
		return;
	}

	if (currentStockAmmo > 0 && currentMagAmmo < maxMagAmmo) {
		auto refreshAmmoUI = [&]() {
			int amountToAdd = maxMagAmmo - currentMagAmmo;
			if (currentStockAmmo >= amountToAdd) {
				currentMagAmmo = maxMagAmmo;
				currentStockAmmo -= amountToAdd;
			}
			else {
				currentMagAmmo += currentStockAmmo;
				currentStockAmmo = 0;
			}

			isReloading = false;
			onAmmoChanged.Broadcast(currentMagAmmo, currentStockAmmo);
			GetWorld()->GetTimerManager().ClearTimer(timerHandle);
		};

		isReloading = true;
		GetOwner<APlayerCharacter>()->GetMesh()->GetAnimInstance()->Montage_Play(armReload);
		gunMesh->GetAnimInstance()->Montage_Play(gunReload);
		GetWorld()->GetTimerManager().SetTimer(timerHandle, refreshAmmoUI, armReload->GetPlayLength(), false);
	}
}