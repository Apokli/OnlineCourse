// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/Apokli.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Items/Item.h"
#include "Items/Weapon.h"
#include "Animation/AnimMontage.h"
#include "Components/BoxComponent.h"
#include "OnlineCourse/DebugMacros.h"

AApokli::AApokli()
{
	PrimaryActorTick.bCanEverTick = true;
    // Default fields
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0, 400.0f, 0);
	
	// Create components
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->TargetArmLength = 300.0f;
	CameraBoom->bUsePawnControlRotation = true;
	ViewCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("ViewCamera"));

	// Set up Hierarchy
	CameraBoom->SetupAttachment(GetRootComponent());
	ViewCamera->SetupAttachment(CameraBoom);
}

void AApokli::BeginPlay()
{
	Super::BeginPlay();
	
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			subsystem->AddMappingContext(ApokliMappingContext, 0);
		}
	}
}

void AApokli::Move(const FInputActionValue& Value)
{
	const FVector2D DirectionValue = Value.Get<FVector2D>();
	if (GetController() && !DirectionValue.IsZero())
	{
		const FRotator RotationValue = GetController()->GetControlRotation();
		const FRotator YawRotation(0.f, RotationValue.Yaw, 0.f);
		
		const FVector Forward = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Forward, DirectionValue.Y);
		const FVector Right = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(Right, DirectionValue.X);
	}
}

void AApokli::Look(const FInputActionValue& Value)
{
	const FVector2D LookAxisValue = Value.Get<FVector2D>();
	if (GetController())
	{
		AddControllerYawInput(LookAxisValue.X);
		AddControllerPitchInput(LookAxisValue.Y);
	}
}

void AApokli::Jump()
{
	Super::Jump();
	bPressedJump = true;
}

void AApokli::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);
	bPressedJump = false;
}

void AApokli::SetWeaponCollisionEnable(ECollisionEnabled::Type CollisionEnable)
{
	if (WeaponInHand && WeaponInHand->GetWeaponHitBox())
	{
		WeaponInHand->GetWeaponHitBox()->SetCollisionEnabled(CollisionEnable);
		WeaponInHand->ActorsToIgnore.Empty();
	}
}

void AApokli::PickUp(const FInputActionValue& Value)
{
	// Check if Item can be picked up
	if (OverlappingItem && OverlappingItem->Pickable())
	{
		// Check if Item is a Weapon
		if (AWeapon* OverlappingWeapon = Cast<AWeapon>(OverlappingItem))
		{
			AWeapon** EmptySlot = GetFirstEmptyWeaponSlot();
			// If has space for weapon
			if (EmptySlot != nullptr)
			{
				if (WeaponInHand == nullptr)
				{
					EquipWeapon(OverlappingWeapon, FName("Weapon_r"));
				}
				else
				{
					FName Slot = *EmptySlot == PrimaryWeapon ? FName("Primary_Weapon_Slot") : FName("Secondary_Weapon_Slot");
					OverlappingWeapon->Sheath(GetMesh(), Slot);
				}
				*EmptySlot = OverlappingWeapon;
			}
		}
	}
	OverlappingItem = nullptr;
}

AWeapon** AApokli::GetFirstEmptyWeaponSlot()
{
	if (PrimaryWeapon == nullptr) return &PrimaryWeapon;
	if (SecondaryWeapon == nullptr) return &SecondaryWeapon;
	return nullptr;
}

AWeapon** AApokli::GetFirstEquippedWeaponSlot()
{
	if (PrimaryWeapon != nullptr) return &PrimaryWeapon;
	if (SecondaryWeapon != nullptr) return &SecondaryWeapon;
	return nullptr;
}

void AApokli::EquipWeapon(AWeapon* Weapon, const FName Socket)
{
	if (Weapon->Equipable())
	{
		Weapon->Equip(GetMesh(), Socket);
		WeaponInHand = Weapon;
		SetPoseStateAccordingToWeaponType(Weapon);
	}
}

void AApokli::SetPoseStateAccordingToWeaponType(AWeapon* EquippedWeapon)
{
	switch (EquippedWeapon->GetEquipMode())
	{
	case EWeaponEquipModes::EWEM_OneHanded:
		CurrentPoseState = ECharacterPoseState::ECPS_EquippedOneHandedWeapon;
		break;
	case EWeaponEquipModes::EWEM_TwoHanded:
		CurrentPoseState = ECharacterPoseState::ECPS_EquippedTwoHandedWeapon;
		break;
	default:
		break;
	}
}

/*
 * Attack
*/

bool AApokli::CanAttack() const
{
	return CurrentActionState == ECharacterActionState::ECAS_Unoccupied &&
		CurrentPoseState != ECharacterPoseState::ECPS_Unequipped;
}

void AApokli::Attack(const FInputActionValue& Value)
{
	if (CanAttack())
	{
		CurrentActionState = ECharacterActionState::ECAS_Attacking;
		PlayAttackMontage();
	}
}

void AApokli::PlayAttackMontage() const
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && AttackMontage)
	{
		AnimInstance->Montage_Play(AttackMontage);
		const int32 Selection = FMath::RandRange(0, 1);
		FName SectionName = FName();
		switch (Selection)
		{
		case 0:
			SectionName = FName("Attack1");
			break;
		case 1:
			SectionName = FName("Attack2");
			break;
		default:
			break;
		}
		AnimInstance->Montage_JumpToSection(SectionName, AttackMontage);
	}
}

void AApokli::OnAttackEnd()
{
	CurrentActionState = ECharacterActionState::ECAS_Unoccupied;
}

/*
 * Weapon Equiping
*/

bool AApokli::CanSheath() const
{
	return CurrentActionState == ECharacterActionState::ECAS_Unoccupied &&
		WeaponInHand != nullptr;
}

bool AApokli::CanUnSheath() const
{
	return CurrentActionState == ECharacterActionState::ECAS_Unoccupied &&
			WeaponInHand == nullptr &&
			HasWeapon();
}

void AApokli::Sheath(const FInputActionValue& Value)
{
	if (CanSheath())
	{
		PlaySheathMontage(FName("Sheath"));
		CurrentActionState = ECharacterActionState::ECAS_Sheathing;
	}
	else if (CanUnSheath())
	{
		PreparedWeapon = *GetFirstEquippedWeaponSlot();
		PlaySheathMontage(FName("Unsheath"));
		CurrentActionState = ECharacterActionState::ECAS_UnSheathing;
	}
}

void AApokli::PlaySheathMontage(const FName& SectionName) const
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && EquipMontage)
	{
		AnimInstance->Montage_Play(EquipMontage);
		AnimInstance->Montage_JumpToSection(SectionName, EquipMontage);
	}
}

void AApokli::DisarmWeapon()
{
	if (WeaponInHand)
	{
		FName Slot = WeaponInHand == PrimaryWeapon ? FName("Primary_Weapon_Slot") : FName("Secondary_Weapon_Slot");
		WeaponInHand->Sheath(GetMesh(), Slot);
		WeaponInHand = nullptr;
		CurrentPoseState = ECharacterPoseState::ECPS_Unequipped;
	}
}

void AApokli::OnSheathEnd()
{
	CurrentActionState = ECharacterActionState::ECAS_Unoccupied;
}

void AApokli::ArmWeapon()
{
	if (WeaponInHand == nullptr && PreparedWeapon != nullptr)
	{
		EquipWeapon(PreparedWeapon, FName("Weapon_r"));
		PreparedWeapon = nullptr;
	}
}

void AApokli::OnUnsheathEnd()
{
	CurrentActionState = ECharacterActionState::ECAS_Unoccupied;
}

void AApokli::UsePrimaryWeapon(const FInputActionValue& Value)
{
	UseWeapon(PrimaryWeapon);
}

void AApokli::UseSecondaryWeapon(const FInputActionValue& Value)
{
	UseWeapon(SecondaryWeapon);
}

void AApokli::UseWeapon(AWeapon* Weapon)
{
	if (Weapon != nullptr)
	{
		// Unequipped
		if (CanUnSheath())
		{
			PreparedWeapon = Weapon;
			PlaySheathMontage(FName("Unsheath"));
			CurrentActionState = ECharacterActionState::ECAS_UnSheathing;
		}
		// Equipped with the other Weapon
		else if (CanSheath() && WeaponInHand != Weapon)
		{
			PreparedWeapon = Weapon;
			PlaySheathMontage(FName("Swap"));
			CurrentActionState = ECharacterActionState::ECAS_Sheathing;
		}
	}
}

void AApokli::SwapWeapon()
{
	DisarmWeapon();
	ArmWeapon();
}

void AApokli::OnSwapWeaponEnd()
{
	CurrentActionState = ECharacterActionState::ECAS_Unoccupied;
}

/*
 * Misc
 */

bool AApokli::HasWeapon() const
{
	return PrimaryWeapon != nullptr || SecondaryWeapon != nullptr;
}


void AApokli::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AApokli::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MovementAction, ETriggerEvent::Triggered, this, &AApokli::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AApokli::Look);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AApokli::Jump);
		EnhancedInputComponent->BindAction(PickUpAction, ETriggerEvent::Started, this, &AApokli::PickUp);
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Started, this, &AApokli::Attack);
		EnhancedInputComponent->BindAction(SheathAction, ETriggerEvent::Started, this, &AApokli::Sheath);
		EnhancedInputComponent->BindAction(UsePrimaryWeaponAction, ETriggerEvent::Started, this, &AApokli::UsePrimaryWeapon);
		EnhancedInputComponent->BindAction(UseSecondaryWeaponAction, ETriggerEvent::Started, this, &AApokli::UseSecondaryWeapon);
	}
}

