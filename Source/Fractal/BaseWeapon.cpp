#include "BaseWeapon.h"
#include "HealthComponent.h"
#include "PlayerCharacter.h"
#include <DrawDebugHelpers.h>
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"

ABaseWeapon::ABaseWeapon() {
	PrimaryActorTick.bCanEverTick = true;
	owner = Cast<APlayerCharacter>(GetOwner());
	gunMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Gun"));
	gunMesh->SetupAttachment(RootComponent);
	audioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	audioComponent->SetupAttachment(RootComponent);
	muzzleFlash = CreateDefaultSubobject<UNiagaraComponent>(TEXT("MuzzleFlash"));
	muzzleFlash->SetupAttachment(gunMesh);
}

void ABaseWeapon::Fire() {
	if (bIsReloading) return;
	if (currentMagAmmo == 0) { Reload(); return; }
	if (!bIsFiring) { bIsFiring = true; }

	currentMagAmmo--;
	onAmmoChanged.Broadcast(currentMagAmmo, currentStockAmmo);
	owner->GetMesh()->GetAnimInstance()->Montage_Play(armFire);
	gunMesh->GetAnimInstance()->Montage_Play(armFire);
	muzzleFlash->Activate(true);

	const FVector start = owner->cam->GetSocketLocation("");
	const FVector end = owner->cam->GetForwardVector() * gunRange + start;
	FCollisionQueryParams params;
	params.AddIgnoredActor(owner);
	FHitResult outHit;
	if (GetWorld()->LineTraceSingleByChannel(outHit, start, end, ECC_Visibility, params)) {
		
	}

	float decalSize = FMath::FRandRange(1.5f, 7.f);
	UGameplayStatics::SpawnDecalAtLocation(GetWorld(), bulletDecal, FVector(decalSize, decalSize, decalSize), outHit.Location, outHit.ImpactNormal.Rotation() * -1, 100.f);

	if (fullAuto && bIsFiring) {
		GetWorld()->GetTimerManager().SetTimer(fullAutoHandle, this, &ABaseWeapon::Fire, fireRate, false);
	}
	else {
		bIsFiring = false;
		muzzleFlash->Deactivate();
		owner->GetMesh()->GetAnimInstance()->StopAllMontages(0.1);
		GetWorld()->GetTimerManager().ClearTimer(fullAutoHandle);
	}
}

void ABaseWeapon::Reload() {
	if (currentStockAmmo > 0 && currentMagAmmo < maxMagAmmo && !bIsFiring) {
		bIsReloading = true;

		auto refreshAmmoUI = [&]() {
			int amountToAdd = maxMagAmmo - currentMagAmmo;
			if (currentStockAmmo >= amountToAdd) {
				currentStockAmmo -= amountToAdd;
				currentMagAmmo = maxMagAmmo;
				onAmmoChanged.Broadcast(currentMagAmmo, currentStockAmmo);
			}
			else {
				currentMagAmmo += currentStockAmmo;
				currentStockAmmo = 0;
			}
			bIsReloading = false;
			GetWorld()->GetTimerManager().ClearTimer(reloadHandle);
		};

		owner->GetMesh()->GetAnimInstance()->Montage_Play(armReload);
		gunMesh->GetAnimInstance()->Montage_Play(gunReload);
		GetWorld()->GetTimerManager().SetTimer(reloadHandle, refreshAmmoUI, armReload->GetPlayLength(), false);
	}
}