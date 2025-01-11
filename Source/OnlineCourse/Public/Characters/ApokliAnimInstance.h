// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "ApokliStates.h"
#include "ApokliAnimInstance.generated.h"

class AApokli;
class UCharacterMovementComponent;

/**
 * 
 */
UCLASS()
class ONLINECOURSE_API UApokliAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
	AApokli* Apokli;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
	UCharacterMovementComponent* ApokliMovementComponent;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
	float GroundSpeed;
	
	UPROPERTY(BlueprintReadOnly, Category = Movement)
	float GroundAcceleration;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
	float VerticalVelocity;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
	bool IsJumping;

	UPROPERTY(BlueprintReadOnly, Category = Movement)
	bool IsFalling;
	
	UPROPERTY(BlueprintReadWrite, Category = IK)
	bool bEnableIKTrace;
	
	UPROPERTY(BlueprintReadWrite, Category = IK)
	bool bEnableIKTraceRotation;
	
	UPROPERTY(BlueprintReadOnly, Category = "Movement | Character Pose State")
	ECharacterPoseState CharacterPoseState;
};
