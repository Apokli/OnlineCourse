#pragma once

UENUM(BlueprintType)
enum class ECharacterPoseState : uint8
{
	ECPS_Unequipped UMETA(DisplayName = "Unequipped"),
	ECPS_EquippedOneHandedWeapon UMETA(DisplayName = "Equipped One Handed Weapon"),
	ECPS_EquippedTwoHandedWeapon UMETA(DisplayName = "Equipped Two Handed Weapon")
};

UENUM(BlueprintType)
enum class ECharacterActionState : uint8
{
	ECAS_Unoccupied UMETA(DisplayName = "Unoccupied"),
	ECAS_Sheathing UMETA(DisplayName = "Sheathing"),
	ECAS_UnSheathing UMETA(DisplayName = "Unsheathing"),
	ECAS_Attacking UMETA(DisplayName = "Attacking")
};