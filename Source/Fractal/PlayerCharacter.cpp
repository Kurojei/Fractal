#include "PlayerCharacter.h"
#include "GameFramework/PlayerInput.h"
#include "GameFramework/CharacterMovementComponent.h"
#include <Kismet/KismetMathLibrary.h>
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "HealthComponent.h"

APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	springArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	springArm->SetupAttachment(GetMesh(), FName("cameraSocket"));
	springArm->TargetArmLength = 0;

	cam = CreateDefaultSubobject<UCameraComponent>(TEXT("Cam"));
	cam->SetupAttachment(springArm, FName("SpringEndpoint"));

	gun = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Gun"));
	gun->SetupAttachment(GetMesh(), FName("gunSocket"));

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
}

void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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
	}
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
	AddControllerYawInput(lookVector.X);

	FRotator currentRotation = GetMesh()->GetRelativeRotation();
	float newPitch = currentRotation.Pitch + (lookVector.Y * lookSensitivity);
	newPitch = FMath::Clamp(newPitch, -70.f, 70.f);
	GetMesh()->SetRelativeRotation(FRotator(newPitch, currentRotation.Yaw, currentRotation.Roll));
}

void APlayerCharacter::OnDeath()
{
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Red, TEXT("Dead"));
}