// Fill out your copyright notice in the Description page of Project Settings.

#include "TankAIController.h"
#include "Engine/World.h"
#include "Tank.h"
#include "Runtime/Engine/Classes/GameFramework/PlayerController.h"

void ATankAIController::BeginPlay()
{
	Super::BeginPlay();

	auto PlayerTank = GetPlayerTank();
	if (!PlayerTank)
	{
		UE_LOG(LogTemp, Warning, TEXT("IA Tank couldn't find Player Controller."));
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("%s Player found!."), *PlayerTank->GetName());
	}
}


void ATankAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (GetPlayerTank()) 
	{
		GetControlledIATank()->AimAt(GetPlayerTank()->GetActorLocation());
	}
	
}

ATank * ATankAIController::GetControlledIATank() const
{
	return Cast<ATank> (GetPawn());
}

ATank * ATankAIController::GetPlayerTank() const
{
	auto PlayerPawn = GetWorld()->GetFirstPlayerController()->GetPawn();
	if (!PlayerPawn) 
	{
		return nullptr;
	}
	else {
		return Cast <ATank> (PlayerPawn);
	}
	
}
