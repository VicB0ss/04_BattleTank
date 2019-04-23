// Fill out your copyright notice in the Description page of Project Settings.

#include "TankTrack.h"
#include "SprungWheel.h"
#include "Components/SceneComponent.h"
#include "SpawnPoint.h"

UTankTrack::UTankTrack() 
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UTankTrack::DriveTrack(float CurrentThrottle)
{
	auto AppliedForce = CurrentThrottle * TrackMaxDrivingForce;
	auto Wheels = GetWheels();
	auto ForcePerWheel = AppliedForce / Wheels.Num();
	for (ASprungWheel* Wheel : Wheels) 
	{
		Wheel->AddDrivingForce(ForcePerWheel);
	}
}
TArray<ASprungWheel*> UTankTrack::GetWheels() const
{
	TArray<ASprungWheel*> ResultArray;
	TArray<USceneComponent*> Children;
	GetChildrenComponents(true, Children);
	for (USceneComponent* Child : Children) 
	{
		auto SpawnPointChild = Cast<USpawnPoint>(Child);
		if (!SpawnPointChild) continue;
		AActor* SpawnedChild = SpawnPointChild->GetSpawnedActor();
		auto SprungWheel = Cast<ASprungWheel>(SpawnedChild);
		if (!SprungWheel) continue;
		ResultArray.Add(SprungWheel);
	}
	return ResultArray;
}
void UTankTrack::SetThrottle(float Throttle) 
{
	float CurrentThrottle = FMath::Clamp<float>(Throttle, -1, 1);
	DriveTrack(CurrentThrottle);
}