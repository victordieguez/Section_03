// Fill out your copyright notice in the Description page of Project Settings.

#include "Tank.h"
#include "TankAIController.h"
#include "Engine/World.h"

// Called when the game starts or when spawned
void ATankAIController::BeginPlay() {
	Super::BeginPlay();

	ATank* ControlledTank = GetControlledTank();
	ATank* PlayerTank = GetPlayerTank();
	if (ControlledTank && PlayerTank) {
		UE_LOG(LogTemp, Warning, TEXT("AIController: %s PlayerTank found: %s"), *ControlledTank->GetName(), *PlayerTank->GetName());
	} else {
		UE_LOG(LogTemp, Warning, TEXT("AIController: No PlayerTank found"));
	}
}

// Called every frame
void ATankAIController::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	ATank* ControlledTank = GetControlledTank();
	ATank* PlayerTank = GetPlayerTank();
	if (ControlledTank && PlayerTank) {
		ControlledTank->AimAt(PlayerTank->GetActorLocation());
	}
}

ATank* ATankAIController::GetControlledTank() const {
	return Cast<ATank>(GetPawn());
}

ATank* ATankAIController::GetPlayerTank() const {
	auto PlayerContoller = GetWorld()->GetFirstPlayerController();
	if (PlayerContoller && PlayerContoller->GetPawn()) {
		return Cast<ATank>(PlayerContoller->GetPawn());
	}
	return nullptr;
}