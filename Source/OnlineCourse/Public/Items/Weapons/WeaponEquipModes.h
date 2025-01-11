#pragma once

UENUM(BlueprintType)
enum class EWeaponEquipModes : uint8
{
	EWEM_OneHanded UMETA(DisplayName = "One Handed"),
	EWEM_TwoHanded UMETA(DisplayName = "Two Handed"),
	EWEM_DualWield UMETA(DisplayName = "Dual Wield")
};