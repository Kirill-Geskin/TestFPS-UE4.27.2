
#include "Item.h"

#include "FPSTestCharacter.h"
#include "Components/BoxComponent.h"
#include "Components/SphereComponent.h"
#include "Components/WidgetComponent.h"

AItem::AItem()
{
	PrimaryActorTick.bCanEverTick = true;

	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>("SkeletalMeshComponent");
	SetRootComponent(SkeletalMeshComponent);

	CollisionBox = CreateDefaultSubobject<UBoxComponent>("CollisionBox");
	CollisionBox->SetupAttachment(SkeletalMeshComponent);
	CollisionBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	CollisionBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	
	PickupWidget = CreateDefaultSubobject<UWidgetComponent>("PickupWidget");
	PickupWidget->SetupAttachment(GetRootComponent());

	SphereArea = CreateDefaultSubobject<USphereComponent>("SphereArea");
	SphereArea->SetupAttachment(GetRootComponent());

	ItemName = "Default";
	ItemAmmoCount = 0;
	ItemRarity = EItemRarity::EIR_Common;
	ItemState = EItemState::EIS_Pickup;
}

void AItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AItem::BeginPlay()
{
	Super::BeginPlay();

	if(PickupWidget)
	{
		PickupWidget->SetVisibility(false);
	}

	/** Sets ActiveStars array based on Item Rarity */
	SetActiveStars();
	
	SphereArea->OnComponentBeginOverlap.AddDynamic(this, &AItem::OnSphereOverlap);
	SphereArea->OnComponentEndOverlap.AddDynamic(this, &AItem::OnSphereEndOverlap);

	/** Set Item properties based on ItemState */
	SetItemProperties(ItemState);
	
}

void AItem::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		AFPSTestCharacter* FPSTestCharacter = Cast<AFPSTestCharacter>(OtherActor);
		if (FPSTestCharacter)
		{
			FPSTestCharacter->IncrementOverlappedItemCount(1);
		}
	}
}

void AItem::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor)
	{
		AFPSTestCharacter* FPSTestCharacter = Cast<AFPSTestCharacter>(OtherActor);
		if (FPSTestCharacter)
		{
			FPSTestCharacter->IncrementOverlappedItemCount(-1);
		}
	}
}

void AItem::SetActiveStars()
{
	for (int32 i = 0; i <= 5; i++)
	{
		ActiveStars.Add(false);
	}
	/**
	 * A small optimization for the classic Switch loop. For example, with EItemRarity::EIR_Legendary,
	 * it passes through all cases (since break is only at the end of the loop), and gets 5 stars in the widget
	 */
	switch (ItemRarity) 
	{
	case EItemRarity::EIR_Legendary:
		ActiveStars[5] = true;
	case EItemRarity::EIR_Rare:
		ActiveStars[4] = true;
	case EItemRarity::EIR_Uncommon:
		ActiveStars[3] = true;
	case EItemRarity::EIR_Common:
		ActiveStars[2] = true;
	case EItemRarity::EIR_Damaged:
		ActiveStars[1] = true;
		break;
	}
}

void AItem::SetItemState(EItemState State)
{
	ItemState = State;
	SetItemProperties(State);
}

void AItem::SetItemProperties(EItemState State) const
{
	switch (State)
	{
	case EItemState::EIS_Pickup:
		/** Set mesh properties */
		SkeletalMeshComponent->SetSimulatePhysics(false);
		SkeletalMeshComponent->SetEnableGravity(false);
		SkeletalMeshComponent->SetVisibility(true);
		SkeletalMeshComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		SkeletalMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		/** Set SphereArea properties */
		SphereArea->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
		SphereArea->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
		/** Set CollisionBox properties */
		CollisionBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		CollisionBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, 
			ECollisionResponse::ECR_Block);
		CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		break;
	case EItemState::EIS_Equipped:
		PickupWidget->SetVisibility(false);
		/** Set mesh properties */
		SkeletalMeshComponent->SetSimulatePhysics(false);
		SkeletalMeshComponent->SetEnableGravity(false);
		SkeletalMeshComponent->SetVisibility(true);
		SkeletalMeshComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		SkeletalMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		/** Set SphereArea properties */
		SphereArea->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		SphereArea->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		/** Set CollisionBox properties */
		CollisionBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		CollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		break;
	case EItemState::EIS_Falling:
		/** Set mesh properties */
		SkeletalMeshComponent->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		SkeletalMeshComponent->SetSimulatePhysics(true);
		SkeletalMeshComponent->SetEnableGravity(true);
		SkeletalMeshComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		SkeletalMeshComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic,
			ECollisionResponse::ECR_Block);
		/** Set SphereArea properties */
		SphereArea->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		SphereArea->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		/** Set CollisionBox properties */
		CollisionBox->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		CollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);

		break;
	}
}
