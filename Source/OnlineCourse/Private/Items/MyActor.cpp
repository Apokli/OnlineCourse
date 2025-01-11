// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/MyActor.h"
#include "OnlineCourse/DebugMacros.h"
#include "Components/SphereComponent.h"

AMyActor::AMyActor()
{
	PrimaryActorTick.bCanEverTick = true;

	ActorMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MyActorStaticMeshComponent"));
	RootComponent = ActorMesh;

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->SetupAttachment(GetRootComponent());
}

float AMyActor::CustomSin(const float Value) const
{
	return Amplitude * FMath::Sin(Value / Period);
}

float AMyActor::CustomCos(const float Value) const
{
	return Amplitude * FMath::Cos(Value / Period);
}

void AMyActor::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	const FString OtherActorName = OtherActor->GetName();
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(1, 30, FColor::Red, OtherActorName);
	}
}

void AMyActor::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	DEBUG(FString(TEXT("End Overlap")));
}


void AMyActor::BeginPlay()
{
	Super::BeginPlay();

	SetActorLocation(FVector(-5840.f, 1690.f, 1600.f));

	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AMyActor::OnSphereBeginOverlap);
	SphereComponent->OnComponentEndOverlap.AddDynamic(this, &AMyActor::OnSphereEndOverlap);
}	

void AMyActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	RunningTime += DeltaTime;

	// float DeltaZ = FMath::Sin(RunningTime / Period) * Amplitude;
	
	// AddActorLocalOffset(FVector(0., 0., DeltaZ));
}

