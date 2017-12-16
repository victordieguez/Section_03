// Fill out your copyright notice in the Description page of Project Settings.

#include "Projectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AProjectile::AProjectile() {
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(FName("Projectile Movement"));
	ProjectileMovement->bAutoActivate = false;

	CollisionMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("Collision Mesh"));
	SetRootComponent(CollisionMesh);
	CollisionMesh->SetNotifyRigidBodyCollision(true);
	CollisionMesh->SetVisibility(false);

	LaunchBlast = CreateDefaultSubobject<UParticleSystemComponent>(FName("Launch Blast"));
	LaunchBlast->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);

	ImpactBlast = CreateDefaultSubobject<UParticleSystemComponent>(FName("Impact Blast"));
	ImpactBlast->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
	ImpactBlast->bAutoActivate = false;

	ExplosionForce = CreateDefaultSubobject<URadialForceComponent>(FName("Explosion Force"));
	ExplosionForce->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay() {
	Super::BeginPlay();
	if (ensure(CollisionMesh)) {
		CollisionMesh->OnComponentHit.AddDynamic(this, &AProjectile::OnHit);
	}
}

void AProjectile::LaunchProjectile(float Speed) {
	if (ensure(ProjectileMovement)) {
		ProjectileMovement->SetVelocityInLocalSpace(FVector::ForwardVector * Speed);
		ProjectileMovement->Activate();
	}
}

void AProjectile::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit) {
	if (ensure(LaunchBlast) && ensure(ImpactBlast) && ensure(ExplosionForce) && ensure(CollisionMesh)) {
		LaunchBlast->Deactivate();
		ImpactBlast->Activate();
		ExplosionForce->FireImpulse();
		SetRootComponent(ImpactBlast);
		CollisionMesh->DestroyComponent();

		UGameplayStatics::ApplyRadialDamage(
			this, ProjectileDamage, GetActorLocation(), 
			ExplosionForce->Radius, UDamageType::StaticClass(),
			TArray<AActor*>());

		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AProjectile::OnTimerExpired, DestroyDelay);
	}
}

void AProjectile::OnTimerExpired() {
	Destroy();
}