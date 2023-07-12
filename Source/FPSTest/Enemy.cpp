
#include "Enemy.h"

#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

AEnemy::AEnemy()
{
	Health = 100.f;
	MaxHealth = 100.f;
	
	PrimaryActorTick.bCanEverTick = true;
}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	GetMesh()->SetCollisionResponseToChannel(ECC_Visibility,ECR_Block);
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AEnemy::Die()
{
	
}

void AEnemy::BulletHit_Implementation(FHitResult HitResult)
{
	if(ImpactSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ImpactSound, GetActorLocation());
	}
	if (ImpactParticles)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactParticles, HitResult.Location,
			FRotator(0.f), true);
	}
}

float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator,
	AActor* DamageCauser)
{
	if (Health - DamageAmount <= 0.f)
	{
		Health = 0.f;
	}
	else
	{
		Health -= DamageAmount;
	}
	return DamageAmount;
}

