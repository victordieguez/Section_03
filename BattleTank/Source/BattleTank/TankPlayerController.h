// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TankPlayerController.generated.h"

//Forward declaration
class UTankAimingComponent;

UCLASS()
class BATTLETANK_API ATankPlayerController : public APlayerController {
	GENERATED_BODY()

public:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:

	UFUNCTION(BlueprintImplementableEvent, Category = "Setup")
	void FoundAimingComponent(UTankAimingComponent* AimingComponent);

private:

	UPROPERTY(EditDefaultsOnly)
	float CrossHairXLocation = 0.5f;
	UPROPERTY(EditDefaultsOnly)
	float CrossHairYLocation = 0.3333333f;
	UPROPERTY(EditDefaultsOnly)
	float LineTraceRange = 1000000.0f;

	void AimTowardsCrossHair();
	bool GetSightRayHitLocation(FVector& Out_HitLocation) const;
	bool GetLookDirection(FVector2D ScreenLocation, FVector& Out_LookDirection) const;
	bool GetLookVectorHitLocation(FVector LookDirection, FVector& Out_HitLocation) const;

};