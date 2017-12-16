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
	if (ensure(GetWorld())) {
		LastFireTime = GetWorld()->GetTimeSeconds();
	}
}

void UTankAimingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction * ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (ensure(GetWorld())) {
		if (RoundsLeft <= 0) {
			FiringState = EFiringState::OutOfAmmo;
		} else if ((GetWorld()->GetTimeSeconds() - LastFireTime) < ReloadTimeInSeconds) {
			FiringState = EFiringState::Reloading;
		} else if (IsBarrelMoving()) {
			FiringState = EFiringState::Aiming;
		} else {
			FiringState = EFiringState::Locked;
		}
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
			AimDirection = TossVelocity.GetSafeNormal();
			MoveBarrelTo(AimDirection);
		}
	}
}

void UTankAimingComponent::MoveBarrelTo(FVector AimDirection) {
	if (ensure(Barrel) && ensure(Turret)) {
		FRotator BarrelRotator = Barrel->GetForwardVector().Rotation();
		FRotator DeltaRotator = AimDirection.Rotation() - BarrelRotator;
		Barrel->Elevate(DeltaRotator.Pitch);
		if (FMath::Abs(DeltaRotator.Yaw) < 180) {
			Turret->Rotate(DeltaRotator.Yaw);
		} else {
			Turret->Rotate(-DeltaRotator.Yaw);
		}
	}
}

bool UTankAimingComponent::IsBarrelMoving() const {
	if (ensure(Barrel)) {
		return !AimDirection.Equals(Barrel->GetForwardVector(), 0.01);
	}
	return true;
}

void UTankAimingComponent::Fire() {
	if (FiringState == EFiringState::Locked || FiringState == EFiringState::Aiming) {
		if (ensure(Barrel) && ensure(ProjectileBP) && ensure(GetWorld())) {
			AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileBP, Barrel->GetSocketLocation(FName("Projectile")), Barrel->GetSocketRotation(FName("Projectile")));
			if (ensure(Projectile)) {
				Projectile->LaunchProjectile(LaunchSpeed);
				LastFireTime = GetWorld()->GetTimeSeconds();
				RoundsLeft--;
			}
		}
	}
}

EFiringState UTankAimingComponent::GetFiringState() const {
	return FiringState;
}

int UTankAimingComponent::GetRoundsLeft() const {
	return RoundsLeft;
}