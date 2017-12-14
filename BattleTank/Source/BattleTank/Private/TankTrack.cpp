// Fill out your copyright notice in the Description page of Project Settings.

#include "TankTrack.h"

// Sets default values for this component's properties
UTankTrack::UTankTrack() {
	PrimaryComponentTick.bCanEverTick = false;
}

void UTankTrack::BeginPlay() {
	Super::BeginPlay();
	OnComponentHit.AddDynamic(this, &UTankTrack::OnHit);
}

void UTankTrack::SetThrottle(float Throttle) {
	CurrentThrottle = FMath::Clamp<float>(CurrentThrottle + Throttle, -1, 1);
}

void UTankTrack::OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit) {
	DriveTrack();
	ApplySidewaysForce();
	CurrentThrottle = 0;
}

void UTankTrack::DriveTrack() {
	UPrimitiveComponent* TankRoot = Cast<UPrimitiveComponent>(GetOwner()->GetRootComponent());
	if (ensure(TankRoot)) {
		auto ForceApplied = GetForwardVector() * CurrentThrottle * TrackMaxDrivingForce;
		auto ForceLocation = GetComponentLocation();
		TankRoot->AddForceAtLocation(ForceApplied, ForceLocation);
	}
}

void UTankTrack::ApplySidewaysForce() {
	// Speed on the side
	auto SlippageSpeed = FVector::DotProduct(GetRightVector(), GetComponentVelocity());
	// Acceleration required this frame to correct:
	auto DeltaTime = GetWorld()->GetDeltaSeconds();
	auto CorrectionAcceleration = -SlippageSpeed / DeltaTime * GetRightVector();
	// Calculate and aply sideways force:
	auto TankRoot = Cast<UStaticMeshComponent>(GetOwner()->GetRootComponent());
	if (ensure(TankRoot)) {
		auto CorrectionForce = (TankRoot->GetMass() * CorrectionAcceleration) / 2;
		TankRoot->AddForce(CorrectionForce);
	}
}