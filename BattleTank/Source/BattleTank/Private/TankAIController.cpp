// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAIController.h"
#include "TankAimingComponent.h"
#include "Engine/World.h"
#include "Tank.h"

// Called when the game starts or when spawned
void ATankAIController::BeginPlay() {
	Super::BeginPlay();
}

// Called every frame
void ATankAIController::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	if (ensure(GetWorld()) && GetPawn()) {
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
}

void ATankAIController::SetPawn(APawn* InPawn) {
	Super::SetPawn(InPawn);
	if (ensure(InPawn)) {
		ATank* PossesedTank = Cast<ATank>(InPawn);
		if (ensure(PossesedTank)) {
			///Subscribe to tank's death event:
			PossesedTank->OnDeath.AddUniqueDynamic(this, &ATankAIController::OnTankDeath);
		}
	}
}

void ATankAIController::OnTankDeath() {
	UE_LOG(LogTemp, Warning, TEXT("AI Tank died"));
	APawn* Pawn = GetPawn();
	if (ensure(Pawn)) {
		Pawn->DetachFromControllerPendingDestroy();
	}
}