
#include "Weapon.h"

AWeapon::AWeapon()
{
	ThrowWeaponTime = 0.7f;	
	bFalling = true;
	Ammo = 30;

	WeaponType = EWeaponType::EWT_SubmachineGun;

	AmmoType = EAmmoType::EAT_PISTOL_AMMO;
	ReloadMontageSection = "ReloadBullpup";

	MagazineCapacity = 30;

	ClipBoneName = "smg_clip";
	
	PrimaryActorTick.bCanEverTick = true;
}

void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GetItemState() == EItemState::EIS_Falling && bFalling)
	{
		const FRotator MeshRotation{ 0.f, GetItemMesh()->GetComponentRotation().Yaw, 0.f };
		GetItemMesh()->SetWorldRotation(MeshRotation, false, nullptr, ETeleportType::TeleportPhysics);
	}
}

void AWeapon::ThrowWeapon()
{
	const FRotator MeshRotation{ 0.f, GetItemMesh()->GetComponentRotation().Yaw, 0.f };
	GetItemMesh()->SetWorldRotation(MeshRotation, false, nullptr, ETeleportType::TeleportPhysics);

	const FVector MeshForward{ GetItemMesh()->GetForwardVector() };
	const FVector MeshRight{ GetItemMesh()->GetRightVector() };
	FVector ImpulseDirection = MeshRight.RotateAngleAxis(-20.f, MeshForward);

	constexpr float RandomRotation{ 30.f };
	ImpulseDirection = ImpulseDirection.RotateAngleAxis(RandomRotation, FVector(0.f, 0.f, 1.f));
	ImpulseDirection *= 10000.f;
	GetItemMesh()->AddImpulse(ImpulseDirection);

	bFalling = true;
	GetWorldTimerManager().SetTimer(ThrowWeaponTimer, this, &AWeapon::StopFalling, ThrowWeaponTime);
}

void AWeapon::DecrementAmmo()
{
	Ammo - 1 <= 0?Ammo = 0:--Ammo;
}

void AWeapon::ReloadAmmo(int32 Amount)
{
	/**
	 * NOTE:
	 * check() is like assert() in C. If the condition is false, it will stop your program dead with a log message,
	 * an error window and - if a debugger is attached - a break into the debugger.
	 * You cannot continue execution after this point.
	 */
	checkf(Ammo + Amount <= MagazineCapacity, TEXT("Attempted to reload with more than magazine capacity!"));
	Ammo += Amount;
}

void AWeapon::StopFalling()
{
	bFalling = false;
	SetItemState(EItemState::EIS_Pickup);
}
