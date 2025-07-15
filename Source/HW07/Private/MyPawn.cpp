#include "MyPawn.h"
#include "EnhancedInputComponent.h"
#include "MyPlayerController.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"


AMyPawn::AMyPawn()
{
	CapsuleComp = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComp"));
	SetRootComponent(CapsuleComp);
	CapsuleComp->InitCapsuleSize(50.f, 150.f);

	SkeletalMeshComp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	SkeletalMeshComp->SetupAttachment(CapsuleComp);
	
	CapsuleComp->SetSimulatePhysics(false);
	SkeletalMeshComp->SetSimulatePhysics(false);


	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SpringArmComp->SetupAttachment(CapsuleComp);
	SpringArmComp->TargetArmLength = 300.0f;
	

	
	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	CameraComp->SetupAttachment(SpringArmComp, USpringArmComponent::SocketName);
	CameraComp->bUsePawnControlRotation = false;

	PrimaryActorTick.bCanEverTick = true;
	MoveSpeed = 500.0f;
}

void AMyPawn::Tick(float DeltaTime) 
{
	Super::Tick(DeltaTime);
	

}

void AMyPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (UEnhancedInputComponent* EnhancedInput = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		if (AMyPlayerController* PlayerController = Cast<AMyPlayerController>(GetController()))
		{
			if (PlayerController->MoveAction)
			{
				EnhancedInput->BindAction(
					PlayerController->MoveAction,
					ETriggerEvent::Triggered,
					this,
					&AMyPawn::Move
				);
			}
			if (PlayerController->LookAction)
			{
				EnhancedInput->BindAction(
					PlayerController->LookAction,
					ETriggerEvent::Triggered,
					this,
					&AMyPawn::Look
				);
			}
		}
	}

}


void AMyPawn:: Move(const FInputActionValue& value)
{
	if (!Controller) return;

	const FVector2D MoveVector = value.Get<FVector2D>();
	
	FVector Forward = GetActorForwardVector();
	FVector Right = GetActorRightVector();

	FVector Move = (Forward * MoveVector.X) + (Right * MoveVector.Y);
	Move.Z = 0.f;
	Move.Normalize();

	float DeltaTime = GetWorld()->GetDeltaSeconds();
	FVector offset = Move * MoveSpeed * DeltaTime;

	AddActorWorldOffset(offset, true);
}

void AMyPawn:: Look(const FInputActionValue& value)
{
	FVector2D LookInput = value.Get<FVector2D>();
	
	float LookSpeed = 50.0f;
	float DeltaTime = GetWorld()->GetDeltaSeconds();
	
	FRotator LookRotationP = SpringArmComp->GetRelativeRotation();
	LookRotationP.Pitch = FMath::Clamp(
		LookRotationP.Pitch + -(LookInput.Y)* LookSpeed * DeltaTime,
		-80.0f,80.0f
	);
	SpringArmComp->SetRelativeRotation(LookRotationP);
	
	FRotator LookRotationY = GetActorRotation();

	LookRotationY.Yaw += LookInput.X * LookSpeed * DeltaTime;
	SetActorRotation(LookRotationY);
	
}

