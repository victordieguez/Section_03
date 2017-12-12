// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAimingComponent.h"
#include "TankBarrel.h"
#include "TankTurret.h"
#include "Projectile.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UTankAimingComponent::UTankAimingComponent() {
	PrimaryComponentTick.bCanEverTick = true;
}

void UTankAimingComponent::BeginPlay() {
	Super::BeginPlay();
	LastFireTime = FPlatformTime::Seconds();
}

void UTankAimingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if ((FPlatformTime::Seconds() - LastFireTime) > ReloadTimeInSeconds) {
		FiringState = EFiringState::Locked;
	} else {
		FiringState = EFiringState::Reloading;
	}
}

void UTankAimingComponent::Initialize(UTankTurret* Turret, UTankBarrel* Barrel) {
	this->Turret = Turret;
	this->Barrel = Barrel;
}

void UTankAimingComponent::AimAt(FVector HitLocation) {
	if (ensure(Barrel) && ensure(Turret)) {
		FVector TossVelocity;
		if (UGameplayStatics::SuggestProjectileVelocity(this, TossVelocity, Barrel->GetSocketLocation(FName("Projectile")), HitLocation, LaunchSpeed, false, 0, 0, ESuggestProjVelocityTraceOption::DoNotTrace)) {
			FVector AimDirection = TossVelocity.GetSafeNormal();
			MoveBarrelTo(AimDirection);
		}
	}
}

void UTankAimingComponent::MoveBarrelTo(FVector AimDirection) {
	if (ensure(Barrel) && ensure(Turret)) {
		FRotator AimRotator = AimDirection.Rotation();

		FRotator BarrelRotator = Barrel->GetForwardVector().Rotation();
		FRotator DeltaRotatorBarrel = AimRotator - BarrelRotator;
		Barrel->Elevate(DeltaRotatorBarrel.Pitch);

		FRotator TurretRotator = Turret->GetForwardVector().Rotation();
		FRotator DeltaRotatorTurret = AimRotator - TurretRotator;
		Turret->Rotate(DeltaRotatorTurret.Yaw);
	}
}

void UTankAimingComponent::Fire() {
	if (FiringState != EFiringState::Reloading) {
		if (ensure(Barrel) && ensure(ProjectileBP)) {
			AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileBP, Barrel->GetSocketLocation(FName("Projectile")), Barrel->GetSocketRotation(FName("Projectile")));
			if (ensure(Projectile)) {
				Projectile->LaunchProjectile(LaunchSpeed);
				LastFireTime = FPlatformTime::Seconds();
			}
		}
	}
}
