#pragma once

UENUM(BlueprintType)
enum class EAmmoType : uint8
{
	EAT_PISTOL_AMMO UMETA(DisplayName = "PistolAmmo"),
	EAT_AR_AMMO UMETA(DisplayName = "AssaultRifleAmmo"),

	EAT_MAX UMETA(DisplayName = "DefaultMAX")
};