// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemStates.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

class USphereComponent;

UCLASS()
class ONLINECOURSE_API AItem : public AActor
{
	GENERATED_BODY()
	
public:
	AItem();
	
	virtual void Tick(float DeltaTime) override;

	void AttachMeshToSocket(USceneComponent* InParent, FName InSocketName) const;
	
protected:
	virtual void BeginPlay() override;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USceneComponent* Root;
	
	UPROPERTY(VisibleAnywhere, BluePrintReadWrite)
	UStaticMeshComponent* ItemMesh;
	
	UPROPERTY(visibleAnywhere)
	USphereComponent* SceneInteractionSphere;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Properties")
	bool bPickable;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Properties")
	bool bEquipable;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item Properties")
	bool bFloatingInScene;

	EItemState ItemState = EItemState::EIS_Idle;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Oscillation")
	float Amplitude = 1.5;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Oscillation")
	float Period = 0.1;

	UFUNCTION(BlueprintPure, Category = "Oscillation")
	float CustomSin(float Value) const;
	
	UFUNCTION(BlueprintPure, Category = "Oscillation")
	float CustomCos(float Value) const;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Oscillation")
	float RunningTime;

	UFUNCTION()
	virtual void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	
	UFUNCTION()
	virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);
	
public:
	FORCEINLINE bool Pickable() const { return bPickable; }
	FORCEINLINE bool Equipable() const{ return bEquipable; }
	FORCEINLINE bool FloatingInWorld() const { return bFloatingInScene; }
};