// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items/Item.h"
#include "Weapons/WeaponEquipModes.h"
#include "Weapon.generated.h"

class USoundBase;
class UBoxComponent;

/**
 * 
 */
UCLASS()
class ONLINECOURSE_API AWeapon : public AItem
{
	GENERATED_BODY()
		
public:
	AWeapon();
	void Equip(USceneComponent* InParent, FName InSocketName);
	void Sheath(USceneComponent* InParent, FName InSocketName);

	TArray<AActor*> ActorsToIgnore;

protected:
	virtual void BeginPlay() override;
	
	virtual void OnSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult) override;
	virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex) override;
	UFUNCTION()
	void OnBoxBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon | Equip")
	EWeaponEquipModes EquipMode;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon | Equip")
	bool bFlipWhenEquipped;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon | Properties")
	USoundBase* UnSheathSound;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon | Properties")
	USoundBase* SheathSound;
	UPROPERTY(VisibleAnywhere)
	UBoxComponent* HitBox;
	UPROPERTY(VisibleAnywhere)
	UBoxComponent* PushBox;

private:
	UPROPERTY(VisibleAnywhere)
	USceneComponent* BoxTraceStart;
	UPROPERTY(VisibleAnywhere)
	USceneComponent* BoxTraceEnd;

public:
	FORCEINLINE EWeaponEquipModes GetEquipMode() const { return EquipMode; }
	FORCEINLINE UBoxComponent* GetWeaponHitBox() const { return HitBox; }
};
