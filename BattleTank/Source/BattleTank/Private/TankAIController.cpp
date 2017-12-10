// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAIController.h"
#include "Tank.h"
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
		MoveToActor(PlayerTank, AcceptanceRadius);
		ControlledTank->AimAt(PlayerTank->GetActorLocation());
		ControlledTank->Fire();
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