// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"
#include "Engine/World.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"
#include "DrawDebugHelpers.h"

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	// ...
	UE_LOG(LogTemp, Warning, TEXT("Grabber reporting for duty!"));

	/// Attach physics handle
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();

	if (PhysicsHandle)
	{

	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Unable to find physics handle on %s"), *GetOwner()->GetName());
	}
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	// ...
	FVector viewPosition;
	FRotator viewRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(viewPosition, viewRotation);

	FVector traceEnd = viewPosition + viewRotation.Vector() * Reach; // FVector(0.0f, 0.0f, 20.0f);
	DrawDebugLine(GetWorld(), viewPosition, traceEnd, FColor(255, 0, 0), false, 0.0f, 0.0f, 10.0f);

	/// Trace out to the world to find a physics object
	FCollisionQueryParams traceParams(FName(TEXT("")), false, GetOwner());
	FHitResult hitResult;
	GetWorld()->LineTraceSingleByObjectType(hitResult, viewPosition, traceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		traceParams);

	/// See what we hit
	AActor * hitActor = hitResult.GetActor();
	if (hitActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("Grabber hit %s"), *hitActor->GetName());
	}
}

