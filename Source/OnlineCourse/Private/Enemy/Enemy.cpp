// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Enemy.h"

#include "Components/SkeletalMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/GameplayStatics.h"
#include "OnlineCourse/DebugMacros.h"

AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetCollisionObjectType(ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetMesh()->SetGenerateOverlapEvents(true);

	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}

void AEnemy::PlayHitReactMontage(const FName& SectionName) const
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && HitReactMontage)
	{
		AnimInstance->Montage_Play(HitReactMontage);
		AnimInstance->Montage_JumpToSection(SectionName, HitReactMontage);
	}
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemy::GetHit(const FVector& ImpactPoint)
{
	DirectionalHitReact(ImpactPoint);
	if (HitSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, HitSound, GetActorLocation());
	}
}
	
void AEnemy::DirectionalHitReact(const FVector& ImpactPoint) const
{
	const FVector Forward = GetActorForwardVector();
	const FVector ImpactPointFlattened = FVector(ImpactPoint.X, ImpactPoint.Y, GetActorLocation().Z);
	const FVector HitDirection = (ImpactPointFlattened - GetActorLocation()).GetSafeNormal();
	const FVector CrossProduct = FVector::CrossProduct(Forward, HitDirection);
	double HitAngle = FMath::RadiansToDegrees(FMath::Acos(FVector::DotProduct(Forward, HitDirection)));
	if (CrossProduct.Z > 0)
	{
		HitAngle = -HitAngle;
	}
	if (-45.f < HitAngle && HitAngle <= 45.f)
	{
		PlayHitReactMontage(FName("KnockBack"));
	}
	else if (45.f < HitAngle && HitAngle <= 135.f)
	{
		PlayHitReactMontage(FName("KnockRight"));
	}
	else if (-135.f < HitAngle && HitAngle <= -45.f)
	{
		PlayHitReactMontage(FName("KnockLeft"));
	}
	else
	{
		PlayHitReactMontage(FName("KnockFront"));
	}
}

