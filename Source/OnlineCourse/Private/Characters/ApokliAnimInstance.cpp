// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/ApokliAnimInstance.h"
#include "Characters/Apokli.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UApokliAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	Apokli = Cast<AApokli>(TryGetPawnOwner());

	if (Apokli)
	{
		ApokliMovementComponent = Apokli->GetCharacterMovement();
	}

	bEnableIKTrace = true;
	bEnableIKTraceRotation = false;
}

void UApokliAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (ApokliMovementComponent)
	{
		GroundSpeed = UKismetMathLibrary::VSizeXY(ApokliMovementComponent->Velocity);
		GroundAcceleration = UKismetMathLibrary::VSizeXY(ApokliMovementComponent->GetCurrentAcceleration());
		VerticalVelocity = ApokliMovementComponent->Velocity.Z;
		IsJumping = Apokli->bPressedJump;
		IsFalling = ApokliMovementComponent->IsFalling();
		CharacterPoseState = Apokli->GetCharacterPoseState();
	}
}
