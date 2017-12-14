// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/StaticMeshComponent.h"
#include "TankTrack.generated.h"

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class BATTLETANK_API UTankTrack : public UStaticMeshComponent {
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UTankTrack();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void ApplySidewaysForce();

	//Sets a throttle between -1 and +1
	UFUNCTION(BlueprintCallable, Category = Input)
	void SetThrottle(float Throttle);

	//Max force per track in newtons
	UPROPERTY(EditDefaultsOnly, Category = Setup)
	float TrackMaxDrivingForce = 1000000;

private:
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComponent, FVector NormalImpulse, const FHitResult& Hit);
	
	void DriveTrack();

	float CurrentThrottle = 0;

};