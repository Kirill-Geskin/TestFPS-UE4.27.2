// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "FPSTestPlayerController.generated.h"

/**
 * 
 */

class UUserWidget;

UCLASS()
class FPSTEST_API AFPSTestPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AFPSTestPlayerController();

protected:

	virtual void BeginPlay() override;

private:
	/** Reference to the Overall HUD Overlay Blueprint Class */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Widgets, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UUserWidget> HUDOverlayClass;
	
	/** Variable to hold the HUD Overlay Widget after creating it */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Widgets, meta = (AllowPrivateAccess = "true"))
	UUserWidget* HUDOverlay;
};
