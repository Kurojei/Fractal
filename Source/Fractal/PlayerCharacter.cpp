#include "PlayerCharacter.h"
#include "GameFramework/PlayerInput.h"
#include "GameFramework/CharacterMovementComponent.h"
#include <Kismet/KismetMathLibrary.h>
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

APlayerCharacter::APlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	springArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	springArm->SetupAttachment(GetMesh(), FName("CameraSocket"));
	springArm->bUsePawnControlRotation = true;
	springArm->TargetArmLength = 0;

	cam = CreateDefaultSubobject<UCameraComponent>(TEXT("Cam"));
	cam->SetupAttachment(springArm, FName("SpringEndpoint"));
	cam->bUsePawnControlRotation = true;

	GetMesh()->SetRelativeLocation(FVector(0.f, 0.f, 50.f));
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
	GEngine->AddOnScreenDebugMessage(-1, 3.f, FColor::Green, FString::Printf(TEXT("Move Vector: X=%f, Y=%f"), moveVector.X, moveVector.Y));
}

void APlayerCharacter::Look(const FInputActionValue& value)
{
	FVector2D lookVector = value.Get<FVector2D>();
	AddControllerYawInput(lookVector.X);
	AddControllerPitchInput(-lookVector.Y);
}