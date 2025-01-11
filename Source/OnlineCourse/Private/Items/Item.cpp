// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Item.h"
#include "Components/SphereComponent.h"
#include "Characters/Apokli.h"

AItem::AItem()
{
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("RootSceneComponent"));
	RootComponent = Root;
	
	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemStaticMeshComponent"));
	ItemMesh->SetupAttachment(Root);

	SceneInteractionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("SceneInteractionSphere"));
	SceneInteractionSphere->SetupAttachment(GetRootComponent());
}

float AItem::CustomSin(const float Value) const
{
	return Amplitude * FMath::Sin(Value / Period);
}

float AItem::CustomCos(const float Value) const
{
	return Amplitude * FMath::Cos(Value / Period);
}

void AItem::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AApokli* Apokli = Cast<AApokli>(OtherActor);
	if (Apokli)
	{
		Apokli->SetOverlappingItem(this);
	}
}

void AItem::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AApokli* Apokli = Cast<AApokli>(OtherActor);
	if (Apokli)
	{
		Apokli->SetOverlappingItem(nullptr);
	}
}

void AItem::AttachMeshToSocket(USceneComponent* InParent, FName InSocketName) const
{
	FAttachmentTransformRules TransformRules(EAttachmentRule::SnapToTarget, true);
	ItemMesh->AttachToComponent(InParent, TransformRules, InSocketName);
}

void AItem::BeginPlay()
{
	Super::BeginPlay();

	SceneInteractionSphere->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnSphereBeginOverlap);
	SceneInteractionSphere->OnComponentEndOverlap.AddDynamic(this, &AItem::OnSphereEndOverlap);
}	

void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	RunningTime += DeltaTime;

	if (ItemState == EItemState::EIS_Idle && bFloatingInScene)
	{
		AddActorWorldOffset(FVector(0.0f, 0.0f, CustomSin(RunningTime)));
	}
}

