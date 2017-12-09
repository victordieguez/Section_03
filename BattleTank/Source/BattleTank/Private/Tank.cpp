// Fill out your copyright notice in the Description page of Project Settings.

#include "Tank.h"
#include "TankAimingComponent.h"
#include "TankBarrel.h"
#include "Projectile.h"
#include "Engine/World.h"

// Sets default values
ATank::ATank() {
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	TankAimingComponent = CreateDefaultSubobject<UTankAimingComponent>(FName("Aiming Component"));
}

// Called when the game starts or when spawned
void ATank::BeginPlay() {
	Super::BeginPlay();
}

// Called to bind functionality to input
void ATank::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) {
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ATank::SetBarrel(UTankBarrel* Barrel) {
	this->Barrel = Barrel;
	if (TankAimingComponent) {
		TankAimingComponent->SetBarrel(Barrel);
	}
}

void ATank::SetTurret(UTankTurret* Turret) {
	if (TankAimingComponent) {
		TankAimingComponent->SetTurret(Turret);
	}
}

void ATank::SetRightTrack(UTankTrack* Track) {
	this->RightTrack = Track;
}

void ATank::SetLeftTrack(UTankTrack* Track) {
	this->LeftTrack = Track;
}

void ATank::AimAt(FVector HitLocation) {
	if (TankAimingComponent) {
		TankAimingComponent->AimAt(HitLocation, LaunchSpeed);
	}
}

void ATank::Fire() {
	bool isReloaded = (FPlatformTime::Seconds() - LastFireTime) > ReloadTimeInSeconds;
	if (Barrel && isReloaded) {
		AProjectile* Projectile = GetWorld()->SpawnActor<AProjectile>(ProjectileBP, Barrel->GetSocketLocation(FName("Projectile")), Barrel->GetSocketRotation(FName("Projectile")));
		if (Projectile) {
			Projectile->LaunchProjectile(LaunchSpeed);
		}
		LastFireTime = FPlatformTime::Seconds();
	}
}
