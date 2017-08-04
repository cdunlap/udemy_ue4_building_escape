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

	FindPhysicsHandleComponent();
	SetupInputComponent();
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	/// If we're carrying anything, update it
	if (PhysicsHandle->GrabbedComponent)
	{
		FVector viewPosition;
		FRotator viewRotation;
		GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(viewPosition, viewRotation);

		FVector traceEnd = viewPosition + viewRotation.Vector() * Reach;

		PhysicsHandle->SetTargetLocation(traceEnd);
	}

}

void UGrabber::FindPhysicsHandleComponent()
{
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

void UGrabber::SetupInputComponent()
{
	/// Get the input component
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent)
	{
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::GrabReleased);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Unable to find input component on %s"), *GetOwner()->GetName());
	}
}

FHitResult UGrabber::GetFirstPhysicsBodyInReach() const
{
	FVector viewPosition;
	FRotator viewRotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(viewPosition, viewRotation);

	FVector traceEnd = viewPosition + viewRotation.Vector() * Reach; // FVector(0.0f, 0.0f, 20.0f);

																	 /// Trace out to the world to find a physics object
	FCollisionQueryParams traceParams(FName(TEXT("")), false, GetOwner());
	FHitResult hitResult;
	GetWorld()->LineTraceSingleByObjectType(hitResult, viewPosition, traceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		traceParams);

	return hitResult;
}

void UGrabber::Grab()
{
	UE_LOG(LogTemp, Warning, TEXT("Grabber key pressed"));

	/// Attempt to reach out to physics body with collision channel set
	FHitResult hitResult = GetFirstPhysicsBodyInReach();

	/// If we hit, attach a physics handle
	UPrimitiveComponent * hitComponent = hitResult.GetComponent();
	AActor * hitActor = hitResult.GetActor();

	if (hitActor)
	{
		UE_LOG(LogTemp, Warning, TEXT("Grabber hit %s"), *hitActor->GetName());
		
		PhysicsHandle->GrabComponent(hitComponent, NAME_None, hitActor->GetActorLocation(), true);
	}
}

void UGrabber::GrabReleased()
{
	UE_LOG(LogTemp, Warning, TEXT("Grabber key released"));

	/// Release physics handle
	if (PhysicsHandle->GrabbedComponent)
	{
		PhysicsHandle->ReleaseComponent();
	}
}