#include "PlayerCharacter.h"
#include "GameFramework/PlayerInput.h"
#include "GameFramework/CharacterMovementComponent.h"
#include <Kismet/KismetMathLibrary.h>
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "HealthComponent.h"
#include "Blueprint/UserWidget.h"

APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	springArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	springArm->SetupAttachment(GetMesh(), FName("cameraSocket"));
	springArm->TargetArmLength = 0;

	cam = CreateDefaultSubobject<UCameraComponent>(TEXT("Cam"));
	cam->SetupAttachment(springArm, FName("SpringEndpoint"));
	cam->SetFieldOfView(95.f);

	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, 60.f));
	GetMesh()->SetupAttachment(RootComponent);

	healthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthComponent"));
	healthComponent->onDeath.AddDynamic(this, &APlayerCharacter::OnDeath);
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	UEnhancedInputLocalPlayerSubsystem* InputSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
	InputSubsystem->AddMappingContext(mappingContext, 0);

	SpawnAndAttachWeapon(startingWeapon);

	if (playerHUDReference) {
		playerHUD = CreateWidget(GetWorld(), playerHUDReference);
		playerHUD->AddToViewport();
	}

	currentWeapon->onAmmoChanged.Broadcast(currentWeapon->maxMagAmmo, currentWeapon->maxStockAmmo);
}

void APlayerCharacter::SpawnAndAttachWeapon(TSubclassOf<ABaseWeapon> weaponToSpawn) 
{
	if (weaponToSpawn) 
	{
		if (ABaseWeapon* spawnedWeapon = GetWorld()->SpawnActor<ABaseWeapon>(weaponToSpawn))
		{
			spawnedWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, FName("gunSocket"));
			spawnedWeapon->SetActorEnableCollision(false);
			spawnedWeapon->SetOwner(this);
			weapons.Add(spawnedWeapon);
			currentWeapon = spawnedWeapon;
		}
	}
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* inputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		inputComponent->BindAction(jumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		inputComponent->BindAction(jumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
		inputComponent->BindAction(moveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);
		inputComponent->BindAction(lookAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Look);
		inputComponent->BindAction(aimAction, ETriggerEvent::Started, this, &APlayerCharacter::StartAiming);
		inputComponent->BindAction(aimAction, ETriggerEvent::Completed, this, &APlayerCharacter::StopAiming);
		inputComponent->BindAction(fireAction, ETriggerEvent::Started, this, &APlayerCharacter::Fire);
		inputComponent->BindAction(fireAction, ETriggerEvent::Completed, this, &APlayerCharacter::StopFire);
		inputComponent->BindAction(reloadAction, ETriggerEvent::Started, this, &APlayerCharacter::Reload);
		//inputComponent->BindAction(reloadAction, ETriggerEvent::Started, this, );
	}
}

void APlayerCharacter::Fire()
{
	if (currentWeapon) {
		currentWeapon->SetIsFiring(true);
		currentWeapon->TryFire();
	}
}

void APlayerCharacter::StopFire()
{
	if (currentWeapon) {
		currentWeapon->SetIsFiring(false);
	}
}

void APlayerCharacter::Reload()
{
	if (currentWeapon) {
		currentWeapon->Reload();
	}
}

void APlayerCharacter::StartAiming() 
{
	isAiming = true;
	GetCharacterMovement()->MaxWalkSpeed = 375.f;

}

void APlayerCharacter::StopAiming()
{
	isAiming = false;
	GetCharacterMovement()->MaxWalkSpeed = 475.f;

}

void APlayerCharacter::Move(const FInputActionValue& value)
{
	FVector2D moveVector = value.Get<FVector2D>();
	AddMovementInput(GetActorForwardVector(), moveVector.Y);
	AddMovementInput(GetActorRightVector(), moveVector.X);
}

void APlayerCharacter::Look(const FInputActionValue& value)
{
	FVector2D lookVector = value.Get<FVector2D>();
	AddControllerYawInput(lookVector.X * lookSensitivity);

	FRotator currentRotation = GetMesh()->GetRelativeRotation();
	float newPitch = currentRotation.Pitch + (lookVector.Y * lookSensitivity);
	newPitch = FMath::Clamp(newPitch, -70.f, 70.f);
	GetMesh()->SetRelativeRotation(FRotator(newPitch, currentRotation.Yaw, currentRotation.Roll));
}

void APlayerCharacter::OnDeath()
{
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("Dead"));
}