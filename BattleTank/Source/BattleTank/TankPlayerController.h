// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Tank.h"
#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TankPlayerController.generated.h"

/**
*
*/
UCLASS()
class BATTLETANK_API ATankPlayerController : public APlayerController {
	GENERATED_BODY()

public:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:

	UPROPERTY(EditAnywhere)
	float CrossHairXLocation = 0.5f;
	UPROPERTY(EditAnywhere)
	float CrossHairYLocation = 0.3333333f;
	UPROPERTY(EditAnywhere)
	float LineTraceRange = 1000000.0f;

	ATank* GetControlledTank() const;
	void AimTowardsCrossHair();
	bool GetSightRayHitLocation(FVector& Out_HitLocation) const;
	bool GetLookDirection(FVector2D ScreenLocation, FVector& Out_LookDirection) const;
	bool GetLookVectorHitLocation(FVector LookDirection, FVector& Out_HitLocation) const;

};