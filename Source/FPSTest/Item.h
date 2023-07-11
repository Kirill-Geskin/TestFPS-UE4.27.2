
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Item.generated.h"

class UBoxComponent;
class USkeletalMeshComponent;
class UWidgetComponent;
class USphereComponent;
class USoundCue;

/**
 * An Enum to determine the number of stars on the weapon widget based on the rarity of the weapon.
 * EIR_Damaged == 1 star → EIR_Legendary == 5 stars
 */
UENUM(BlueprintType)
enum class EItemRarity : uint8
{
	EIR_Damaged UMETA(DisplayName = "Damaged"),
	EIR_Common UMETA(DisplayName = "Common"),
	EIR_Uncommon UMETA(DisplayName = "Uncommon"),
	EIR_Rare UMETA(DisplayName = "Rare"),
	EIR_Legendary UMETA(DisplayName = "Legendary"),

	EIR_MAX UMETA(DisplayName = "DefaultMAX")
};

/**
 * EIS_Equipped (Held in hand, Attached to Socket ("RightHandSocket"),
 * Collision is off, EquippedWeapon holds reference to current equipped weapon)
 * 
 * EIS_Pickup (Siting on the ground, SphereArea and CollisionBox are active, not attached to anything)
 * 
 * EIS_EquipInterping (For picking up process, collision is off)  
 *
 * EIS_PickedUp (We are having items in ours inventory but they are not equipped, collision and visibility is off)
 *
 * EIS_Falling (For falling process, should block the floor, SphereArea and CollisionBox is off)
 */
UENUM(BlueprintType)

enum class EItemState : uint8
{
	EIS_Pickup UMETA(DisplayName = "Pickup"),
	EIS_EquipInterping UMETA(DisplayName = "EquipInterping"),
	EIS_PickedUp UMETA(DisplayName = "PickedUp"),
	EIS_Equipped UMETA(DisplayName = "Equipped"),
	EIS_Falling UMETA(DisplayName = "Falling"),

	EIS_MAX UMETA(DisplayName = "DefaultMAX")
};

UCLASS()
class FPSTEST_API AItem : public AActor
{
	GENERATED_BODY()
	
public:	

	AItem();

protected:

	virtual void BeginPlay() override;

	/** Called when overlapping SphereArea */
	UFUNCTION()
	void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	/** Called when End Overlapping SphereArea */
	UFUNCTION()
	void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	/** Sets the ActiveStars array based on rarity */
	void SetActiveStars();

	/** Sets properties of the Item's components based on State */
	void SetItemProperties(EItemState State) const;

public:	

	virtual void Tick(float DeltaTime) override;

private:

	/** Collision to show/hide HUD widget */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item properties", meta = (AllowPrivateAccess = true))
	UBoxComponent* CollisionBox;

	/** SkeletalMeshComponent for the item */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item properties", meta = (AllowPrivateAccess = true))
	USkeletalMeshComponent* SkeletalMeshComponent;

	/** Widget for items that we can pick up */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item properties", meta = (AllowPrivateAccess = true))
	UWidgetComponent* PickupWidget;

	/** Item trace when overlapped */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item properties", meta = (AllowPrivateAccess = true))
	USphereComponent* SphereArea;

	/** The name for pickup widget */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item properties", meta = (AllowPrivateAccess = true))
	FString ItemName;

	/** Inner Item count for ammo */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item properties", meta = (AllowPrivateAccess = true))
	int32 ItemAmmoCount;
	
	/** Item rarity - determines number of stars in Pickup Widget */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	EItemRarity ItemRarity;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	TArray<bool> ActiveStars;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	EItemState ItemState;

	/** Sound for pickupping items from the ground */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	USoundCue* PickupSound;

	/** Sound for equipping items */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Item Properties", meta = (AllowPrivateAccess = "true"))
	USoundCue* EquipSound;
	
public:

	FORCEINLINE UWidgetComponent* GetPickupWidget() const { return PickupWidget; }
	FORCEINLINE USphereComponent* GetSphereArea() const { return SphereArea; }
	FORCEINLINE UBoxComponent* GetBoxComponent() const { return CollisionBox; }
	FORCEINLINE EItemState GetItemState() const { return ItemState; }
	void SetItemState(EItemState State);
	FORCEINLINE USkeletalMeshComponent* GetItemMesh() const { return SkeletalMeshComponent; }
	FORCEINLINE USoundCue* GetPickupSound() const { return PickupSound; }
	FORCEINLINE USoundCue* GetEquipSound() const { return EquipSound; }

};
