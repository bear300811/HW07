#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Components/CapsuleComponent.h"
#include "MyPawn.generated.h"

class USpringArmComponent;
class UCameraComponent;
class USkeletalMeshComponent;
struct FInputActionValue;


UCLASS()
class HW07_API AMyPawn : public APawn
{
	GENERATED_BODY()

public:
	
	AMyPawn();
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Root")
	UCapsuleComponent* CapsuleComp;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SkeletalMesh")
	USkeletalMeshComponent* SkeletalMeshComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	USpringArmComponent* SpringArmComp;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	UCameraComponent* CameraComp;

protected:
	
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void Tick(float DeltaTime) override;
	
	UFUNCTION()
	void Move(const FInputActionValue& value);
	UFUNCTION()
	void Look(const FInputActionValue& value);
private:
	UPROPERTY(EditAnywhere,Category = "Movement")
	float MoveSpeed;
};
