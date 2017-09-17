// Copiright scheme for n!Games is under development.

#include "OpenDoor.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "Components/PrimitiveComponent.h"
#include "GameFramework/PlayerController.h"

#define OUT

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

	Owner = GetOwner();
	Owner->SetActorRotation(FRotator(0.0f, ClosedAngle, 0.0f));
}

void UOpenDoor::OpenDoor()
{
	/*FRotator NewRotation = FRotator(0.0f, OpenAngle, 0.0f);
	AActor* Owner = GetOwner();
	Owner->SetActorRotation(NewRotation);*/

	OnOpenRequest.Broadcast();
}

void UOpenDoor::CloseDoor()
{
	FRotator NewRotation = FRotator(0.0f, ClosedAngle, 0.0f);
	Owner = GetOwner();
	Owner->SetActorRotation(NewRotation);
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// Poll the TriggerVolume
	// If enough mass is in the volume, open the door
	if (GetTotalMassOnTriggerPlate() > PlateTriggerValue){
		OpenDoor();
		LastDoorOpenTime = GetWorld()->GetTimeSeconds();
	}

	// If the specified amount of time has passed, close the door
	if (GetWorld()->GetTimeSeconds() - LastDoorOpenTime > DoorCloseDelay){
		CloseDoor();
	}
}

float UOpenDoor::GetTotalMassOnTriggerPlate() {
	float TotalMass = 0.f;

	/// find all overlaping actors and add to TArray
	TArray<AActor*> OverlappingActors;
	if (!PressurePlate) { UE_LOG(LogTemp,Warning,TEXT("%s missing pressure plate."), *Owner->GetName()); return 0.f; }
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);

	/// iterate over TArray to sum actors' masses
	for (const auto* actorOnPlate : OverlappingActors)
	{
		if (!actorOnPlate) { return 0.f; }
		UE_LOG(LogTemp, Warning, TEXT("%s is on the plate!"), *actorOnPlate->GetName());
		TotalMass += actorOnPlate->FindComponentByClass<UPrimitiveComponent>()->GetMass();
		
	}

	return TotalMass;
}

