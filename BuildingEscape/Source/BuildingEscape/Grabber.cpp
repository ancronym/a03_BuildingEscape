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
	FHitResult HitResult = GetFirstPhysicsBodyInReach();
	UPrimitiveComponent* ComponentToGrab = HitResult.GetComponent();
	AActor* ActorHit = HitResult.GetActor();
	
	// if we hit, attach a physics handle
	if (ActorHit){
		PhysicsHandle->GrabComponentAtLocation(
			HitResult.GetComponent(),
			NAME_None,
			ActorHit->GetOwner()->GetActorLocation() // allow rotation of grabbed object
		);
	}
}

void UGrabber::Release() {
	UE_LOG(LogTemp, Warning, TEXT("Grab released."))
	PhysicsHandle->ReleaseComponent();
		/// TODO release physics handle
}

void UGrabber::FindPhysicsHandleComponent() {
	/// Look for physics handle
	
	PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
	if (!PhysicsHandle) {
		UE_LOG(LogTemp, Warning, TEXT("Physics component not found of %s"), *(GetOwner()->GetName()));
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Physics component found"));
	}
}

void UGrabber::SetupInputComponent()
{	
	InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
	if (InputComponent) {
		UE_LOG(LogTemp, Warning, TEXT("InputComponent for %s found"), *(GetOwner()->GetName()));
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

	FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());

	FHitResult Hit;

	PlayerViewInfo PVI = GetViewInfo();

	GetWorld()->LineTraceSingleByObjectType(
		OUT Hit,
		PVI.ViewPointLocation,
		LineTraceEnd,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody),
		TraceParameters
	);

	AActor* Target = Hit.GetActor();

	if (Target) {
		UE_LOG(LogTemp, Warning, TEXT("%s hit confirmed."), *(Target->GetName()));
	}	

	return Hit;
}
