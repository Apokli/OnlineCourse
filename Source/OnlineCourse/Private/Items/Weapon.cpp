// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/Weapon.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "Interfaces/HitInterface.h"
#include "OnlineCourse/DebugMacros.h"

AWeapon::AWeapon()
{
	bPickable = true;
	bEquipable = true;
	bFloatingInScene = true;
	
	ItemMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	HitBox = CreateDefaultSubobject<UBoxComponent>(FName("Hit Box"));
	HitBox->SetupAttachment(ItemMesh);
	HitBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	HitBox->SetCollisionResponseToAllChannels(ECR_Overlap);
	HitBox->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);

	BoxTraceStart = CreateDefaultSubobject<USceneComponent>(FName("Box Trace Start"));
	BoxTraceStart->SetupAttachment(ItemMesh);
	BoxTraceEnd = CreateDefaultSubobject<USceneComponent>(FName("Box Trace End"));
	BoxTraceEnd->SetupAttachment(ItemMesh);
	
	PushBox = CreateDefaultSubobject<UBoxComponent>(FName("Push Box"));
	PushBox->SetupAttachment(ItemMesh);
	PushBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	HitBox->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::OnBoxBeginOverlap);
}

void AWeapon::Equip(USceneComponent* InParent, FName InSocketName)
{
	ItemState = EItemState::EIS_Equipped;
	AttachMeshToSocket(InParent, InSocketName);
	if (bFlipWhenEquipped)
	{
		ItemMesh->SetRelativeRotation(FRotator(180.0f, 0, 0));
	}
	if (UnSheathSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, UnSheathSound, GetActorLocation());
	}
	if (SceneInteractionSphere)
	{
		SceneInteractionSphere->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}

void AWeapon::Sheath(USceneComponent* InParent, FName InSocketName)
{
	ItemState = EItemState::EIS_Sheathed;
	AttachMeshToSocket(InParent, InSocketName);
	if (bFlipWhenEquipped)
	{
		ItemMesh->SetRelativeRotation(FRotator(180.0f, 0, 0));
	}
	if (SheathSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, SheathSound, GetActorLocation());
	}
}

void AWeapon::OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
                                   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnSphereBeginOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}

void AWeapon::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnSphereEndOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
}

void AWeapon::OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	const FVector Start = BoxTraceStart->GetComponentLocation();
	const FVector End = BoxTraceEnd->GetComponentLocation();
	
	FHitResult HitResult;
	ActorsToIgnore.AddUnique(this);

	UKismetSystemLibrary::BoxTraceSingle(
		this,
		Start,
		End,
		FVector(2.5f, 2.5f, 2.5f),
		BoxTraceStart->GetComponentRotation(),
		TraceTypeQuery1,
		false,
		ActorsToIgnore,
		EDrawDebugTrace::ForDuration,
		HitResult,
		true
	);
	if (AActor* Target = HitResult.GetActor())
	{
		if (IHitInterface* HitInterface = Cast<IHitInterface>(Target))
		{
			HitInterface->GetHit(HitResult.ImpactPoint);
		}
		ActorsToIgnore.AddUnique(Target);
	}
}
