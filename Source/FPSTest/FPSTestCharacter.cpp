
#include "FPSTestCharacter.h"

#include "Enemy.h"
#include "Item.h"
#include "RangeWeaponHitInterface.h"
#include "Weapon.h"
#include "Camera/CameraComponent.h"
#include "Engine/SkeletalMeshSocket.h"
#include "Kismet/GameplayStatics.h"
#include "Components/InputComponent.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Sound/SoundCue.h"

AFPSTestCharacter::AFPSTestCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	CombatState = ECombatState::ECS_Unoccupied;

	/** Base rates for turning/looking up */
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	StartingPistolAmmo = 85.f;
	StartingARAmmo = 120.f;

	/**
	 * Variables for aiming logic
	 */
	
	bAiming = false;
	/** CameraDefaultFOV is set in BeginPlay() */
	CameraDefaultFOV = 0.f; 
	CameraAimedFOV = 40.f;
	CameraCurrentFOV = 0.f;
	ZoomInterpolationSpeed = 20.f;

	/**
	 * Gun fire variables for automatic weapons
	 */
	
	AutomaticFireRate = 0.1f;
	bShouldFire = true;
	bFireKeyPressed = false;

	/**
	 * Variables for item traces 
	 */
	
	bShouldTraceForItems = false;
	OverlappedItemCount = 0.f;

	/** Create CameraSpringArmComponent. Pulls towards the character if there is a collision*/
	CameraSpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraSpringArmComponent"));
	CameraSpringArmComponent->SetupAttachment(GetRootComponent());
	CameraSpringArmComponent->TargetArmLength = 300.f;
	CameraSpringArmComponent->bUsePawnControlRotation = true;
	CameraSpringArmComponent->SocketOffset = FVector(0.f,50.f,50.f);

	/** Create follow camera*/
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(CameraSpringArmComponent, USpringArmComponent::SocketName);
	CameraComponent->bUsePawnControlRotation = false; // camera does not rotate to arm component

	/** Don`t rotate Player character when the controller rotates. Let the controller only affect the camera */
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = true;
	bUseControllerRotationRoll = false;

	/** Rotate Player Character to the direction of movement */
	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->RotationRate = FRotator(0.f, 540.f, 0.f);
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	HandSceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("HandSceneComponent"));
}

void AFPSTestCharacter::BeginPlay()
{
	Super::BeginPlay();

	if(CameraComponent)
	{
		CameraDefaultFOV = GetCameraComponent()->FieldOfView;
		CameraCurrentFOV = CameraDefaultFOV;
	}

	/** Spawn the default weapon and equip it */
	EquipWeapon(SpawnDefaultWeapon());

	InitializeAmmoMap();
}

void AFPSTestCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CameraInterpolationAiming(DeltaTime);

	/** Check OverlappedItemCount, then trace for items */
	TraceForItems();
}

void AFPSTestCharacter::FireWeapon()
{
	if(EquippedWeapon == nullptr) return;

	/** We are cheking our combat state */
	if(CombatState != ECombatState::ECS_Unoccupied) return;

	if(WeaponHasAmmo())
	{
		PlayFireSound();
		SendBullet();
		PlayGunfireMontage();
		/** subtract 1 from the weapons ammo */ 
		EquippedWeapon->DecrementAmmo();

		StartFireTimer();
	}
}

void AFPSTestCharacter::StartFireTimerCompleted()
 {
 	/**
 	 * if we continue to press the fire key, as soon as the timer finishes autofire reset,
 	 * we will check if fire key still pressing we start the fire timer all over again. And we will continue to fire
 	 */
 	CombatState = ECombatState::ECS_Unoccupied;
 
 	if (WeaponHasAmmo())
 	{
 		if (bFireKeyPressed)
 		{
 			FireWeapon();
 		}
 	}
 	else
 	{
 		ReloadWeapon();
 	}
 }
void AFPSTestCharacter::CameraInterpolationAiming(float DeltaTime)
{
	/** Set current camera FOV through interpolation function*/
	if(bAiming)
	{
		CameraCurrentFOV = FMath::FInterpTo(CameraCurrentFOV, CameraAimedFOV, DeltaTime, ZoomInterpolationSpeed);
	}
	else
	{
		CameraCurrentFOV = FMath::FInterpTo(CameraCurrentFOV, CameraDefaultFOV, DeltaTime, ZoomInterpolationSpeed);
	}
	GetCameraComponent()->SetFieldOfView(CameraCurrentFOV);
}

void AFPSTestCharacter::FireKeyPressed()
{
	bFireKeyPressed = true;
	FireWeapon();
}

void AFPSTestCharacter::FireKeyReleased()
{
	bFireKeyPressed = false;
}

void AFPSTestCharacter::StartFireTimer()
{
	CombatState = ECombatState::ECS_FireTimerInProgress;
	
	GetWorldTimerManager().SetTimer(AutoFireTimer, this,
			&AFPSTestCharacter::StartFireTimerCompleted, AutomaticFireRate);
}

bool AFPSTestCharacter::CrosshairTrace(FHitResult& HitResult, FVector& OutHitLocation)
{
	FVector2D ViewportSize;
	if(GEngine && GEngine->GameViewport)
	{
		GEngine->GameViewport->GetViewportSize(ViewportSize);
	}
	
	/** Get screen space location of crosshairs */
	FVector2D CrosshairLocation(ViewportSize.X / 2.f, ViewportSize.Y / 2.f);
	CrosshairLocation.Y -= 50.f;
	FVector CrosshairWorldPosition;
	FVector CrosshairWorldDirection;
	
	/**
	* Function DeprojectScreenToWorld return bool which you check in if() statement below.
	* Also DeprojectScreenToWorld() revrite CrosshairWorldPosition and CrosshairWorldDirection.
	*/
	if(UGameplayStatics::DeprojectScreenToWorld(UGameplayStatics::GetPlayerController(this,
	0), CrosshairLocation, CrosshairWorldPosition, CrosshairWorldDirection))
	{
		const FVector Start{ CrosshairWorldPosition };
		const FVector End{ Start + CrosshairWorldDirection * 50000.f };

		OutHitLocation = End;
		
		GetWorld()->LineTraceSingleByChannel(HitResult, Start, End,ECC_Visibility);

		if(HitResult.bBlockingHit)
		{
			OutHitLocation = HitResult.Location;
			return true;
		}
	}
	return false;
}

void AFPSTestCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	check(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &AFPSTestCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AFPSTestCharacter::MoveRight);
	PlayerInputComponent->BindAxis("TurnRate", this, &AFPSTestCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AFPSTestCharacter::LookUpAtRate);
	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);

	PlayerInputComponent->BindAction("Jump",IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump",IE_Released, this, &ACharacter::StopJumping);
	
	PlayerInputComponent->BindAction("FireButton",IE_Pressed, this, &AFPSTestCharacter::FireKeyPressed);
	PlayerInputComponent->BindAction("FireButton",IE_Released, this, &AFPSTestCharacter::FireKeyReleased);

	PlayerInputComponent->BindAction("AimingButton",IE_Pressed, this, &AFPSTestCharacter::AimingKeyPressed);
	PlayerInputComponent->BindAction("AimingButton",IE_Released, this, &AFPSTestCharacter::AimingKeyReleased);

	PlayerInputComponent->BindAction("Select", IE_Pressed, this, &AFPSTestCharacter::SelectButtonPressed);
	PlayerInputComponent->BindAction("Select", IE_Released, this, &AFPSTestCharacter::SelectButtonReleased);

	PlayerInputComponent->BindAction("ReloadButton", IE_Pressed, this, &AFPSTestCharacter::ReloadButtonPressed);
}

void AFPSTestCharacter::MoveForward(float Value)
{
	if(Controller && Value)
	{
		const FRotator Rotation {Controller->GetControlRotation()};
		const FRotator YawRotation {0, Rotation.Yaw, 0};

		const FVector Direction{ FRotationMatrix{YawRotation}.GetUnitAxis(EAxis::X) };
		AddMovementInput(Direction, Value);
	}
}

void AFPSTestCharacter::MoveRight(float Value)
{
	if (Controller && Value)
	{
		const FRotator Rotation{ Controller->GetControlRotation() };
		const FRotator YawRotation{ 0, Rotation.Yaw, 0 };

		const FVector Direction{ FRotationMatrix{YawRotation}.GetUnitAxis(EAxis::Y) };
		AddMovementInput(Direction, Value);
	}
}

void AFPSTestCharacter::TurnAtRate(float Rate)
{
	/** Calculate delta for this frame from the Rate information. deg/sec * sec/frame */
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds()); 
}

void AFPSTestCharacter::LookUpAtRate(float Rate)
{
	/** Calculate delta for this frame from the Rate information. deg/sec * sec/frame */
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds()); 
}



bool AFPSTestCharacter::GetBeamEndLocation(const FVector& MuzzleSocketLocation, FHitResult& OutHitResult)
{
	FVector OutBeamLocation;
	/** Check for crosshair trace hit */
	FHitResult CrosshairHitResult;
	
	if(CrosshairTrace(CrosshairHitResult,OutBeamLocation))
	{
		 OutBeamLocation = CrosshairHitResult.Location;
	}
	else
	{
		/** OutBeamLocation is the End location for the lne trace */
	}

	/** Second trace from the gub barrrel */
	const FVector WeaponTStartTrace{ MuzzleSocketLocation };
	const FVector StartToEnd{ OutBeamLocation - MuzzleSocketLocation };
	const FVector WeaponEndTrace{ MuzzleSocketLocation + StartToEnd + 1.25f };

	GetWorld()->LineTraceSingleByChannel(OutHitResult, WeaponTStartTrace, WeaponEndTrace,
		ECC_Visibility);

	/** WeaponTraceHit shows if we hit the object`s between barrel and end point */
	if(!OutHitResult.bBlockingHit)
	{
		OutHitResult.Location = OutBeamLocation;
		return false;
	}
	return true;
}

void AFPSTestCharacter::TraceForItems()
{
	if(bShouldTraceForItems)
	{
		FHitResult ItemTraceResult;
		FVector HitLocation;
		CrosshairTrace(ItemTraceResult,HitLocation);
		if(ItemTraceResult.bBlockingHit)
		{
			TraceHitItem = Cast<AItem>(ItemTraceResult.Actor);
			if(TraceHitItem && TraceHitItem->GetPickupWidget())
			{
				const FVector ItemLocation = ItemTraceResult.Location;
				const FVector ShooterLocation = GetActorLocation();
				const float Distance = FVector::Dist(ItemLocation, ShooterLocation);
				/**
				 * Resolves an issue where, while in one item sphere,
				 * you could call the widget of other items outside of the item sphere you're standing in
				 */
				if (Distance <= TraceHitItem->GetSphereArea()->GetScaledSphereRadius())
				{
					// Show item's pickup widget
					TraceHitItem->GetPickupWidget()->SetVisibility(true);
				}
			}
			/** We hit an AItem last frame */
			if(TraceHitItemLastFrame)
			{
				if (TraceHitItem != TraceHitItemLastFrame)
				{
					/** We are hitting a different AItem this frame from last frame or AItem is null */
					TraceHitItemLastFrame->GetPickupWidget()->SetVisibility(false);
				}
			}
			/** Store a reference to HitItem for next frame */
			TraceHitItemLastFrame = TraceHitItem;
		}
	}
	else if (TraceHitItemLastFrame)
	{
		/** No longer overlapping any items, ItemLastFrame should not show widget */
		TraceHitItemLastFrame->GetPickupWidget()->SetVisibility(false);
	}
}

void AFPSTestCharacter::AimingKeyPressed()
{
	bAiming = true;
}

void AFPSTestCharacter::AimingKeyReleased()
{
	bAiming = false;
}


void AFPSTestCharacter::IncrementOverlappedItemCount(int8 Amount)
{
	if (OverlappedItemCount + Amount <= 0)
	{
		OverlappedItemCount = 0;
		bShouldTraceForItems = false;
	}
	else
	{
		OverlappedItemCount += Amount;
		bShouldTraceForItems = true;
	}
}

AWeapon* AFPSTestCharacter::SpawnDefaultWeapon()
{
	if (DefaultWeaponClass)
	{
		return GetWorld()->SpawnActor<AWeapon>(DefaultWeaponClass);
	}
	return nullptr;
}

void AFPSTestCharacter::EquipWeapon(AWeapon* WeaponToEquip)
{
	if (WeaponToEquip)
	{
		/** Get the Right Hand Socket in SK_FPSTestCharacter */
		const USkeletalMeshSocket* HandSocket = GetMesh()->GetSocketByName(FName("RightHandSocket"));
		if (HandSocket)
		{
			/** Attach the Weapon to the hand socket RightHandSocket */
			HandSocket->AttachActor(WeaponToEquip, GetMesh());
		}
		/** Set EquippedWeapon to the newly spawned Weapon */
		EquippedWeapon = WeaponToEquip;
		EquippedWeapon->SetItemState(EItemState::EIS_Equipped);
	}
}

void AFPSTestCharacter::DropWeapon()
{
	if (EquippedWeapon)
	{
		const FDetachmentTransformRules DetachmentTransformRules(EDetachmentRule::KeepWorld, true);
		EquippedWeapon->GetItemMesh()->DetachFromComponent(DetachmentTransformRules);

		EquippedWeapon->SetItemState(EItemState::EIS_Falling);
		EquippedWeapon->ThrowWeapon();
	}
}

void AFPSTestCharacter::SelectButtonPressed()
{
	if (TraceHitItem)
	{
		if(TraceHitItem->GetPickupSound())
		{
			UGameplayStatics::PlaySound2D(this, TraceHitItem->GetPickupSound());
		}
		auto TraceHitWeapon = Cast<AWeapon>(TraceHitItem);
		SwapWeapon(TraceHitWeapon);
	}
}

void AFPSTestCharacter::SelectButtonReleased()
{
	
}

void AFPSTestCharacter::SwapWeapon(AWeapon* WeaponToSwap)
{
	if(TraceHitItem->GetEquipSound())
	{
		UGameplayStatics::PlaySound2D(this, TraceHitItem->GetEquipSound());
	}
	DropWeapon();
	EquipWeapon(WeaponToSwap);

	/** It is necessary to reset these variables after changing weapons */
	TraceHitItem = nullptr;
	TraceHitItemLastFrame = nullptr;
}

void AFPSTestCharacter::InitializeAmmoMap()
{
	AmmoMap.Add(EAmmoType::EAT_PISTOL_AMMO, StartingPistolAmmo);
	AmmoMap.Add(EAmmoType::EAT_AR_AMMO, StartingARAmmo);
}

bool AFPSTestCharacter::WeaponHasAmmo()
{
	if (EquippedWeapon == nullptr) return false;

	return EquippedWeapon->GetAmmo() > 0;
}

void AFPSTestCharacter::PlayFireSound()
{
	/** We are checking if GunFireSound is setup in UE and play it */
	if(GunFireSound)
	{
		UGameplayStatics::PlaySound2D(this, GunFireSound);
	}
}

void AFPSTestCharacter::SendBullet()
{
	/**
	 * 1. We are checking if BarrelSocket is setup in UE by name "BarrelSocket"
	 * and store socket location in local veriable
	 */
	const USkeletalMeshSocket* BarrelSocket = EquippedWeapon->GetItemMesh()->GetSocketByName("BarrelSocket");
	if(BarrelSocket)
	{
		/** 2. Save transform of BarrelSocket if variable*/
		const FTransform SocketTransform = BarrelSocket->GetSocketTransform(EquippedWeapon->GetItemMesh());

		/** 3. We are checking if MuzzleFlashParticle is setup in UE and spawn it */
		if(MuzzleFlashParticle)
		{
			UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), MuzzleFlashParticle, SocketTransform);
		}

		/** 4. We call GetBeamEndLocation() function if it return true we spawn two particle effects*/ 
		FHitResult BeamHitResult;
		bool bBeamEnd = GetBeamEndLocation(SocketTransform.GetLocation(), BeamHitResult);
		if(bBeamEnd)
		{
			/** Does hit Actor implement RangeWeaponHitInterface? */
			if (BeamHitResult.Actor.IsValid())
			{
				IRangeWeaponHitInterface* RangeWeaponHitInterface = Cast<IRangeWeaponHitInterface>(BeamHitResult.Actor.Get());
				if (RangeWeaponHitInterface)
				{
					RangeWeaponHitInterface->BulletHit_Implementation(BeamHitResult);
				}
				
				
				AEnemy* HitEnemy = Cast<AEnemy>(BeamHitResult.Actor.Get());
				if (HitEnemy)
				{
					if (BeamHitResult.BoneName.ToString() == HitEnemy->GetHeadBone())
					{
						// Head shot
						UGameplayStatics::ApplyDamage(
							BeamHitResult.Actor.Get(),
							EquippedWeapon->GetHeadShotDamage(),
							GetController(),
							this,
							UDamageType::StaticClass());
						UE_LOG(LogTemp, Warning, TEXT("HitComponent: %s"), *BeamHitResult.BoneName.ToString());
					}
					else
					{
						// Body shot
						UGameplayStatics::ApplyDamage(
							BeamHitResult.Actor.Get(),
							EquippedWeapon->GetDamage(),
							GetController(),
							this,
							UDamageType::StaticClass());
						UE_LOG(LogTemp, Warning, TEXT("HitComponent: %s"), *BeamHitResult.BoneName.ToString());
					}
				}
			}

			else
			{
				if(ImpactParticles)
				{
					UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactParticles, BeamHitResult.Location);
				}
			}
			
			if(ImpactParticles)
			{
				UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),ImpactParticles, BeamHitResult.Location);
			}

			if(BeamParticles)
			{
				if(UParticleSystemComponent* Beam = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(),
					BeamParticles, SocketTransform))
				{
					Beam->SetVectorParameter(FName("Target"), BeamHitResult.Location);
				}
			}
		}
	}
}

void AFPSTestCharacter::PlayGunfireMontage()
{
	/** Plying animation montage*/
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if(AnimInstance && HipFireMontage)
	{
		AnimInstance->Montage_Play(HipFireMontage);
		AnimInstance->Montage_JumpToSection(FName("StartFire"));
	}
}

void AFPSTestCharacter::ReloadButtonPressed()
{
	ReloadWeapon();
}

void AFPSTestCharacter::ReloadWeapon()
{
	if (CombatState != ECombatState::ECS_Unoccupied) return;
	if (EquippedWeapon == nullptr) return;
	if (CarryingAmmo())
	{
		CombatState = ECombatState::ECS_Reloading;
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		if (AnimInstance && ReloadMontage)
		{
			AnimInstance->Montage_Play(ReloadMontage);
			AnimInstance->Montage_JumpToSection(EquippedWeapon->GetReloadMontageSection());
		}
	}
}

bool AFPSTestCharacter::CarryingAmmo()
{
	if (EquippedWeapon == nullptr) return false;

	auto AmmoType = EquippedWeapon->GetAmmoType();

	if (AmmoMap.Contains(AmmoType))
	{
		return AmmoMap[AmmoType] > 0;
	}

	return false;
}

void AFPSTestCharacter::FinishReloading()
{
	CombatState = ECombatState::ECS_Unoccupied;
	if (EquippedWeapon == nullptr) return;
	const auto AmmoType{ EquippedWeapon->GetAmmoType() };

	if (AmmoMap.Contains(AmmoType))
	{
		/** Amount of ammo the Character is carrying of the EquippedWeapon type */
		int32 CarriedAmmo = AmmoMap[AmmoType];
		
		const int32 MagEmptySpace = EquippedWeapon->GetMagazineCapacity()- EquippedWeapon->GetAmmo();

		if (MagEmptySpace > CarriedAmmo)
		{
			/** Reload the magazine with all the ammo we are carrying */
			EquippedWeapon->ReloadAmmo(CarriedAmmo);
			CarriedAmmo = 0;
			AmmoMap.Add(AmmoType, CarriedAmmo);
		}
		else
		{
			EquippedWeapon->ReloadAmmo(MagEmptySpace);
			CarriedAmmo -= MagEmptySpace;
			AmmoMap.Add(AmmoType, CarriedAmmo);
		}
	}
}

void AFPSTestCharacter::GrabClip()
{
	if (EquippedWeapon == nullptr) return;
	if (HandSceneComponent == nullptr) return;
	
	/** Index for the clip bone on the Equipped Weapon */
	const int32 ClipBoneIndex{ EquippedWeapon->GetItemMesh()->GetBoneIndex(EquippedWeapon->GetClipBoneName()) };
	/** Store the transform of the clip */
	ClipTransform = EquippedWeapon->GetItemMesh()->GetBoneTransform(ClipBoneIndex);

	const FAttachmentTransformRules AttachmentRules(EAttachmentRule::KeepRelative, true);
	HandSceneComponent->AttachToComponent(GetMesh(), AttachmentRules, FName(TEXT("Hand_L")));
	HandSceneComponent->SetWorldTransform(ClipTransform);

	EquippedWeapon->SetMovingClip(true);
}

void AFPSTestCharacter::ReplaceClip()
{
	EquippedWeapon->SetMovingClip(false);
}
