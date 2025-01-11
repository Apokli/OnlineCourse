// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "ApokliStates.h"
#include "Apokli.generated.h"

class UInputMappingContext;
class UInputAction;
class USpringArmComponent;
class UCameraComponent;
class AItem;
class UAnimMontage;
class AWeapon;

UCLASS()
class ONLINECOURSE_API AApokli : public ACharacter
{
	GENERATED_BODY()

public:
	AApokli();

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void Jump() override;
	virtual void Landed(const FHitResult& Hit) override;

	UFUNCTION(BlueprintCallable)
	void SetWeaponCollisionEnable(ECollisionEnabled::Type CollisionEnable);

	bool bPressedJump;
	
protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, Category = Input)
	UInputMappingContext* ApokliMappingContext;

	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* MovementAction;
	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* LookAction;
	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* JumpAction;
	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* PickUpAction;
	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* AttackAction;
	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* SheathAction;
	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* UsePrimaryWeaponAction;
	UPROPERTY(EditAnywhere, Category = Input)
	UInputAction* UseSecondaryWeaponAction;

	/*
	 * Callbacks for Input
	 */
	void Look(const FInputActionValue& Value);
	void Move(const FInputActionValue& Value);
	void PickUp(const FInputActionValue& Value);
	void Attack(const FInputActionValue& Value);
	void Sheath(const FInputActionValue& Value);
	void UsePrimaryWeapon(const FInputActionValue& Value);
	void UseSecondaryWeapon(const FInputActionValue& Value);
	/*
	 * Play montage functions
	 */
	void PlayAttackMontage() const;
	void PlaySheathMontage(const FName& SectionName) const;
	/*
	 * Middle of Actions
	 */
	UFUNCTION(BlueprintCallable)
	void DisarmWeapon();
	UFUNCTION(BlueprintCallable)
	void ArmWeapon();
	UFUNCTION(BlueprintCallable)
	void SwapWeapon();
	/*
	 * End of Actions
	 */
	UFUNCTION(BlueprintCallable)
	void OnAttackEnd();
	UFUNCTION(BlueprintCallable)
	void OnSheathEnd();
	UFUNCTION(BlueprintCallable)
	void OnUnsheathEnd();
	UFUNCTION(BlueprintCallable)
	void OnSwapWeaponEnd();
	
	/*
	 * checks
	 */
	bool CanAttack() const;
	bool CanSheath() const;
	bool CanUnSheath() const;
	bool HasWeapon() const;

private:
	/*
	 * Components
	 */
	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* ViewCamera;
	/*
	 * States
	 */
	ECharacterPoseState CurrentPoseState = ECharacterPoseState::ECPS_Unequipped;
	
	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	ECharacterActionState CurrentActionState = ECharacterActionState::ECAS_Unoccupied;
	/*
	 * Gameplay
	 */
	UPROPERTY(VisibleInstanceOnly)
	AItem* OverlappingItem = nullptr;
	UPROPERTY(VisibleInstanceOnly)
	AWeapon* PrimaryWeapon = nullptr;
	UPROPERTY(VisibleInstanceOnly)
	AWeapon* SecondaryWeapon = nullptr;
	UPROPERTY(VisibleInstanceOnly)
	AWeapon* WeaponInHand = nullptr;
	UPROPERTY(VisibleInstanceOnly)
	AWeapon* PreparedWeapon = nullptr;
	
	AWeapon** GetFirstEmptyWeaponSlot();
	AWeapon** GetFirstEquippedWeaponSlot();
	void UseWeapon(AWeapon* Weapon);
	void EquipWeapon(AWeapon* Weapon, const FName Socket);
	/*
	 * Utils
	 */
	void SetPoseStateAccordingToWeaponType(AWeapon* EquippedWeapon);
	/*
	 * Animation Montages
	 */
	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* AttackMontage;
	UPROPERTY(EditDefaultsOnly, Category = Montages)
	UAnimMontage* EquipMontage;

public:
	FORCEINLINE void SetOverlappingItem(AItem* Item) { this->OverlappingItem = Item; }
	FORCEINLINE ECharacterPoseState GetCharacterPoseState() const { return CurrentPoseState; }
};
