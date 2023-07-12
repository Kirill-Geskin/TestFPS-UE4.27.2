// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ENUM_Library.h"
#include "FPSTestCharacter.generated.h"

class AItem;
class USoundCue;
class UCameraComponent;
class USpringArmComponent;
class UParticleSystem;
class UAnimMontage;
class AWeapon;



UENUM(BlueprintType)
enum class ECombatState : uint8
{
	ECS_Unoccupied UMETA(DisplayName = "Unoccupied"),
	ECS_FireTimerInProgress UMETA(DisplayName = "FireTimerInProgress"),
	ECS_Reloading UMETA(DisplayName = "Reloading"),

	ECS_MAX UMETA(DisplayName = "DefaultMAX")
};

UCLASS(config=Game)
class AFPSTestCharacter : public ACharacter
{
	GENERATED_BODY()
public:
	
	AFPSTestCharacter();

protected:
	
	virtual void BeginPlay() override;

	void MoveForward(float Value);
	void MoveRight(float Value);

	/**
	* Called via input to turn at a given rate.
	* @param Rate This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	*/
	void TurnAtRate(float Rate);

	/**
	* Called via input to look up/down at a given rate.
	* @param Rate This is a normalized rate, i.e. 1.0 means 100% of desired rate
	*/
	void LookUpAtRate(float Rate);

	/** Called when fire button is pressed */
	void FireWeapon();

	bool GetBeamEndLocation(const FVector& MuzzleSocketLocation, FHitResult& OutHitResult);

	/** Trace from crosshair to target */
	bool CrosshairTrace(FHitResult& HitResult, FVector& OutHitLocation);

	/**
	 *	Section for items tracing logic
	 */
	
	/** True if we should trace every frame for items */
	bool bShouldTraceForItems;

	/** Number of overlapped AItems */
	int8 OverlappedItemCount;
	
	/** Trace for items if OverlappedItemCount > 0 */
	void TraceForItems();

	/**
	 *	Section for aiming logic
	 */
	
	/** Set bAiming true/false */
	void AimingKeyPressed();
	void AimingKeyReleased();

	/** Default game FOV. bAiming == false*/
	float CameraDefaultFOV;

	/** FOV after aiming. bAiming == true */
	float CameraAimedFOV;

	/** CameraCurrentFOV */
	float CameraCurrentFOV;
	
	void CameraInterpolationAiming(float DeltaTime);

	/**
	 *	Section for automatic fire logic.
	 *
	 *	FireKeyPressed() --> StartFireTimer() if bShouldFire == true we call FireWeapon() and start timer,
	 *	as soon as timer finishes, then --> StartFireTimerCompleted() gets called (it`s callback for the timer),
	 *	if fire key is still held down, then we call start fire timer again (we doing this in loop, until we
	 *	release the fire key).
	 *	If the fire key released, we call FireKeyReleased()
	 */

	void FireKeyPressed();
	void FireKeyReleased();
	
	void StartFireTimer();

	UFUNCTION()
	void StartFireTimerCompleted();

	/**
	 *	Section for weapon equipping
	 */
	
	/** Spawns a default weapon and equips it */
	AWeapon* SpawnDefaultWeapon();

	/** Takes a weapon and attaches it to the mesh */
	void EquipWeapon(AWeapon* WeaponToEquip);

	/** Detach weapon and let it fall to the ground */
	void DropWeapon();
	
	void SelectButtonPressed();
	void SelectButtonReleased();

	/** Drops currently equipped Weapon and Equips TraceHitItem */
	void SwapWeapon(AWeapon* WeaponToSwap);

	/**
	 *	Section for different weapons ammo logic
	 */
	
	/** Initialize the Ammo Map with ammo values */
	void InitializeAmmoMap();

	/** Check to make sure our weapon has ammo */
	bool WeaponHasAmmo();

	void PlayFireSound();

	void SendBullet();

	void PlayGunfireMontage();
	
	/**
	 *	Section for reload weapon logic
	 */
	
	/** Bound to the R key and Gamepad Face Button Left */
	void ReloadButtonPressed();
	
	/** Handle reloading of the weapon */
	void ReloadWeapon();
	
	/** Checks to see if we have ammo of the EquippedWeapon's ammo type */
	bool CarryingAmmo();

	/** Called from ABP_FPSTestCharacter with GrabClip notify from AM_Reload */
	UFUNCTION(BlueprintCallable)
	void GrabClip();

	/** Called from ABP_FPSTestCharacter with ReplaceClip notify from AM_Reload */
	UFUNCTION(BlueprintCallable)
	void ReplaceClip();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	/** CameraSpringArmComponent positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraSpringArmComponent;

	/** Camera that follows the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* CameraComponent;

	/** Base turn rate */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	float BaseTurnRate;

	/** Base look up/down rate */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	float BaseLookUpRate;

	/** Boolean variable responsible for the aiming state */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess =  "true"))
	bool bAiming;

	/** Empty pointer for Gun Sound Que */ 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess =  "true"))
	USoundCue* GunFireSound;

	/** Empty pointer for muzzle flash particles. Spawned at BarrelSocket */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess =  "true"))
	UParticleSystem*  MuzzleFlashParticle;

	/** Empty pointer for Impact from shots */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess =  "true"))
	UParticleSystem* ImpactParticles;

	/** Empty pointer for beam particles system */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess =  "true"))
	UParticleSystem* BeamParticles;
	
	/** Montage for firing the weapon */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess =  "true"))
	UAnimMontage* HipFireMontage;

	/** Interpolation speed when aiming */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess =  "true"))
	float ZoomInterpolationSpeed;

	/** Holds reference to currently equipped Weapon */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	AWeapon* EquippedWeapon;

	/** Set this in Blueprint for the default Weapon class */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AWeapon> DefaultWeaponClass;

	/** Variable to store last AItem we hit last frame by trace */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Items, meta = (AllowPrivateAccess =  "true") )
	AItem* TraceHitItemLastFrame;

	/** The item currently hit by our trace in TraceForItems (could be null) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	AItem* TraceHitItem;

	/** Combat State, can only fire or reload if Unoccupied */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	ECombatState CombatState;


	/**
	*	Section for different weapons ammo logic
	*/
	
	/** Map to keep track of ammo of the different ammo types */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Items, meta = (AllowPrivateAccess = "true"))
	TMap<EAmmoType, int32> AmmoMap;

	/** Starting amount of Pistol ammo */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Items, meta = (AllowPrivateAccess = "true"))
	int32 StartingPistolAmmo;

	/** Starting amount of AR ammo */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Items, meta = (AllowPrivateAccess = "true"))
	int32 StartingARAmmo;

	/**
	 *	Section for automatic fire logic
	 */
	
	/** keys for weapon fire pressed */
	bool bFireKeyPressed;

	bool bShouldFire;

	float AutomaticFireRate;

	FTimerHandle AutoFireTimer;

	/**
	 *	Section for reload weapon
	 */
	
	/** Montage for reload animations */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	UAnimMontage* ReloadMontage;

	UFUNCTION(BlueprintCallable)
	void FinishReloading();
	
	/** Transform of the clip when we first grab the clip during reloading */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	FTransform ClipTransform;

	/** Scene component to attach to the Character's hand during reloading */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	USceneComponent* HandSceneComponent;

public:
	/** Returns CameraSpringArmComponent subobject*/
	FORCEINLINE USpringArmComponent* GetCameraSpringArmComponent() const { return CameraSpringArmComponent; }

	/** Returns CameraComponent subobject*/
	FORCEINLINE UCameraComponent* GetCameraComponent() const { return CameraComponent; }

	FORCEINLINE bool GetAiming() const { return bAiming; }

	FORCEINLINE int8 GetOverlappedItemCount() const { return OverlappedItemCount; }

	/** Adds/subtracts to/from OverlappedItemCount and updates bShouldTraceForItems */
	void IncrementOverlappedItemCount(int8 Amount);
};

