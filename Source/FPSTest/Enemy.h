
#pragma once

#include "CoreMinimal.h"
#include "RangeWeaponHitInterface.h"
#include "GameFramework/Character.h"
#include "Enemy.generated.h"

class UParticleSystem;
class USoundCue;

UCLASS()
class FPSTEST_API AEnemy : public ACharacter, public IRangeWeaponHitInterface
{
	GENERATED_BODY()

public:
	AEnemy();

protected:
	virtual void BeginPlay() override;

	/** Particles for hitting by bullets */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess =  "true"))
	UParticleSystem* ImpactParticles;

	/** Sound for hitting by bullets */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	USoundCue* ImpactSound;

	/** Current enemy health */ 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	float Health;

	/** Max enemy health */ 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	float MaxHealth;

	/** Variables to store name of the head bone from Skeleton tree for enemy */ 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	FString HeadBone;

	void Die();

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void BulletHit_Implementation(FHitResult HitResult) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent,
		AController* EventInstigator, AActor* DamageCauser) override;

	FORCEINLINE FString GetHeadBone() const { return HeadBone; }
	

};
