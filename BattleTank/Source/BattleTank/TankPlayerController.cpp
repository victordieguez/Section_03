// Fill out your copyright notice in the Description page of Project Settings.

#include "TankPlayerController.h"
#include "TankAimingComponent.h"
#include "Engine/World.h"
#include "Tank.h"

// Called when the game starts or when spawned
void ATankPlayerController::BeginPlay() {
	Super::BeginPlay();
	if (GetPawn()) {
		UTankAimingComponent* AimingComponent = GetPawn()->FindComponentByClass<UTankAimingComponent>();
		if (ensure(AimingComponent)) {
			FoundAimingComponent(AimingComponent);
		}
	}
}

// Called every frame
void ATankPlayerController::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);
	AimTowardsCrossHair();
}

void ATankPlayerController::AimTowardsCrossHair() {
	if (GetPawn()) {
		UTankAimingComponent* AimingComponent = GetPawn()->FindComponentByClass<UTankAimingComponent>();
		if (ensure(AimingComponent)) {
			FVector HitLocation;
			if (GetSightRayHitLocation(HitLocation)) {
				AimingComponent->AimAt(HitLocation);
			}
		}
	}
	return;
}

bool ATankPlayerController::GetSightRayHitLocation(FVector& Out_HitLocation) const {
	int32 ViewportSizeX, ViewPortSizeY;
	GetViewportSize(ViewportSizeX, ViewPortSizeY);

	FVector2D ScreenLocation = FVector2D(ViewportSizeX * CrossHairXLocation, ViewPortSizeY * CrossHairYLocation);
	FVector LookDirection;
	if (GetLookDirection(ScreenLocation, LookDirection)) {
		return GetLookVectorHitLocation(LookDirection, Out_HitLocation);
	}
	return false;
}

bool ATankPlayerController::GetLookDirection(FVector2D ScreenLocation, FVector& Out_LookDirection) const {
	FVector WorldLocation;
	return DeprojectScreenPositionToWorld(ScreenLocation.X, ScreenLocation.Y, WorldLocation, Out_LookDirection);
}

bool ATankPlayerController::GetLookVectorHitLocation(FVector LookDirection, FVector& Out_HitLocation) const {
	FHitResult HitResult;
	if (ensure(PlayerCameraManager) && ensure(GetWorld())) {
		FVector StartLocation = PlayerCameraManager->GetCameraLocation();
		if (GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, StartLocation + LookDirection * LineTraceRange, ECollisionChannel::ECC_Camera)) {
			Out_HitLocation = HitResult.Location;
			return true;
		}
	}
	return false;
}


void ATankPlayerController::SetPawn(APawn* InPawn) {
	Super::SetPawn(InPawn);
	if (ensure(InPawn)) {
		ATank* PossesedTank = Cast<ATank>(InPawn);
		if (ensure(PossesedTank)) {
			///Subscribe to tank's death event:
			PossesedTank->OnDeath.AddUniqueDynamic(this, &ATankPlayerController::OnTankDeath);
		}
	}
}

void ATankPlayerController::OnTankDeath() {
	UE_LOG(LogTemp, Warning, TEXT("Player Tank died"));
	StartSpectatingOnly();
}