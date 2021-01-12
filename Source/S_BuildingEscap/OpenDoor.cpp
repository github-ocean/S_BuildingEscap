// Fill out your copyright notice in the Description page of Project Settings.


#include "OpenDoor.h"
#include "Components/PrimitiveComponent.h"
#include "GameFramework/Actor.h"
#include "GameFrameWork/PlayerController.h"
#include "Engine/TriggerVolume.h"
#include "Engine/World.h"

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	/*FRotator OpenDoor(0.f, -90.f, 0.f);

	GetOwner()->SetActorRotation(OpenDoor);*/

	InitialYaw = GetOwner()->GetActorRotation().Yaw;
	CurrentYaw = InitialYaw;
	OpenAngle += InitialYaw;
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	/*float CurrentYaw = GetOwner()->GetActorRotation().Yaw;
	float RotationYaw = FMath::Lerp(CurrentYaw, OpenAngle, 0.02f);
	FRotator OpenDoor(0.f, RotationYaw, 0.f);
	GetOwner()->SetActorRotation(OpenDoor);*/
	
	if (TotalMassOfActors() > 50.f)
	{
		OpenDoor(DeltaTime);
		DoorLastOpened = GetWorld()->GetTimeSeconds();
	}
	else
	{
		if ((GetWorld()->GetTimeSeconds() - DoorLastOpened) > DoorCloseDelay)
		{
			CloseDoor(DeltaTime);
		}
	}
}

void UOpenDoor::OpenDoor(float DeltaTime)
{
	CurrentYaw = FMath::Lerp(CurrentYaw, OpenAngle, DeltaTime * DoorOpenSpeed);
	FRotator DoorRotation(0.f, CurrentYaw, 0.f);
	GetOwner()->SetActorRotation(DoorRotation);
}

void UOpenDoor::CloseDoor(float DeltaTime)
{
	CurrentYaw = FMath::Lerp(CurrentYaw, InitialYaw, DeltaTime * DoorCloseSpeed);
	FRotator DoorRotation(0.f, CurrentYaw, 0.f);
	GetOwner()->SetActorRotation(DoorRotation);
}

float UOpenDoor::TotalMassOfActors() const
{
	float TotalMass = 0.f;

	TArray<AActor*> OverlappingActors;
	PressurePlate->GetOverlappingActors(OverlappingActors);

	for (AActor* Actor : OverlappingActors)
	{
		TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
	}

	return TotalMass;
}

