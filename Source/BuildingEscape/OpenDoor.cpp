// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenDoor.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "GameFramework/Pawn.h"
#include "GameFramework/PlayerController.h"
#include "Components/PrimitiveComponent.h"

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
	
	Owner = GetOwner();

	if (PressurePlate == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("%s does not have PressurePlate specified"), *Owner->GetName());
	}
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);



	// ...
	if (GetTotalMassOfActorsOnPlate() > 50.0f)
	{
		OpenDoor();
	}

	if (GetWorld()->GetTimeSeconds() - DoorLastOpenTime > DoorCloseDelay)
	{
		CloseDoor();
	}
}

void UOpenDoor::OpenDoor()
{
	Owner->SetActorRotation(FRotator(0.f, OpenAngle, 0.f));
	DoorLastOpenTime = GetWorld()->GetTimeSeconds();
}

void UOpenDoor::CloseDoor()
{
	Owner->SetActorRotation(FRotator(0.f, 0, 0.f));
}

float UOpenDoor::GetTotalMassOfActorsOnPlate()
{
	float totalMass = 0.0f;
	if (PressurePlate)
	{
		TArray<AActor *> overlappingActors;
		PressurePlate->GetOverlappingActors(overlappingActors);

		for (const auto *actor : overlappingActors)
		{
			UE_LOG(LogTemp, Warning, TEXT("Weighing actor %s"), *actor->GetName());
			totalMass += actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
		}
	}
	return totalMass;
}
