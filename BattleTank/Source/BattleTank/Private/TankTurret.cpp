// Fill out your copyright notice in the Description page of Project Settings.

#include "TankTurret.h"
#include "Engine/World.h"

void UTankTurret::Rotate(float RelativeSpeed) {
	if (ensure(GetWorld())) {
		auto RotationChange = FMath::Clamp<float>(RelativeSpeed, -1, 1) * MaxDegreesPerSecond * GetWorld()->DeltaTimeSeconds;
		SetRelativeRotation(FRotator(0, RelativeRotation.Yaw + RotationChange, 0));
	}
}