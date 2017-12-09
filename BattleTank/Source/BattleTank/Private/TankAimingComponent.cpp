// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAimingComponent.h"
#include "TankBarrel.h"
#include "TankTurret.h"
#include "Kismet/GameplayStatics.h"

// Sets default values for this component's properties
UTankAimingComponent::UTankAimingComponent() {
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
}

// Called when the game starts
void UTankAimingComponent::BeginPlay() {
	Super::BeginPlay();
}

// Called every frame
void UTankAimingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) {
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UTankAimingComponent::AimAt(FVector HitLocation, float LaunchSpeed) {
	if (Barrel && Turret) {
		FVector TossVelocity;
		if (UGameplayStatics::SuggestProjectileVelocity(this, TossVelocity, Barrel->GetSocketLocation(FName("Projectile")), HitLocation, LaunchSpeed, false, 0, 0, ESuggestProjVelocityTraceOption::DoNotTrace)) {
			FVector AimDirection = TossVelocity.GetSafeNormal();
			MoveBarrelTo(AimDirection);
		}
	}
}

void UTankAimingComponent::SetBarrel(UTankBarrel* Barrel) {
	this->Barrel = Barrel;
}

void UTankAimingComponent::SetTurret(UTankTurret* Turret) {
	this->Turret = Turret;
}

void UTankAimingComponent::MoveBarrelTo(FVector AimDirection) {
	if (Barrel && Turret) {
		FRotator AimRotator = AimDirection.Rotation();

		FRotator BarrelRotator = Barrel->GetForwardVector().Rotation();
		FRotator DeltaRotatorBarrel = AimRotator - BarrelRotator;
		Barrel->Elevate(DeltaRotatorBarrel.Pitch);

		FRotator TurretRotator = Turret->GetForwardVector().Rotation();
		FRotator DeltaRotatorTurret = AimRotator - TurretRotator;
		Turret->Rotate(DeltaRotatorTurret.Yaw);
	}
}