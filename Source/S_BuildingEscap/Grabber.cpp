// Fill out your copyright notice in the Description page of Project Settings.


#include "Grabber.h"
#include "DrawDebugHelpers.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"

#define OUT 

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	
	FindPhysicsHandle();

	SetupInputComponent();
}

void UGrabber::SetupInputComponent()
{
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();

	if (InputComponent)
	{
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
}

void UGrabber::FindPhysicsHandle()
{
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
}

void UGrabber::Grab()
{
	UE_LOG(LogTemp, Warning, TEXT("Right mouse button pressed."));

	FVector PlayerViewpointLocation;
	FRotator PlayerViewpointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewpointLocation, OUT PlayerViewpointRotation);
	FVector LineTraceEnd = PlayerViewpointLocation + PlayerViewpointRotation.Vector() * Reach;

	FHitResult HitResult = GetFirstPhysicsBodyInReach();
	UPrimitiveComponent* ComponentToGrab = HitResult.GetComponent();

	AActor* HitActor = HitResult.GetActor();
	if (HitActor)
	{
		PhysicsHandle->GrabComponentAtLocation(ComponentToGrab, NAME_None, LineTraceEnd);
	}
}

void UGrabber::Release()
{
	UE_LOG(LogTemp, Warning, TEXT("Right mouse Button Released."));
	PhysicsHandle->ReleaseComponent();
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	FVector PlayerViewpointLocation;
	FRotator PlayerViewpointRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewpointLocation, OUT PlayerViewpointRotation);
	FVector LineTraceEnd = PlayerViewpointLocation + PlayerViewpointRotation.Vector() * Reach;

	if (PhysicsHandle->GrabbedComponent)
	{
		PhysicsHandle->SetTargetLocation(LineTraceEnd);
	}

}

FHitResult UGrabber::GetFirstPhysicsBodyInReach() const
{
	FVector PlayerViewpointLocation;
	FRotator PlayerViewpointRotation;

	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewpointLocation, OUT PlayerViewpointRotation);

	FVector LineTraceEnd = PlayerViewpointLocation + PlayerViewpointRotation.Vector() * Reach;

	FHitResult Hit;
	FCollisionQueryParams TraceParams(FName(TEXT("")), false, GetOwner());

	GetWorld()->LineTraceSingleByObjectType(OUT Hit, PlayerViewpointLocation, LineTraceEnd, FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody), TraceParams);

	AActor* ActorHit = Hit.GetActor();

	if (ActorHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("Line trace has hit : %s"), *(ActorHit->GetName()));
	}

	return Hit;
}

/*FVector UGrabber::GetPlayerReach() const
{
	return FVector();
}*/
