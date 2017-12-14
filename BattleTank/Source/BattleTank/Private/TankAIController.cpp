// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAIController.h"
#include "TankAimingComponent.h"
#include "Engine/World.h"

// Called when the game starts or when spawned
void ATankAIController::BeginPlay() {
	Super::BeginPlay();
}

// Called every frame
void ATankAIController::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	auto PlayerContoller = GetWorld()->GetFirstPlayerController();
	if (ensure(PlayerContoller)) {
		auto PlayerTank = PlayerContoller->GetPawn();
		UTankAimingComponent* AimingComponent = GetPawn()->FindComponentByClass<UTankAimingComponent>();
		if (ensure(PlayerTank) && ensure(AimingComponent)) {
			MoveToActor(PlayerTank, AcceptanceRadius);
			AimingComponent->AimAt(PlayerTank->GetActorLocation());
			if (AimingComponent->GetFiringState() == EFiringState::Locked) {
				AimingComponent->Fire();
			}
		}
	}
}