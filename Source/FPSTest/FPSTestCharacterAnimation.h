
#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "FPSTestCharacterAnimation.generated.h"

/**
 * 
 */

class AFPSTestCharacter;

UCLASS()
class FPSTEST_API UFPSTestCharacterAnimation : public UAnimInstance
{
	GENERATED_BODY()
	
	/** UpdateAnimationProperties behave like tick function from actors */
	UFUNCTION(BlueprintCallable)
	void UpdateAnimationProperties(float DeltaTime);
	
	/** NativeInitializeAnimation works like begin play for actors, but for UAnimInstance */
	virtual void NativeInitializeAnimation() override;

private:

	/** This pointer to APlayerCharacter, but before initializing int`s a pointer to nowhere */ 
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	AFPSTestCharacter* FPSTestCharacter;

	/** The speed of the player character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float PlayerCharacterSpeed;

	/** Whether or not the character is in the air */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	bool bIsInAir;

	/** Whether or not the character is moving */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	bool bIsAccelerating;

	/** Offset yaw used for strafing */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float MovementOffsetYaw;

	/** Offset yaw the frame before we stopped moving */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	float LastMovementOffsetYaw;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement, meta = (AllowPrivateAccess = "true"))
	bool bAiming;
};