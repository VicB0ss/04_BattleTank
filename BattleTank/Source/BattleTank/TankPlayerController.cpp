// Fill out your copyright notice in the Description page of Project Settings.

#include "TankPlayerController.h"
#include "Engine/World.h"
#include "Tank.h"

void ATankPlayerController::BeginPlay()
{
	Super::BeginPlay();

	auto TankControlled = GetControlledTank();
	if (!TankControlled)
	{
		UE_LOG(LogTemp, Warning, TEXT("Couldn't get Tank."));
	}
	else 
	{
		UE_LOG(LogTemp, Warning, TEXT("%s possesed!"), *GetPawn()->GetName());
	}

	UE_LOG(LogTemp, Warning, TEXT("Tank controller Begin Play"));
}

void ATankPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	AimTowardsCrosshair();
}

ATank* ATankPlayerController::GetControlledTank() const
{
	return Cast<ATank> (GetPawn());
}

void ATankPlayerController::AimTowardsCrosshair()
{
	if (!GetControlledTank()) { return; }

	FVector HitLocation; //OUT parameter
	if (GetSightRayHitLocation(HitLocation)) 
	{
		GetControlledTank()->AimAt(HitLocation);
	}
}

bool ATankPlayerController::GetSightRayHitLocation(FVector& OutHitLocation) const
{
	int32 ViewportSizeX, ViewportSizeY;
	GetViewportSize(ViewportSizeX, ViewportSizeY);


	auto ScreenLocation = FVector2D(CrossHairLocationX*ViewportSizeX, CrossHairLocationY*ViewportSizeY);

	FVector CameraWorldLocation;
	FVector LookDirection;

	if(GetLookDirection(CameraWorldLocation, LookDirection)) 
	{
		GetLookVectorHitLocation(LookDirection,OutHitLocation);
	}
	return true;
}

bool ATankPlayerController::GetLookDirection(FVector & ScreenLocation, FVector & LookDirection) const
{

	return DeprojectScreenPositionToWorld(ScreenLocation.X,
		ScreenLocation.Y,
		ScreenLocation,
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
		ECC_Visibility)) 
	{
		HitLocation = HitResult.Location;
		return true;
	}
	HitLocation = FVector(0.0);
	return false;
}

