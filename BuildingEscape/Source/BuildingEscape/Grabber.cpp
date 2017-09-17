// Copiright scheme for n!Games is under development.

#include "Grabber.h"
#include "GameFramework/Actor.h"
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "Public/DrawDebugHelpers.h"
#include "PhysicsEngine/PhysicsHandleComponent.h"
#include "Components/InputComponent.h"


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

	FindPhysicsHandleComponent();
	SetupInputComponent();
	
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FVector LineTraceEnd = GetLineTraceEnd();
	 
	/* FString debugprint = LineTraceEnd.ToString();
	UE_LOG(LogTemp, Warning, TEXT("Line trace end is %s"), *debugprint); */
	
	// If physics handle is attached
	if (!PhysicsHandle) { return; }
	if (PhysicsHandle->GrabbedComponent)
	{
		// move the object that we are holding
		PhysicsHandle->SetTargetLocation(LineTraceEnd);

	}	
}

// Get player view point 
const FVector UGrabber::GetLineTraceEnd()
{
	PlayerViewInfo PVI = GetViewInfo();
	FVector LineTraceEnd = PVI.ViewPointLocation + PVI.ViewPointRotation.Vector() * Reach;
	return LineTraceEnd;
}

PlayerViewInfo UGrabber::GetViewInfo()
{
	PlayerViewInfo PVI;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
		OUT PVI.ViewPointLocation,
		OUT PVI.ViewPointRotation
	);

	return PVI;
}

void UGrabber::Grab() {
	UE_LOG(LogTemp, Warning, TEXT("Grab pressed."))

		// LINE TRACE Try and reach any actor with physics body collision channel set
	auto HitResult = GetFirstPhysicsBodyInReach();
	auto ComponentToGrab = HitResult.GetComponent();
	AActor* ActorHit = HitResult.GetActor();
		
	// if we hit, attach a physics handle
	if (ActorHit){
		if (!PhysicsHandle) { return; }
		PhysicsHandle->GrabComponent(
			ComponentToGrab,
			NAME_None,
			ComponentToGrab->GetOwner()->GetActorLocation(),
			true				// allow rotation of grabbed object
			);
	}
	return;
}

/// release physics handle
void UGrabber::Release() {
	UE_LOG(LogTemp, Warning, TEXT("Grab released."))
	if (!PhysicsHandle) { return; }
	PhysicsHandle->ReleaseComponent();		
}

// Look for attached physics handle
void UGrabber::FindPhysicsHandleComponent() {
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (!PhysicsHandle) {
		UE_LOG(LogTemp, Warning, TEXT("Physics component not found of %s"));
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Physics component found"));
	}
}

// Look for attached input component (only appears at runtime)
void UGrabber::SetupInputComponent()
{	
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent) {
		UE_LOG(LogTemp, Warning, TEXT("InputComponent found"));
		InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Input component unavailable."));
	}
}

const FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{
	// Get player view point
	FVector LineTraceEnd = GetLineTraceEnd();
	PlayerViewInfo PVI = GetViewInfo();

	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());
	FHitResult Hit;

	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		PVI.ViewPointLocation,
		LineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParameters
	);

	/*AActor* Target = Hit.GetActor();

	if (Target) {
		UE_LOG(LogTemp, Warning, TEXT("%s hit confirmed."), *(Target->GetName()));
	}	*/

	return Hit;
}
