// Fill out your copyright notice in the Description page of Project Settings.


#include "OpenDoor.h"
#include "GameFramework/Actor.h"

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	/*FRotator OpenDoor(0.f, -90.f, 0.f);

	GetOwner()->SetActorRotation(OpenDoor);*/

	InitialYaw = GetOwner()->GetActorRotation().Yaw;
	CurrentYaw = InitialYaw;
	TargetYaw += InitialYaw;
	
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	/*float CurrentYaw = GetOwner()->GetActorRotation().Yaw;
	float RotationYaw = FMath::Lerp(CurrentYaw, TargetYaw, 0.02f);
	FRotator OpenDoor(0.f, RotationYaw, 0.f);
	GetOwner()->SetActorRotation(OpenDoor);*/

	CurrentYaw = FMath::Lerp(CurrentYaw, TargetYaw, 0.02f);
	FRotator DoorRotation(0.f, CurrentYaw, 0.f);
	GetOwner()->SetActorRotation(DoorRotation);
}

