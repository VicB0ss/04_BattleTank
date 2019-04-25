// Fill out your copyright notice in the Description page of Project Settings.

#include "TankPlayerController.h"
#include "Engine/World.h"
#include "TankAimingComponent.h"

void ATankPlayerController::BeginPlay()
{
	Super::BeginPlay();
	if (!GetPawn()) { return; }
	auto AimingComponent = GetPawn()->FindComponentByClass<UTankAimingComponent>();
	if (!ensure(AimingComponent)) { return; }
	FoundAimingComponent(AimingComponent);
}

void ATankPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	AimTowardsCrosshair();
}

void ATankPlayerController::OnTankDeath()
{
	StartSpectatingOnly();
}


void ATankPlayerController::AimTowardsCrosshair()
{
	if (!GetPawn()) { return; }
	auto AimingComponent = GetPawn()->FindComponentByClass<UTankAimingComponent>();
	if (!ensure(AimingComponent)) { return; }
	FVector HitLocation; //OUT parameter
	bool bGotHitLocation = GetSightRayHitLocation(HitLocation);
	if (bGotHitLocation)
	{
		AimingComponent->AimAt(HitLocation);
	}
}

bool ATankPlayerController::GetSightRayHitLocation(FVector& OutHitLocation) const
{
	int32 ViewportSizeX, ViewportSizeY;
	GetViewportSize(ViewportSizeX, ViewportSizeY);

	auto ScreenLocation = FVector2D(CrossHairLocationX*ViewportSizeX, CrossHairLocationY*ViewportSizeY);

	FVector LookDirection;

	if(GetLookDirection(ScreenLocation, LookDirection)) 
	{
		return GetLookVectorHitLocation(LookDirection,OutHitLocation);
	}
	return false;
}

bool ATankPlayerController::GetLookDirection(FVector2D & ScreenLocation, FVector & LookDirection) const
{
	FVector CameraWorldLocation(0);
	return DeprojectScreenPositionToWorld(ScreenLocation.X,
		ScreenLocation.Y,
		CameraWorldLocation,
		LookDirection);
}

bool ATankPlayerController::GetLookVectorHitLocation(FVector LookDirection,FVector & HitLocation) const
{
	FHitResult HitResult;
	auto StartLocation = PlayerCameraManager->GetCameraLocation();
	auto EndLocation = StartLocation + LookDirection * LineTraceRange;
	
	if (GetWorld()->LineTraceSingleByChannel(
		HitResult, 
		StartLocation, 
		EndLocation, 
		ECC_Camera)) 
	{
		HitLocation = HitResult.Location;
		return true;
	}
	HitLocation = FVector(0.0);
	return false;
}

void ATankPlayerController::SetPawn(APawn * InPawn)
{
	Super::SetPawn(InPawn);
	if (InPawn)
	{
		auto PlayerTank = Cast <ATank>(InPawn);
		if (!ensure(PlayerTank)) { return; }
		PlayerTank->OnDeath.AddUniqueDynamic(this, &ATankPlayerController::OnTankDeath);
	}
}